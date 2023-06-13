//
// Created by nami on 10/05/23.
//

#include <Renderer/gl_renderer.h>


namespace Wave
{
  Renderer_api Gl_renderer::api = Renderer_api::OpenGL;
  Renderer_state Gl_renderer::state = {nullptr,
                                       nullptr,
                                       nullptr,
                                       WAVE_RENDERER_INIT};
  std::map<uint32_t, Renderer::Draw_command *> Gl_renderer::draw_commands;
  Camera *Gl_renderer::scene_camera;
  std::map<uint32_t, Texture *> Gl_renderer::textures;
  std::vector<std::shared_ptr<Uniform_buffer>> Gl_renderer::uniform_buffers;
  std::function<void(Event &)> Gl_renderer::event_callback_function;
  Renderer::Renderer_stats_s Gl_renderer::stats = {0};
  
  static char *s_glsl_version = nullptr;
  static std::vector<Renderer::Draw_command *> s_object_draw_commands;
  static int32_t s_texture_unit_number_assigned = 0, s_max_texture_samplers = 4;
  
  bool Gl_renderer::is_running()
  {
    return get_state().code == WAVE_RENDERER_RUNNING;
  }
  
  const Renderer_state &Gl_renderer::get_state()
  {
    return Gl_renderer::state;
  }
  
  const Renderer_api &Gl_renderer::get_api()
  {
    return Gl_renderer::api;
  }
  
  Renderer::Renderer_stats_s Gl_renderer::get_stats()
  {
    return Gl_renderer::stats;
  }
  
  int32_t Gl_renderer::get_max_texture_units()
  {
    GLint result;
    CHECK_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_UNITS, &result));
    return result;
  }
  
  const char *Gl_renderer::get_api_version()
  {
    return (const char *) (glGetString(GL_VERSION));
  }
  
  const char *Gl_renderer::get_api_shader_version()
  {
    std::string version_and_name = Renderer::get_api_version();
    if (snprintf(s_glsl_version, LINE_MAX, "#version %c%c%c",
                 glGetString(GL_SHADING_LANGUAGE_VERSION)[0],
                 glGetString(GL_SHADING_LANGUAGE_VERSION)[2],
                 glGetString(GL_SHADING_LANGUAGE_VERSION)[3]) < 0)
    {
      return "#version 330";  // Default version.
    }
    return s_glsl_version;
  }
  
  [[maybe_unused]] const std::function<void(Event &event)> &Gl_renderer::get_event_callback_function()
  {
    return Gl_renderer::event_callback_function;
  }
  
  void Gl_renderer::set_event_callback_function(const std::function<void(Event &)> &event_callback_function_)
  {
    Gl_renderer::event_callback_function = event_callback_function_;
  }
  
  void Gl_renderer::set_state(Renderer_state new_state)
  {
    Gl_renderer::state = new_state;
  }
  
  void Gl_renderer::set_viewport(float width, float height)
  {
    if (width <= 0.0f || height <= 0.0f)
      gl_synchronous_error_callback(WAVE_GL_INVALID_VIEWPORT_DIMENSIONS, "Invalid viewport size given!", __FUNCTION__,
                                    __FILE__, __LINE__);
    CHECK_GL_CALL(glViewport(0, 0, width, height));
  }
  
  void Gl_renderer::init()
  {
    s_glsl_version = (char *) calloc(1, LINE_MAX);
    WAVE_LOG_TASK("GL renderer", CYAN, 1, "Loading openGL renderer ...",
                  {
                    WAVE_LOG_INSTRUCTION("GL renderer", DEFAULT, "Loading GLEW", CHECK_GL_CALL(glewInit()))
                    Gl_renderer::set_state(Gl_renderer::get_state());
                    
                    /****** Enable OpenGL error handling. ********/
                    // Check if we successfully initialized a debug context
                    int flags;
                    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
                    // Get max number of simultaneous texture upload to the GPU.
                    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &s_max_texture_samplers);
                    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
                    {
                      glEnable(GL_DEBUG_OUTPUT);  // Enable debug output.
                      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);  // Call the callback as soon as there's an error.
                    }
                    
                    Color background_color;  // Default dark-mode-like color for background.
                    CHECK_GL_CALL(glClearColor(background_color.get_red(), background_color.get_green(),
                                               background_color.get_blue(), background_color.get_alpha()));
                    
                    CHECK_GL_CALL(glEnable(GL_CULL_FACE));
                    CHECK_GL_CALL(
                      glCullFace(GL_BACK));  // Don't update the back face of shapes since the camera won't see it.
                    CHECK_GL_CALL(
                      glFrontFace(GL_CW));  // Every shape drawn in clock-wise manner will be considered the FRONT face.
                    
                    // Let OpenGL keep track of depth for shapes and auto determine if some shapes closer or further away from
                    // the camera should take priority (drawn on top of other ones).
                    CHECK_GL_CALL(glEnable(GL_DEPTH_TEST));
                    CHECK_GL_CALL(glEnable(GL_MULTISAMPLE));
                    CHECK_GL_CALL(glEnable(GL_BLEND));
                    CHECK_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
                  }, "GL renderer built")
    Gl_renderer::set_state({nullptr, nullptr, nullptr, WAVE_RENDERER_RUNNING});

