//
// Created by nami on 10/05/23.
//

#include <Renderer/gl_renderer.h>
#include <Scene/editor_camera.h>


namespace Wave
{
  Renderer_api Gl_renderer::api = Renderer_api::Opengl;
  Renderer_state Gl_renderer::state = {nullptr,
                                       nullptr,
                                       nullptr,
                                       WAVE_RENDERER_INIT};
  Renderer::Draw_commands_list Gl_renderer::draw_commands;
  std::function<void(Event &)> Gl_renderer::event_callback_function;
  
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
  
  [[maybe_unused]] const std::function<void(Event &event)> &Gl_renderer::get_event_callback_function()
  {
    return Gl_renderer::event_callback_function;
  }
  
  void Gl_renderer::begin_scene(Camera &camera)
  {
    float *view = &(camera.get_view_matrix().get_matrix()[0][0]);
    float *projection = &(camera.get_projection_matrix().get_matrix()[0][0]);
    
    Gl_renderer::draw_commands.uniform_buffers.back()->set_data(0, 64, view);
    Gl_renderer::draw_commands.uniform_buffers.back()->set_data(64, 64, projection);
  }
  
  void Gl_renderer::end_scene()
  {
    Gl_renderer::draw_commands.uniform_buffers.clear();
  }
  
  void Gl_renderer::flush()
  {
  }
  
  void Gl_renderer::send(const std::shared_ptr<Shader> &shader,
                         const std::shared_ptr<Vertex_array_buffer> &vertexArray,
                         const Matrix_4f &transform)
  {
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
  
  void Gl_renderer::init()
  {
    LOG_TASK("RENDERER 3D", CYAN, 1, "Loading 3D renderer ...",
             {
               LOG_INSTRUCTION("RENDERER 3D", DEFAULT, "Loading GLEW", CHECK_GL_CALL(glewInit()))
               Gl_renderer::set_state(Gl_renderer::get_state());
               
               /****** Enable OpenGL error handling. ********/
               // Check if we successfully initialized a debug context
               int flags;
               glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
               if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
               {
                 glEnable(GL_DEBUG_OUTPUT);  // Enable debug output.
                 glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);  // Call the callback as soon as there's an error.
               }
               
               Color background_color;  // Default dark-mode-like color for background.
               CHECK_GL_CALL(glClearColor(background_color.get_red(), background_color.get_green(),
                                          background_color.get_blue(), background_color.get_alpha()));
               
               CHECK_GL_CALL(
                 glFrontFace(GL_CW)); // Every shape drawn in clock-wise manner will be considered the FRONT face.
               CHECK_GL_CALL(glCullFace(GL_FRONT_AND_BACK)); // The back side of shapes will NOT be drawn.
               CHECK_GL_CALL(
                 glDisable(GL_CULL_FACE)); // Don't update the back face of shapes since the camera won't see it.
               
               // Let OpenGL keep track of depth for shapes and auto determine if some shapes closer or further away from
               // the camera should take priority (drawn on top of other ones).
               CHECK_GL_CALL(glEnable(GL_DEPTH_TEST));
               CHECK_GL_CALL(glEnable(GL_MULTISAMPLE));
               CHECK_GL_CALL(glEnable(GL_BLEND));
               CHECK_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
               
               // Let OpenGL do the exponential gamma correction for us so textures and colors don't appear as dark.
//               CHECK_GL_CALL(glEnable(GL_FRAMEBUFFER_SRGB));
      }, "Renderer 3D loaded")
    Gl_renderer::set_state({nullptr, nullptr, nullptr, WAVE_RENDERER_RUNNING});

#if defined(OPENGL_VERSION_4_3_PLUS) && defined(DEBUG)
    glDebugMessageCallback(gl_asynchronous_error_callback, nullptr);
          // Filter the messages we want to debug or not with the last param 'enabled'.
          glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
#endif
    
    LOG_TASK("RENDERER 3D", YELLOW, 1, "Fetching renderer info on client system ...",
             Gl_renderer::show_renderer_info(),
             "Renderer info fetched")
  }
  
  const char *Gl_renderer::get_gl_version()
  {
    return (const char *) (glGetString(GL_VERSION));
  }
  
  void Gl_renderer::show_renderer_info()
  {
    // Get GLFW and OpenGL version.
    int32_t num_ext;
    CHECK_GL_CALL(glGetIntegerv(GL_NUM_EXTENSIONS, &num_ext));
    Wave::alert(WAVE_INFO, "[RENDERER] --> Api (Glew) version : %d.%d.%d", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR,
                GLEW_VERSION_MICRO);
    Wave::alert(WAVE_INFO, "[RENDERER] --> Graphics renderer : %s", glGetString(GL_RENDERER));
    Wave::alert(WAVE_INFO, "[RENDERER] --> Extensions available : %d", num_ext);
    Wave::alert(WAVE_INFO, "[RENDERER] --> Opengl Vendor : %s", glGetString(GL_VENDOR));
    Wave::alert(WAVE_INFO, "[RENDERER] --> Opengl version : %s", get_gl_version());
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
    if (width <= 0.0f || height <= 0.0f)
      gl_synchronous_error_callback(GL_INVALID_VIEWPORT_DIMENSIONS, "Invalid viewport size given!", __FUNCTION__,
                                    __FILE__, __LINE__);
    CHECK_GL_CALL(glViewport(0, 0, width, height));
  }
  
