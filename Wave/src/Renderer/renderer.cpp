
#include <Renderer/gl_renderer.h>
#include <Renderer/renderer.h>


namespace Wave
{
  
  Renderer_api Renderer::api_in_use = Renderer_api::None;
  
  Renderer_api Renderer::get_api()
  {
    return Renderer::api_in_use;
  }
  
  int32_t Renderer::get_max_texture_units()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL: return Gl_renderer::get_max_texture_units();
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Renderer::init(Renderer_api api)",
                                                   "renderer.cpp", __LINE__ - 2);
        return Gl_renderer::get_max_texture_units();
      }
    }
  }
  
  void Renderer::set_event_callback_function(const std::function<void(Event &)> &callback)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::set_event_callback_function(callback);
        break;
      }
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Renderer::set_event_callback_function(Renderer_api api, const std::function<void (Event &)> callback)",
                                                   "renderer.cpp", __LINE__ - 2);
        Gl_renderer::set_event_callback_function(callback);
      }
    }
  }
  
  void Renderer::create(Renderer_api api)
  {
    switch (api)
    {
      case Renderer_api::OpenGL:
      {
        Renderer::api_in_use = Gl_renderer::get_api();
        break;
      }
      default:
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Renderer::init(Renderer_api api)",
                                                   "renderer.cpp", __LINE__ - 2);
        Renderer::api_in_use = Gl_renderer::get_api();
        break;
    }
  }
  
  void Renderer::init()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::init();
        break;
      }
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Renderer::init(Renderer_api api)",
                                                   "renderer.cpp", __LINE__ - 2);
        Gl_renderer::init();
        Renderer::api_in_use = Renderer_api::OpenGL;
      }
    }
  }
  
  bool Renderer::is_running()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:return Gl_renderer::is_running();
      default:return false;
    }
  }
  
  const char *Renderer::get_api_version()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL: return Gl_renderer::get_api_version();
      default:
        return "Api version undefined! This is likely due to an incorrect renderer API being set as active. "
               "Refer to the documentation to see if the current API is supported or not.";
    }
  }
  
  const char *Renderer::get_api_shader_version()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL: return Gl_renderer::get_api_shader_version();
      default:
        return "Api version undefined! This is likely due to an incorrect renderer API being set as active. "
               "Refer to the documentation to see if the current API is supported or not.";
    }
  }
  
  Renderer_state Renderer::get_state()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:return Gl_renderer::get_state();
      default:return {nullptr, nullptr, nullptr, WAVE_RENDERER_SHUTDOWN};
    }
  }
  
  void Renderer::clear_errors()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::gl_clear_errors();
        break;
      }
      default:break;
    }
  }
  
  void Renderer::clear_bg()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::clear_bg();
        break;
      }
      default:break;
    }
  }
  
  void Renderer::set_clear_color(const Color &bg_color)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::set_clear_color(bg_color);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::set_viewport(const Vector_2f &viewport)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::set_viewport(viewport.get_x(), viewport.get_y());
        break;
      }
      default:break;
    }
  }
  
  void Renderer::on_event(Event &event)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::on_event(event);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::load_dynamic_data(const void *vertices, size_t size, uint64_t command_index, uint64_t vbo_index)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::load_dynamic_data(vertices, size, command_index, vbo_index);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::prerender_text()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::init_text_buffers();
        break;
      }
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "[Renderer] --> API currently not supported or active, auto selecting OpenGL API renderer...",
                                                   __FUNCTION__,
                                                   "renderer.cpp",
                                                   __LINE__ - 6);
        Gl_renderer::init_text_buffers();
      }
    }
  }
  
  void Renderer::init_object_buffers()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::init_object_buffers();
        break;
      }
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "[Renderer] --> API currently not supported or active, auto selecting OpenGL API renderer...",
                                                   __FUNCTION__,
                                                   "renderer.cpp",
                                                   __LINE__ - 6);
        Gl_renderer::init_object_buffers();
      }
    }
  }
  
  void Renderer::draw_object(const std::shared_ptr<Object> &object)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::draw_object(object);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::draw_text(const std::shared_ptr<Text> &text)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::draw_text(text);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::begin(std::shared_ptr<Camera> &camera)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::begin(camera);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::end()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::end();
        break;
      }
      default:break;
    }
  }
  
  void Renderer::flush()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::flush();
        break;
      }
      default:break;
    }
  }
  
  void Renderer::send(const std::vector<std::shared_ptr<Object>> &objects)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::send(objects);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::shutdown()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:
      {
        Gl_renderer::shutdown();
        break;
      }
      default:break;
    }
  }
  
  bool Renderer::has_crashed()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::OpenGL:return Gl_renderer::has_crashed();
      default:return true;
    }
  }
}