#if defined(OPENGL_VERSION_4_3_PLUS) && defined(DEBUG)
    glDebugMessageCallback(gl_asynchronous_error_callback, nullptr);
    // Filter the messages we want to debug or not with the last param 'enabled'.
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
#endif
    
    WAVE_LOG_TASK("GL renderer", YELLOW, 1, "Fetching renderer info on client system ...",
                  Gl_renderer::show_renderer_info(),
                  "GL renderer info fetched")
  }
  
  void Gl_renderer::init_text_draw_command(Shader &shader_linked)
  {
    WAVE_PROFILE_INSTRUCTIONS("Pre-rendering text", RED, 5,
                              {
                                // Add draw command for batch rendering.
                                Renderer::Draw_command initial_command;
                                std::vector<Buffer_element> b_elements;  // Vbo layout.
                                
                                initial_command.associated_shader = &shader_linked;
                                
                                b_elements.emplace_back(Buffer_data_type::Vector_2f, "Position", true);
                                b_elements.emplace_back(Buffer_data_type::Color_4f, "Color", true);
                                b_elements.emplace_back(Buffer_data_type::Vector_2f, "Texture coords", true);
                                
                                Buffer_layout vbo_layout(b_elements);
                                auto vbo_ = Vertex_buffer::create(sizeof(float) * 4, max_vbo_buffer_size);
                                vbo_->set_layout(vbo_layout);
                                
                                initial_command.vertex_array_buffer = Vertex_array_buffer::create();
                                initial_command.vertex_array_buffer->add_vertex_buffer(vbo_);
                                
                                // Make sure the buffer is set to 0 in count to prevent fetching the max count with 'get_count()'.
                                initial_command.vertex_array_buffer->get_vertex_buffers()[0]->set_count(0);
                                
                                Gl_renderer::draw_commands[shader_linked.get_id()] = new Renderer::Draw_command(
                                  initial_command);
                              })
  }
  
  void Gl_renderer::init_object_draw_command(Shader &shader_linked)
  {
    WAVE_PROFILE_INSTRUCTIONS("Pre-rendering objects", YELLOW, 4,
                              {
                                Gl_renderer::uniform_buffers.emplace_back(
                                  Uniform_buffer::create(Buffer_data_type_size(Buffer_data_type::Matrix_4f) * 2,
                                                         3));
                                
                                // Add draw command for batch rendering.
                                std::vector<Buffer_element> b_elements;  // Vbo layout.
                                
                                s_object_draw_commands.emplace_back(new Renderer::Draw_command());
                                s_object_draw_commands.back()->associated_shader = &shader_linked;
                                s_object_draw_commands.back()->batch_offset.emplace_back(0, 0, 0);
                                
                                // Set default shader layout for an 3D object.
                                b_elements.emplace_back(Buffer_data_type::Vector_3f, "Position", true);
                                b_elements.emplace_back(Buffer_data_type::Vector_3f, "Normal", true);
                                b_elements.emplace_back(Buffer_data_type::Color_4f, "Color", true);
                                b_elements.emplace_back(Buffer_data_type::Vector_2f, "Texture coords", true);
                                b_elements.emplace_back(Buffer_data_type::Matrix_4f, "Model Matrix", false);
                                
                                s_object_draw_commands.back()->vertex_array_buffer = Vertex_array_buffer::create();
                                s_object_draw_commands.back()->vertex_array_buffer->set_index_buffer(
                                  Index_buffer::create(nullptr, max_ibo_buffer_size));
                                
                                // Make sure the buffer is set to 0 in count to prevent fetching the max count with 'get_count()'.
                                s_object_draw_commands.back()->vertex_array_buffer->get_index_buffer()->set_count(0);
                                
                                auto vbo_ = Vertex_buffer::create(sizeof(Vertex_3D), max_vbo_buffer_size);
                                Buffer_layout vbo_layout(b_elements);
                                vbo_->set_layout(vbo_layout);
                                s_object_draw_commands.back()->vertex_array_buffer->add_vertex_buffer(vbo_);
                                
                                // Make sure the buffer is set to 0 in count to prevent fetching the max count with 'get_count()'.
                                s_object_draw_commands.back()->vertex_array_buffer->get_vertex_buffers()[0]->set_count(
                                  0);
                                Gl_renderer::draw_commands[shader_linked.get_id()] = s_object_draw_commands.back();
                              }
    
    )
  }
  
  void Gl_renderer::gl_clear_errors()
  {
    while (glGetError());  // Clear all previous openGL errors before polling the next one.
  }
  
  void Gl_renderer::clear_bg()
  {
    // Clear and change the back buffer color bit with our color.
    CHECK_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  }
  
  void Gl_renderer::set_clear_color(const Color &color)
  {
    CHECK_GL_CALL(glClearColor(color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()));
  }
  
  void Gl_renderer::show_renderer_info()
  {
    // Get GLFW and OpenGL version.
    int32_t num_ext;
    CHECK_GL_CALL(glGetIntegerv(GL_NUM_EXTENSIONS, &num_ext));
    Wave::alert(WAVE_LOG_INFO, "[GL renderer] --> Api (Glew) version : %d.%d.%d",
                GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);
    Wave::alert(WAVE_LOG_INFO, "[GL renderer] --> Graphics renderer : %s", glGetString(GL_RENDERER));
    Wave::alert(WAVE_LOG_INFO, "[GL renderer] --> Extensions available : %d", num_ext);
    Wave::alert(WAVE_LOG_INFO, "[GL renderer] --> OpenGL Vendor : %s", glGetString(GL_VENDOR));
    Wave::alert(WAVE_LOG_INFO, "[GL renderer] --> OpenGL version : %s", get_api_version());
    Wave::alert(WAVE_LOG_INFO, "[GL renderer] --> Shading language version : %s", get_api_shader_version());
  }
  
  void Gl_renderer::begin(std::shared_ptr<Camera> &camera)
  {
    // Reset stats.
    Gl_renderer::stats.shaders_count = 0;
    Gl_renderer::stats.textures_drawn_count = 0;
    Gl_renderer::stats.vao_count = 0;
    Gl_renderer::stats.vertices_drawn_count = 0;
    Gl_renderer::stats.indices_drawn_count = 0;
    Gl_renderer::stats.draw_call_count = 0;
    
    Gl_renderer::scene_camera = camera.get();
    
    // View and projection for objects.
    Gl_renderer::uniform_buffers[0]->set_data(0, Buffer_data_type_size(Buffer_data_type::Matrix_4f),
                                              &(Gl_renderer::scene_camera->get_view_matrix().get_matrix()[0][0]));
    Gl_renderer::uniform_buffers[0]->set_data(Buffer_data_type_size(Buffer_data_type::Matrix_4f),
                                              Buffer_data_type_size(Buffer_data_type::Matrix_4f),
                                              &(Gl_renderer::scene_camera->get_projection_matrix().get_matrix()[0][0]));
  }
  
  void Gl_renderer::on_event(Event &event)
  {
    Event_dispatcher renderer_dispatcher(event);
    switch (event.get_event_type())
    {
      case Event_type::On_window_resize:
      {
        auto window_resize = dynamic_cast<On_window_resize &>(event);
        Gl_renderer::set_viewport(window_resize.get_width(), window_resize.get_height());
        return;
      }
      case Event_type::On_renderer_error:
      {
        renderer_dispatcher.dispatch<On_renderer_error>(Gl_renderer::renderer_error_callback);
        return;
      }
      default:return;
    }
  }
  
  void Gl_renderer::load_dynamic_vbo_data(const void *vertices, uint64_t count, uint64_t size, uint64_t command_index,
                                          int64_t vbo_offset)
  {
    if (!vertices || size == 0 || count == 0) return;
    if (Gl_renderer::draw_commands.contains(command_index))
    {
      if (vbo_offset == 0)
      {
        // Reset buffer contents.
        Gl_renderer::draw_commands[command_index]->global_vbo_offset = vbo_offset;
        Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_vertex_buffers()[0]->set_count(0);
      }
      Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_vertex_buffers()[0]->set_data(
        vertices,
        count * size,
        vbo_offset == WAVE_VALUE_DONT_CARE ? Gl_renderer::draw_commands[command_index]->global_vbo_offset :
        vbo_offset);
    }
    
    // Update vbo count.
    Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_vertex_buffers()[0]->set_count(
      Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_vertex_buffers()[0]->get_count() +
      count);
    Gl_renderer::draw_commands[command_index]->global_vbo_offset =
      vbo_offset == WAVE_VALUE_DONT_CARE ?
      Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_vertex_buffers()[0]->get_count() *
      size :
      (uint64_t) Gl_renderer::draw_commands[command_index]->global_vbo_offset + vbo_offset + (count * size);
  }
  
  void Gl_renderer::load_dynamic_ibo_data(const void *faces, uint64_t count, uint64_t size, uint64_t command_index,
                                          int64_t ibo_offset)
  {
    if (!faces || size == 0 || count == 0) return;
    
    if (ibo_offset == WAVE_VALUE_DONT_CARE &&
        Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_index_buffer()->get_count() != 0)
      Gl_renderer::draw_commands[command_index]->batch_offset.back().ibo_offset =
        Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_index_buffer()->get_count() *
        sizeof(uint32_t);
    else if (ibo_offset != WAVE_VALUE_DONT_CARE)
    {
      Gl_renderer::draw_commands[command_index]->batch_offset.back().ibo_offset +=
        (uint64_t) ibo_offset + (count * size);
    }
    
    if (Gl_renderer::draw_commands.contains(command_index))
    {
      if (ibo_offset == 0)
      {
        // Reset buffer contents.
        Gl_renderer::draw_commands[command_index]->batch_offset.back().ibo_offset = ibo_offset;
        Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_index_buffer()->set_count(0);
      }
      Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_index_buffer()->set_data(
        faces,
        count * size,
        ibo_offset == WAVE_VALUE_DONT_CARE ?
        Gl_renderer::draw_commands[command_index]->batch_offset.back().ibo_offset : ibo_offset);
      
      // Update ibo count.
      Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_index_buffer()->set_count(
        Gl_renderer::draw_commands[command_index]->vertex_array_buffer->get_index_buffer()->get_count() + count);
    }
  }
  
  void Gl_renderer::send_object(Shader &shader, const std::vector<std::shared_ptr<Texture>> &textures_,
                                const void *vertices, uint64_t vertex_count, uint64_t vertex_size,
                                const void *indices, uint64_t index_count, int64_t vbo_offset, int64_t ibo_offset)
  {
    shader.bind();
    int32_t object_shader_id = shader.get_id();
    
    if (Gl_renderer::draw_commands.contains(object_shader_id))
    {
      Gl_renderer::draw_commands[object_shader_id]->batch_offset.emplace_back(
        Gl_renderer::draw_commands[object_shader_id]->vertex_array_buffer->get_index_buffer()->get_count(),
        Gl_renderer::draw_commands[object_shader_id]->vertex_array_buffer->get_index_buffer()->get_count() *
        sizeof(uint32_t),
        Gl_renderer::draw_commands[object_shader_id]->vertex_array_buffer->get_vertex_buffers()[0]->get_count());
    } else if (!Gl_renderer::draw_commands.contains(object_shader_id))
    {
      init_object_draw_command(shader);
    }
    
    Gl_renderer::draw_commands[object_shader_id]->batch_offset.back() = {
      .vertex_count = vertex_count,
      .vbo_offset = Gl_renderer::draw_commands[object_shader_id]->vertex_array_buffer->get_vertex_buffers()[0]->get_count()};
    
    Gl_renderer::draw_commands[object_shader_id]->associated_shader->bind();
    Gl_renderer::draw_commands[object_shader_id]->associated_shader->set_uniform("u_sampler",
                                                                                 s_texture_unit_number_assigned);
    
    unsigned int u_camera_block = glGetUniformBlockIndex(
      Gl_renderer::draw_commands[object_shader_id]->associated_shader->get_id(), "u_camera");
    CHECK_GL_CALL(glUniformBlockBinding(Gl_renderer::draw_commands[object_shader_id]->associated_shader->get_id(),
                                        u_camera_block,
                                        3));
    
    
    // If we reached the max batch memory limit (2 MB).
    if ((Gl_renderer::draw_commands[object_shader_id]->vertex_array_buffer->get_vertex_buffers()[0]->get_count() +
         index_count) * sizeof(uint32_t) >= max_vbo_buffer_size)
      flush();
    
    // Set data in vbo and ibo.
    load_dynamic_vbo_data(vertices, vertex_count, vertex_size,
                          object_shader_id, vbo_offset);
    
    load_dynamic_ibo_data(indices, index_count, sizeof(uint32_t),
                          object_shader_id, ibo_offset);
    
    Gl_renderer::draw_commands[object_shader_id]->batch_offset.back().index_count = index_count;
    
    if (!textures_.empty())
      Gl_renderer::draw_commands[object_shader_id]->associated_shader->set_uniform("u_has_texture", true);
    
    // Link object texture objects to texture units for rendering.
    for (auto &texture: textures_)
    {
      texture->set_texture_slot(s_texture_unit_number_assigned);
      if (!texture->is_sent()) texture->send_gpu(1);
      
      // If this is a new object texture.
      if (!Gl_renderer::textures.contains(texture->get_id())) s_texture_unit_number_assigned++;
      Gl_renderer::textures[texture->get_id()] = texture.get();
    }
    Gl_renderer::stats.object_count++;
  }
  
  void Gl_renderer::send_text(Shader &shader, Texture &texture_atlas, const void *vertices, uint64_t vertex_count,
                              uint64_t vertex_size, [[maybe_unused]] const void *indices,
                              [[maybe_unused]] uint64_t index_count, int64_t vbo_offset,
                              [[maybe_unused]] int64_t ibo_offset)
  {
    shader.bind();
    int32_t shader_id = shader.get_id();
    
    // Setup new buffer.
    if (!Gl_renderer::draw_commands.contains(shader_id))
    {
      Gl_renderer::stats.text_glyph_count += vertex_count / 6;
      init_text_draw_command(shader);
    }
    
    if (vbo_offset != 0)
    {
      Gl_renderer::draw_commands[shader_id]->batch_offset.emplace_back(
        vertex_count,
        0, 0,
        Gl_renderer::draw_commands[shader_id]->vertex_array_buffer->get_vertex_buffers()[0]->get_count());
    }
    
    Gl_renderer::draw_commands[shader_id]->associated_shader->bind();
    Gl_renderer::draw_commands[shader_id]->associated_shader->set_uniform("u_sampler", s_texture_unit_number_assigned);
    
    // Update content of VBO memory.
    load_dynamic_vbo_data(vertices, vertex_count, vertex_size, shader_id, vbo_offset);
    
    // Set texture unit for draw command.
    texture_atlas.set_texture_slot(s_texture_unit_number_assigned);
    
    // If this is a new texture.
    if (!Gl_renderer::textures.contains(texture_atlas.get_id())) s_texture_unit_number_assigned++;
    
    Gl_renderer::textures[texture_atlas.get_id()] = &texture_atlas;  // Load glyph texture atlas.
  }
  
  void Gl_renderer::flush()
  {
    for (const auto &texture: Gl_renderer::textures)
      if (texture.second) texture.second->bind(texture.second->get_texture_slot());
    Gl_renderer::stats.textures_drawn_count = Gl_renderer::textures.size();
    
    for (auto &draw_command: Gl_renderer::draw_commands)
    {
      draw_command.second->associated_shader->bind();
      Gl_renderer::stats.shaders_count++;
      draw_command.second->vertex_array_buffer->bind();
      
      if (draw_command.second->vertex_array_buffer->get_index_buffer())
        draw_command.second->vertex_array_buffer->get_index_buffer()->bind();
      for (const auto &offset: draw_command.second->batch_offset)
      {
        if (draw_command.second->instance_count == 1)
        {
          if (offset.vbo_offset == 0 && offset.index_count > 0)
          {
            CHECK_GL_CALL(glDrawElements(GL_TRIANGLES, offset.index_count,
                                         GL_UNSIGNED_INT,
                                         nullptr));
          } else if (offset.index_count > 0)
          {
            CHECK_GL_CALL(glDrawElementsBaseVertex(GL_TRIANGLES, offset.index_count,
                                                   GL_UNSIGNED_INT,
                                                   (const void *) offset.ibo_offset,
                                                   offset.vbo_offset));
          } else if (offset.index_count == 0)
          {
            CHECK_GL_CALL(glDisable(GL_CULL_FACE));
            CHECK_GL_CALL(glDrawArrays(GL_TRIANGLES, offset.vbo_offset,
                                       offset.vertex_count));
            CHECK_GL_CALL(glEnable(GL_CULL_FACE));
          }
        } else if (draw_command.second->instance_count > 1)
        {
          if (offset.index_count == 0)
          {
            CHECK_GL_CALL(glDisable(GL_CULL_FACE));
            CHECK_GL_CALL(glDrawArraysInstanced(GL_TRIANGLES, 0,
                                                offset.vertex_count,
                                                draw_command.second->instance_count));
            CHECK_GL_CALL(glEnable(GL_CULL_FACE));
          } else
          {
            CHECK_GL_CALL(glDrawElementsInstancedBaseVertex(GL_TRIANGLES, offset.index_count,
                                                            GL_UNSIGNED_INT,
                                                            nullptr,
                                                            draw_command.second->instance_count,
                                                            offset.vbo_offset));
          }
        }
        Gl_renderer::stats.vertices_drawn_count += offset.vertex_count;
        Gl_renderer::stats.indices_drawn_count += offset.index_count;
        Gl_renderer::stats.draw_call_count++;
      }
      Gl_renderer::stats.vao_count++;
      draw_command.second->global_vbo_offset = 0;
    }
  }
  
  void Gl_renderer::end()
  {
    Gl_renderer::flush();
  }
  
  void Gl_renderer::shutdown()
  {
    Gl_renderer::set_state({nullptr, nullptr, nullptr, WAVE_RENDERER_SHUTDOWN});
    
    for (const auto &draw_command: Gl_renderer::draw_commands) delete draw_command.second;
    
    free(s_glsl_version);
  }
  
  bool Gl_renderer::has_crashed()
  {
    return Gl_renderer::state.code != WAVE_RENDERER_RUNNING &&
           Gl_renderer::state.code != WAVE_RENDERER_INIT &&
           Gl_renderer::state.code != WAVE_RENDERER_SHUTDOWN;
  }
  
  bool Gl_renderer::renderer_error_callback(On_renderer_error &renderer_error)
  {
    if (renderer_error.get_renderer_state().severity[0] == 'W')  // Warning
    {
      renderer_error.print(Print_type::Warn);
    } else if (renderer_error.get_renderer_state().severity[0] == 'F')  // Fatal error
    {
      renderer_error.print(Print_type::Error);
      Gl_renderer::set_state({nullptr, nullptr, nullptr,
                              renderer_error.get_renderer_state().code});
    }
    return true;
  }
  
  void
  Gl_renderer::gl_synchronous_error_callback(GLenum error_code, const char *error_message, const char *function_name,
                                             const char *file_name, size_t line_number)
  {
    if (!Gl_renderer::is_running()) return;
    Renderer_state temp{};
    char _source[FILENAME_MAX * 4];
    char output[FILENAME_MAX * 4];
    int snprintf_result;
    if (error_code != GL_NO_ERROR)
    {
      switch (error_code)
      {
        case GL_DEBUG_SOURCE_API:
        {
          temp.type = "Api error";
          temp.severity = "Warning";
          temp.code = GL_DEBUG_SOURCE_API;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Api error");
          break;
        }
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        {
          temp.type = "Window creation error";
          temp.severity = "Fatal";
          temp.code = GL_DEBUG_SOURCE_WINDOW_SYSTEM;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Window creation error");
          break;
        }
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
        {
          temp.type = "Shader compilation error";
          temp.severity = "Fatal";
          temp.code = GL_DEBUG_SOURCE_SHADER_COMPILER;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Shader compilation error");
          break;
        }
        case GL_DEBUG_SOURCE_THIRD_PARTY:
        {
          temp.type = "Third party error";
          temp.severity = "Warning";
          temp.code = GL_DEBUG_SOURCE_THIRD_PARTY;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Third party error");
          break;
        }
        case GL_DEBUG_SOURCE_APPLICATION:
        {
          temp.type = "App error";
          temp.severity = "Fatal";
          temp.code = GL_DEBUG_SOURCE_APPLICATION;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "App error");
          break;
        }
        case GL_INVALID_OPERATION:
        {
          temp.type = "Invalid operation";
          temp.severity = "Warning";
          temp.code = GL_INVALID_OPERATION;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Invalid operation");
          break;
        }
        case GL_INVALID_ENUM:
        {
          temp.type = "Invalid enum";
          temp.severity = "Warning";
          temp.code = GL_INVALID_ENUM;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Invalid enum");
          break;
        }
        case WAVE_GL_INVALID_VIEWPORT_DIMENSIONS:
        {
          temp.type = "Invalid viewport dimensions";
          temp.severity = "Fatal";
          temp.code = GL_INVALID_ENUM;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Invalid viewport dimensions");
          break;
        }
        case GL_INVALID_VALUE:
        {
          temp.type = "Invalid value";
          temp.severity = "Warning";
          temp.code = GL_INVALID_VALUE;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Invalid value");
          break;
        }
        case WAVE_GL_BUFFER_NOT_LOADED:
        {
          temp.type = "Buffer not built";
          temp.severity = "Fatal";
          temp.code = WAVE_GL_BUFFER_NOT_LOADED;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Buffer not built");
          break;
        }
        case WAVE_GL_DEBUG_SOURCE_INVALID_UNIFORM:
        {
          temp.type = "Invalid uniform";
          temp.severity = "Warning";
          temp.code = WAVE_GL_DEBUG_SOURCE_INVALID_UNIFORM;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Invalid uniform");
          break;
        }
        case GL_INVALID_FRAMEBUFFER_OPERATION:
        {
          temp.type = "Invalid framebuffer operation";
          temp.severity = "Fatal";
          temp.code = GL_INVALID_FRAMEBUFFER_OPERATION;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Invalid framebuffer operation");
          break;
        }
        case GL_FRAMEBUFFER_UNDEFINED:
        {
          temp.type = "Invalid framebuffer (undefined)";
          temp.severity = "Fatal";
          temp.code = GL_FRAMEBUFFER_UNDEFINED;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Invalid framebuffer (undefined)");
          break;
        }
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        {
          temp.type = "Incomplete framebuffer attachment";
          temp.severity = "Fatal";
          temp.code = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Incomplete framebuffer attachment");
          break;
        }
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        {
          temp.type = "Missing framebuffer attachment";
          temp.severity = "Fatal";
          temp.code = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Missing framebuffer color or depth attachment");
          break;
        }
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        {
          temp.type = "Framebuffer texture 2D incomplete multisample";
          temp.severity = "Fatal";
          temp.code = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Framebuffer texture 2D incomplete multisample");
          break;
        }
        case GL_OUT_OF_MEMORY:
        {
          temp.type = "Out of memory";
          temp.severity = "Fatal";
          temp.code = GL_OUT_OF_MEMORY;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Out of memory");
          break;
        }
        default:temp.type = "Other error";
          temp.severity = "Fatal";
          temp.code = GL_DEBUG_SOURCE_OTHER;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "Other error");
          break;
      }
      
      temp.description = error_message;
      
      if (snprintf_result < 0)
      {
        Wave::alert(WAVE_LOG_ERROR, "%s[SNPRINTF]%s IN FUNCTION %s IN CASE %d",
                    RED, DEFAULT, __FUNCTION__, error_code);
        temp.description = std::string("Unknown error").c_str();
      } else if (snprintf(output, sizeof(output),
                          "OpenGL call returned an error :\n%52sIn function --> %s,\n%52sIn file --> %s,\n"
                          "%52sAt line --> %zu,\n%52sDetails --> %s%s",
                          DEFAULT, function_name, DEFAULT, file_name, DEFAULT, line_number,
                          DEFAULT, error_message, DEFAULT) < 0)
      {
        temp.description = std::string(
          "Error while building string with snprintf() on line 531 in file renderer.cpp!").c_str();
      } else
      {
        temp.description = output;
        On_renderer_error gl_error(temp, Renderer_api::OpenGL);
        Gl_renderer::renderer_error_callback(gl_error);
      }
    }
  }

