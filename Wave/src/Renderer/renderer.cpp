//
// Created by nami on 9/5/22.
//

#include <Renderer/renderer.h>

namespace Wave
{
  bool Gl_renderer::running = false;
  Renderer_api Gl_renderer::api = Renderer_api::None;
  Renderer_state Gl_renderer::state = {nullptr,
                                       nullptr,
                                       nullptr,
                                       Renderer_error_type::NO_ERROR};
  
  std::shared_ptr<Vertex_array_buffer> Gl_renderer::vertex_array_buffers;
  std::vector<Texture> Gl_renderer::textures;
  std::function<void(Event &event)> Gl_renderer::event_callback_function;
  
  /****************************** RENDERER 3D **************************************/
  
  bool Gl_renderer::is_running()
  {
    return Gl_renderer::running;
  }
  
  Renderer_state Gl_renderer::get_state()
  {
    return Gl_renderer::state;
  }
  
  Renderer_api Gl_renderer::get_api()
  {
    return Renderer_api::Opengl;
  }
  
  const std::function<void(Event &event)> &Gl_renderer::get_event_callback_function()
  {
    return Gl_renderer::event_callback_function;
  }
  
  void Gl_renderer::on_event(Event &event)
  {
    Event_dispatcher renderer_dispatcher(event);
    switch (event.get_event_type())
    {
      case Event_type::On_renderer_error:
        renderer_dispatcher.dispatch<On_renderer_error>(BIND_EVENT_FUNCTION(renderer_error_callback));
        return;
      default:return;
    }
  }
  
  bool Gl_renderer::renderer_error_callback([[maybe_unused]] On_renderer_error &renderer_error)
  {
    // Reset state to prevent subsequent renderer events to propagate errors.
    Gl_renderer::set_state({nullptr,
                            nullptr,
                            nullptr,
                            Renderer_error_type::NO_ERROR});
    
    if (strcmp(renderer_error.get_renderer_state().severity, "Warning") == 0)
    {
      renderer_error.print(Print_type::Warn);
    }
    else if (strcmp(renderer_error.get_renderer_state().severity, "Fatal") == 0)
    {
      renderer_error.print(Print_type::Error);
      Gl_renderer::shutdown();
    }
    return true;
  }
  
  void Gl_renderer::gl_clear_errors()
  {
    while (glGetError());  // Clear all previous openGL errors before polling the next one.
  }
  
  void Gl_renderer::clear_bg()
  {
    // Clear and change the back buffer color bit with our color.
    gl_call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  }
  
  void Gl_renderer::set_clear_color(const Color &color)
  {
    gl_call(glClearColor(color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()));
  }
  
  void Gl_renderer::enable_textures(bool enabled)
  {
    if (enabled) { glEnable(GL_TEXTURE_2D); }
    else { glDisable(GL_TEXTURE_2D); }
  }
  
  void Gl_renderer::init()
  {
    Gl_renderer::running = true;
    log_task("RENDERER 3D", CYAN, 1, "Loading 3D renderer ...",
             {
               log_instruction("RENDERER 3D", DEFAULT, "Loading GLEW", gl_call(glewInit()))
               Gl_renderer::set_state(Gl_renderer::get_state());
               
               // Enable openGL error handling.
               gl_call(glEnable(
                   GL_DEBUG_OUTPUT_SYNCHRONOUS));  // Calls to callback will be synchronous for debug breakpoints.
               gl_call(glEnable(GL_DEBUG_OUTPUT));  // Enable debug output.
               log_instruction("RENDERER 3D|GLEW", DEFAULT, "Enabling textures",
                               enable_textures(true))  // Enable textures.
               
               Color background_color;  // Default dark-mode-like color for background.
               gl_call(glClearColor(background_color.get_red(), background_color.get_green(),
                                    background_color.get_blue(), background_color.get_alpha()));
               
               gl_call(glFrontFace(GL_CW)); // Every shape drawn in clock-wise manner will be considered the FRONT face.
               gl_call(glCullFace(GL_FRONT_AND_BACK)); // The back side of shapes will NOT be drawn.
               gl_call(glDisable(GL_CULL_FACE)); // Don't update the back face of shapes since the camera won't see it.
               
               // Let OpenGL keep track of depth for shapes and auto determine if some shapes closer or further away from
               // the camera should take priority (drawn on top of other ones).
               gl_call(glEnable(GL_DEPTH_TEST));
               gl_call(glEnable(GL_MULTISAMPLE));
               gl_call(glEnable(GL_BLEND));
               gl_call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
               
               // Let OpenGL do the exponential gamma correction for us so textures and colors don't appear as dark.
//               gl_call(glEnable(GL_FRAMEBUFFER_SRGB));
             }, "Renderer 3D loaded")
    
    log_task("RENDERER 3D", YELLOW, 1, "Fetching renderer info on client system ...",
             Gl_renderer::show_renderer_info(),
             "Renderer info fetched")
  }
  