  void Gl_renderer::load_object(const Object_3D *object)
  {
    std::vector<Buffer_element> b_elements;
    b_elements.emplace_back(Buffer_data_type::Vector_3f, "Position", true);
    b_elements.emplace_back(Buffer_data_type::Vector_3f, "Normal", true);
    b_elements.emplace_back(Buffer_data_type::Color_4f, "Color", true);
    b_elements.emplace_back(Buffer_data_type::Vector_2f, "Texture coords", true);
    
    Gl_renderer::draw_commands.names.emplace_back("3D Object");
    Gl_renderer::draw_commands.vertex_array_buffers.emplace_back(Vertex_array_buffer::create());
    Gl_renderer::draw_commands.vertex_array_buffers.back()->set_index_buffer(
      Index_buffer::create(object->get_faces().data(),
                           static_cast<uint32_t>(object->get_faces().size())));
    Gl_renderer::draw_commands.uniform_buffers.emplace_back(
      Uniform_buffer::create(64 * 2, 3));
    
    auto vbo_ = Vertex_buffer::create(object->get_vertices(),
                                      Object_3D::get_vertex_size() *
                                      object->get_vertex_count(),
                                      STATIC_DRAW);
    Buffer_layout vbo_layout(b_elements);
    vbo_->set_layout(vbo_layout);
    
    Gl_renderer::draw_commands.vertex_array_buffers.back()->add_vertex_buffer(vbo_);
    Gl_renderer::draw_commands.textures.emplace_back(object->get_textures()[0]);
    Gl_renderer::draw_commands.vertex_array_buffers.back()->unbind();
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
  
  [[maybe_unused]] void Gl_renderer::load_dynamic_data(const void *vertices, size_t size, uint64_t vbo_index)
  {
    if (!vertices || size == 0) return;
    Gl_renderer::draw_commands.vertex_array_buffers.back()->get_vertex_buffers()[vbo_index]->set_data(vertices, size,
                                                                                                      0);
  }
  
  void Gl_renderer::draw_object(const Object_3D *object)
  {
    load_object(object);
    draw_loaded_objects(1);
  }
  
  [[maybe_unused]] void Gl_renderer::draw_objects(const std::vector<Object_3D> *objects)
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
    Gl_renderer::draw_commands.vertex_array_buffers.back()->bind();
    if (!Gl_renderer::draw_commands.textures.empty()) Gl_renderer::draw_commands.textures.back()->bind(0);
    CHECK_GL_CALL(glDrawElements(GL_TRIANGLES,
                                 Gl_renderer::draw_commands.vertex_array_buffers.back()->get_index_buffer()->get_count(),
                                 GL_UNSIGNED_INT, nullptr));
    Gl_renderer::draw_commands.vertex_array_buffers.back()->unbind();
    if (!Gl_renderer::draw_commands.textures.empty()) Gl_renderer::draw_commands.textures[0]->unbind();
  }
  
  void Gl_renderer::draw_text(const std::shared_ptr<Text> &text, const std::shared_ptr<Vertex_array_buffer> &vao)
  {
    CHECK_GL_CALL(glActiveTexture(GL_TEXTURE1));
    CHECK_GL_CALL(glBindVertexArray(vao->get_id()));
    
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
      CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D, ch.texture_id));
      
      // Update content of VBO memory
      CHECK_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vao->get_vertex_buffers().back()->get_id()));
      
      // Use glBufferSubData and not glBufferData
      CHECK_GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
      CHECK_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
      
      // Draw
      CHECK_GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
      // Advance cursors for next glyph (note that advance is number of 1/64 pixels)
      x += static_cast<float>((ch.advance >> 6)) *
           scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    CHECK_GL_CALL(glBindVertexArray(0));
    CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
  }
  
  void Gl_renderer::shutdown()
  {
    Gl_renderer::set_state({nullptr, nullptr, nullptr, WAVE_RENDERER_SHUTDOWN});
  }
  
  bool Gl_renderer::has_crashed()
  {
    return Gl_renderer::state.code != WAVE_RENDERER_RUNNING &&
           Gl_renderer::state.code != WAVE_RENDERER_INIT &&
           Gl_renderer::state.code != WAVE_RENDERER_SHUTDOWN;
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
        case GL_INVALID_VIEWPORT_DIMENSIONS:
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
        case GL_DEBUG_SOURCE_INVALID_UNIFORM:
        {
          temp.type = "Invalid uniform";
          temp.severity = "Warning";
          temp.code = GL_DEBUG_SOURCE_INVALID_UNIFORM;
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
        Wave::alert(WAVE_ERROR, "%s[SNPRINTF]%s IN FUNCTION %s IN CASE %d",
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
        On_renderer_error gl_error(temp, Renderer_api::Opengl);
        Gl_renderer::renderer_error_callback(gl_error);
      }
    }
  }

#ifdef OPENGL_VERSION_4_3_PLUS
  
  void Gl_renderer::gl_asynchronous_error_callback(GLenum error_code, GLenum type, [[maybe_unused]] GLuint id,
                                    GLenum severity, GLsizei length, const GLchar *error_message,
                                    [[maybe_unused]] const void *userParam)
{
  if (!Gl_renderer::is_running()) return;
  
  Renderer_state temp{};
  char output[length * 4];
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
      case GL_DEBUG_SOURCE_INVALID_UNIFORM:
      {
        temp.code = GL_DEBUG_SOURCE_INVALID_UNIFORM;
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
      On_renderer_error gl_error(temp, Renderer_api::Opengl);
      Gl_renderer::renderer_error_callback(gl_error);
    }
  }
}
#endif
}