#ifdef OPENGL_VERSION_4_3_PLUS
  
  void Gl_renderer::gl_asynchronous_error_callback(GLenum error_code, GLenum type, [[maybe_unused]] GLuint id,
                                                   GLenum severity, [[maybe_unused]] GLsizei length,
                                                   const GLchar *error_message,
                                                   [[maybe_unused]] const void *userParam)
  {
    if (!Gl_renderer::is_running()) return;
    
    Renderer_state temp{};
    char output[LINE_MAX];
    if (error_code != GL_NO_ERROR)
    {
      
      switch (type)
      {
        case GL_DEBUG_TYPE_ERROR: temp.type = "Error";
          break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: temp.type = "Deprecated Behaviour";
          break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: temp.type = "Undefined Behaviour";
          break;
        case GL_DEBUG_TYPE_PORTABILITY: temp.type = "Portability";
          break;
        case GL_DEBUG_TYPE_PERFORMANCE: temp.type = "Performance";
          break;
        case GL_DEBUG_TYPE_MARKER: temp.type = "Marker";
          break;
        case GL_DEBUG_TYPE_PUSH_GROUP: temp.type = "Push Group";
          break;
        case GL_DEBUG_TYPE_POP_GROUP: temp.type = "Pop Group";
          break;
        case GL_DEBUG_TYPE_OTHER: temp.type = "Other";
          break;
        default: temp.type = "Unknown";
          break;
      }
      //TODO Replace return statements for both low and notification severity levels for thorough debugging.
      switch (severity)
      {
        case GL_DEBUG_SEVERITY_HIGH: temp.severity = "Fatal (High)";
          break;
        case GL_DEBUG_SEVERITY_MEDIUM: temp.severity = "Fatal (Medium)";
          break;
        case GL_DEBUG_SEVERITY_LOW: temp.severity = "Warn (low)";
          break;  // Silence low warnings for now due to clutter in terminal when this is logged.
        case GL_DEBUG_SEVERITY_NOTIFICATION: temp.severity = "Warn (info)";
          return;  // Silence info for now due to clutter in terminal when this is logged.
        default: temp.severity = "Warn (Unknown)";
          break;
      }
      
      switch (error_code)
      {
        case GL_DEBUG_SOURCE_API:
        {
          temp.code = GL_DEBUG_SOURCE_API;
          break;
        }
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        {
          temp.code = GL_DEBUG_SOURCE_WINDOW_SYSTEM;
          break;
        }
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
        {
          temp.code = GL_DEBUG_SOURCE_SHADER_COMPILER;
          break;
        }
        case GL_DEBUG_SOURCE_THIRD_PARTY:
        {
          temp.code = GL_DEBUG_SOURCE_THIRD_PARTY;
          break;
        }
        case GL_DEBUG_SOURCE_APPLICATION:
        {
          temp.code = GL_DEBUG_SOURCE_APPLICATION;
          break;
        }
        case GL_INVALID_OPERATION:
        {
          temp.code = GL_INVALID_OPERATION;
          break;
        }
        case GL_INVALID_ENUM:
        {
          temp.code = GL_INVALID_ENUM;
          break;
        }
        case GL_INVALID_VALUE:
        {
          temp.code = GL_INVALID_VALUE;
          break;
        }
        case WAVE_GL_DEBUG_SOURCE_INVALID_UNIFORM:
        {
          temp.code = WAVE_GL_DEBUG_SOURCE_INVALID_UNIFORM;
          break;
        }
        case GL_INVALID_FRAMEBUFFER_OPERATION:
        {
          temp.code = GL_INVALID_FRAMEBUFFER_OPERATION;
          break;
        }
        case GL_FRAMEBUFFER_UNDEFINED:
        {
          temp.code = GL_FRAMEBUFFER_UNDEFINED;
          break;
        }
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        {
          temp.code = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
          break;
        }
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        {
          temp.code = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT;
          break;
        }
        case GL_OUT_OF_MEMORY:
        {
          temp.code = GL_OUT_OF_MEMORY;
          break;
        }
        default:
        {
          temp.code = GL_DEBUG_SOURCE_OTHER;
          break;
        }
      }
      
      temp.description = error_message;
      
      if (snprintf(output,
                   sizeof(output),
                   "%s%s",
                   error_message, DEFAULT) < 0)
      {
        temp.description = std::string(
          "Error while building string with snprintf() on line 531 in file renderer.cpp!").c_str();
      } else
      {
        temp.description = output;
        On_renderer_error gl_error(temp, Renderer_api::OpenGL);
        Gl_renderer::renderer_error_callback(gl_error);
      }
    }
  }

#endif
}