  void Gl_renderer::on_window_resize(Window *window, float width, float height)
  {
    if (!window) return;
    Gl_renderer::set_viewport(width, height);
  }
  
  const char *Gl_renderer::get_gl_version()
  {
    return (const char *) (glGetString(GL_VERSION));
  }
  
  void Gl_renderer::show_renderer_info()
  {
    // Get GLFW and OpenGL version.
    int32_t num_ext;
    gl_call(glGetIntegerv(GL_NUM_EXTENSIONS, &num_ext));
    Wave::alert(WAVE_INFO, "[RENDERER] --> Api (Glew) version : %d.%d.%d", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR,
                GLEW_VERSION_MICRO);
    Wave::alert(WAVE_INFO, "[RENDERER] --> Graphics renderer : %s", glGetString(GL_RENDERER));
    Wave::alert(WAVE_INFO, "[RENDERER] --> Extensions available : %d", num_ext);
    Wave::alert(WAVE_INFO, "[RENDERER] --> Opengl Vendor : %s", glGetString(GL_VENDOR));
    Wave::alert(WAVE_INFO, "[RENDERER] --> Opengl version : %s", glGetString(GL_VERSION));
    Wave::alert(WAVE_INFO, "[RENDERER] --> Shading language version : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
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
    gl_call(glViewport(0, 0, width, height));
  }
  
  void Gl_renderer::load_object(const Object_3D *object)
  {
    std::vector<Buffer_element> b_elements;
    b_elements.emplace_back(Buffer_data_type::Vector_3f, "Position", true);
    b_elements.emplace_back(Buffer_data_type::Vector_3f, "Normal", true);
    b_elements.emplace_back(Buffer_data_type::Color_4f, "Color", true);
    b_elements.emplace_back(Buffer_data_type::Vector_2f, "Texture coords", true);
    
    auto vbo_ = Vertex_buffer::create(object->get_vertices(),
                                      Object_3D::get_vertex_size() *
                                      object->get_vertex_count(),
                                      STATIC_DRAW);
    if (!Gl_renderer::vertex_array_buffers) Gl_renderer::vertex_array_buffers = Vertex_array_buffer::create();
    Gl_renderer::vertex_array_buffers->set_index_buffer(
        Index_buffer::create(object->get_faces().data(),
                                               static_cast<uint32_t>(object->get_faces().size())));
    Buffer_layout vbo_layout(b_elements);
    vbo_->set_layout(vbo_layout);
    Gl_renderer::vertex_array_buffers->add_vertex_buffer(vbo_);
    
    if (!object->get_textures().empty() && object->get_textures()[0].get_id() != 255)
    {
      Gl_renderer::textures.emplace_back(object->get_textures()[0]);
    }
    Gl_renderer::vertex_array_buffers->unbind();
  }
  
  std::shared_ptr<Vertex_array_buffer> Gl_renderer::load_text()
  {
    std::vector<Buffer_element> b_elements;
    b_elements.emplace_back(Buffer_data_type::Vector_2f, "Position", true);
    b_elements.emplace_back(Buffer_data_type::Vector_2f, "Texture coords", true);
    
    Buffer_layout vbo_layout(b_elements);
    auto vbo_ = Vertex_buffer::create(sizeof(float) * 6 * 4);
    vbo_->set_layout(vbo_layout);
    
    auto vao = Vertex_array_buffer::create();
    vao->add_vertex_buffer(vbo_);
    return vao;
  }
  
  void Gl_renderer::load_dynamic_data(const void *vertices, size_t size, uint64_t vbo_index)
  {
    if (!vertices || size == 0) return;
    Gl_renderer::vertex_array_buffers->get_vertex_buffers()[vbo_index]->set_data(vertices, size, 0);
  }
  
  void Gl_renderer::draw_object(const Object_3D *object)
  {
    load_object(object);
    draw_loaded_objects(1);
  }
  
  void Gl_renderer::draw_objects(const std::vector<Object_3D> *objects)
  {
    for (const Object_3D &object: *objects) draw_object(&object);
    
  }
  
  void Gl_renderer::draw_loaded_objects(uint32_t object_count)
  {
    for (size_t i = 0; i < object_count; i++)
    {
      draw_object();
    }
  }
  
  void Gl_renderer::draw_object()
  {
    Gl_renderer::vertex_array_buffers->bind();
    if (!Gl_renderer::textures.empty()) Gl_renderer::textures.back().bind(0);
    gl_call(glDrawElements(GL_TRIANGLES, Gl_renderer::vertex_array_buffers->get_index_buffer()->get_count(),
                           GL_UNSIGNED_INT, nullptr));
    Gl_renderer::vertex_array_buffers->unbind();
    if (!Gl_renderer::textures.empty()) Gl_renderer::textures[0].unbind();
  }
  
  void Gl_renderer::draw_text(const std::shared_ptr<Text> &text, const std::shared_ptr<Vertex_array_buffer> &vao)
  {
    gl_call(glActiveTexture(GL_TEXTURE1));
    gl_call(glBindVertexArray(vao->get_id()));
    
    float x = text->get_offset_x(), y = text->get_offset_y(), scale = text->get_scale();
    
    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text->get_string().begin(); c != text->get_string().end(); c++)
    {
      Glyph ch = (*text)[*c];
      
      float x_pos = x + ch.bearing.get_x() * scale;
      float y_pos = y - (ch.size.get_y() - ch.bearing.get_y()) * scale;
      float w = ch.size.get_x() * scale;
      float h = ch.size.get_y() * scale;
      // Update VBO for each character
      float vertices[6][4] = {
          {x_pos,     y_pos + h, 0.0f, 0.0f},
          {x_pos,     y_pos,     0.0f, 1.0f},
          {x_pos + w, y_pos,     1.0f, 1.0f},
          
          {x_pos,     y_pos + h, 0.0f, 0.0f},
          {x_pos + w, y_pos,     1.0f, 1.0f},
          {x_pos + w, y_pos + h, 1.0f, 0.0f}
      };
      // Render glyph texture over quad
      gl_call(glBindTexture(GL_TEXTURE_2D, ch.texture_id));
      
      // Update content of VBO memory
      gl_call(glBindBuffer(GL_ARRAY_BUFFER, vao->get_vertex_buffers().back()->get_id()));
      
      // Use glBufferSubData and not glBufferData
      gl_call(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
      gl_call(glBindBuffer(GL_ARRAY_BUFFER, 0));
      
      // Draw
      gl_call(glDrawArrays(GL_TRIANGLES, 0, 6));
      // Advance cursors for next glyph (note that advance is number of 1/64 pixels)
      x += static_cast<float>((ch.advance >> 6)) *
           scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    gl_call(glBindVertexArray(0));
    gl_call(glBindTexture(GL_TEXTURE_2D, 0));
  }
  
  void Gl_renderer::delete_gl_buffers()
  {
    if (Gl_renderer::is_running())
    {
      for (const auto &texture: Gl_renderer::textures) texture.remove();
      if (Gl_renderer::vertex_array_buffers) Gl_renderer::vertex_array_buffers->remove();
    }
  }
  
  void Gl_renderer::gl_error_callback(GLenum error_code, const char *error_message, const char *function_name,
                                      const char *file_name, size_t line_number)
  {
    if (!Gl_renderer::is_running()) return;
    char _source[FILENAME_MAX * 4];
    char output[FILENAME_MAX * 4];
    int snprintf_result;
    if (error_code != GL_NO_ERROR)
    {
      switch (error_code)
      {
        case static_cast<GLenum>(Renderer_error_type::API_ERROR):Gl_renderer::state.type = "API ERROR";
          Gl_renderer::state.severity = "Fatal";
          Gl_renderer::state.code = Renderer_error_type::API_ERROR;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "API ERROR");
          break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:Gl_renderer::state.type = "WINDOW SYSTEM ERROR";
          Gl_renderer::state.severity = "Fatal";
          Gl_renderer::state.code = Renderer_error_type::WINDOW_SYSTEM_ERROR;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "WINDOW SYSTEM ERROR");
          break;
        case static_cast<GLenum>(Renderer_error_type::SHADER_CREATION_ERROR):Gl_renderer::state.type = "SHADER CREATION ERROR";
          Gl_renderer::state.severity = "Fatal";
          Gl_renderer::state.code = Renderer_error_type::SHADER_CREATION_ERROR;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "SHADER CREATION ERROR");
          break;
        case static_cast<GLenum>(Renderer_error_type::SHADER_COMPILATION_ERROR):Gl_renderer::state.type = "SHADER COMPILATION ERROR";
          Gl_renderer::state.severity = "Fatal";
          Gl_renderer::state.code = Renderer_error_type::SHADER_COMPILATION_ERROR;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "SHADER COMPILATION ERROR");
          break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:Gl_renderer::state.type = "THIRD PARTY ERROR";
          Gl_renderer::state.severity = "Fatal";
          Gl_renderer::state.code = Renderer_error_type::THIRD_PARTY_ERROR;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "THIRD PARTY ERROR");
          break;
        case GL_DEBUG_SOURCE_APPLICATION:Gl_renderer::state.type = "APPLICATION ERROR";
          Gl_renderer::state.severity = "Fatal";
          Gl_renderer::state.code = Renderer_error_type::APPLICATION_ERROR;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "APPLICATION ERROR");
          break;
        case GL_INVALID_OPERATION:Gl_renderer::state.type = "INVALID OPERATION";
          Gl_renderer::state.severity = "Warning";
          Gl_renderer::state.code = Renderer_error_type::INVALID_OPERATION;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "INVALID OPERATION");
          break;
        case GL_INVALID_ENUM:Gl_renderer::state.type = "INVALID ENUM";
          Gl_renderer::state.severity = "Warning";
          Gl_renderer::state.code = Renderer_error_type::INVALID_ENUM;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "INVALID ENUM");
          break;
        case GL_INVALID_VALUE:Gl_renderer::state.type = "INVALID VALUE";
          Gl_renderer::state.severity = "Warning";
          Gl_renderer::state.code = Renderer_error_type::INVALID_VALUE;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "INVALID VALUE");
          break;
        case static_cast<GLenum>(Renderer_error_type::INVALID_UNIFORM):Gl_renderer::state.type = "INVALID UNIFORM";
          Gl_renderer::state.severity = "Warning";
          Gl_renderer::state.code = Renderer_error_type::INVALID_UNIFORM;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "INVALID UNIFORM");
          break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:Gl_renderer::state.type = "INVALID FRAMEBUFFER OPERATION";
          Gl_renderer::state.severity = "Warning";
          Gl_renderer::state.code = Renderer_error_type::INVALID_FRAMEBUFFER;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "INVALID FRAMEBUFFER OPERATION");
          break;
        case GL_FRAMEBUFFER_UNDEFINED:Gl_renderer::state.type = "INVALID FRAMEBUFFER UNDEFINED";
          Gl_renderer::state.severity = "Warning";
          Gl_renderer::state.code = Renderer_error_type::UNDEFINED_FRAMEBUFFER;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "INVALID FRAMEBUFFER UNDEFINED");
          break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:Gl_renderer::state.type = "INCOMPLETE FRAMEBUFFER ATTACHMENT";
          Gl_renderer::state.severity = "Warning";
          Gl_renderer::state.code = Renderer_error_type::INCOMPLETE_FRAMEBUFFER_ATTACHMENT;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "INCOMPLETE FRAMEBUFFER ATTACHMENT");
          break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
          Gl_renderer::state.type = "MISSING FRAMEBUFFER ATTACHMENT";
          Gl_renderer::state.severity = "Warning";
          Gl_renderer::state.code = Renderer_error_type::MISSING_FRAMEBUFFER_ATTACHMENT;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "MISSING FRAMEBUFFER ATTACHMENT");
          break;
        case GL_OUT_OF_MEMORY:
        {
          Gl_renderer::state.type = "OUT OF MEMORY";
          Gl_renderer::state.severity = "Fatal";
          Gl_renderer::state.code = Renderer_error_type::OUT_OF_MEMORY;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "OUT OF MEMORY");
          break;
        }
        default:Gl_renderer::state.type = "UNKNOWN ERROR";
          Gl_renderer::state.severity = "Fatal";
          Gl_renderer::state.code = Renderer_error_type::UNKNOWN_ERROR;
          snprintf_result = snprintf(_source, FILENAME_MAX * 4, "UNKNOWN ERROR");
          break;
      }
      
      Gl_renderer::state.description = error_message;
      
      if (snprintf_result < 0)
      {
        Wave::alert(WAVE_ERROR, "%s[SNPRINTF]%s IN FUNCTION %s IN CASE %d",
                    RED, DEFAULT, __FUNCTION__, error_code);
        Gl_renderer::state.description = std::string("Unknown error").c_str();
      }
      else if (snprintf(output, sizeof(output),
                        "[OpenGL error]%s\n%48s%sIn function --> %s,\n%48s%sIn file --> %s,\n"
                        "%48s%sAt line --> %zu,\n%52sDetails --> %s%s",
                        DEFAULT, " ", DEFAULT, function_name, " ", DEFAULT, file_name, " ", DEFAULT, line_number,
                        DEFAULT, error_message, DEFAULT) < 0)
      {
        Gl_renderer::state.description = std::string(
            "Error while building string with snprintf() on line 531 in file renderer.cpp!").c_str();
      }
      else
      {
        Gl_renderer::state.description = output;
        On_renderer_error gl_error(Gl_renderer::state, Renderer_api::Opengl);
        Gl_renderer::renderer_error_callback(gl_error);
      }
    }
  }
  
  void Gl_renderer::shutdown()
  {
    if (Gl_renderer::is_running())
    {
      log_instruction("RENDERER 3D", DEFAULT, "Deleting buffers", Gl_renderer::delete_gl_buffers())
    }
    Gl_renderer::running = false;
  }
}