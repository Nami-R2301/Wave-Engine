
#include <Renderer/gl_renderer.h>



namespace Wave
{
  
  Renderer_api Renderer::api_in_use = Renderer_api::None;
  
  Renderer_api Renderer::get_api()
  {
    return Renderer::api_in_use;
  }
  
  void Renderer::set_event_callback_function(const std::function<void(Event &)> &callback)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
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
      case Renderer_api::Opengl:
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
      case Renderer_api::Opengl:
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
        Renderer::api_in_use = Renderer_api::Opengl;
      }
    }
  }
  
  bool Renderer::is_running()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:return Gl_renderer::is_running();
      default:return false;
    }
  }
  
  Renderer_state Renderer::get_state()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:return Gl_renderer::get_state();
      default:return {nullptr, nullptr, nullptr, WAVE_RENDERER_SHUTDOWN};
    }
  }
  
  void Renderer::clear_errors()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
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
      case Renderer_api::Opengl:
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
      case Renderer_api::Opengl:
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
      case Renderer_api::Opengl:
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
      case Renderer_api::Opengl:
      {
        Gl_renderer::on_event(event);
        break;
      }
      default:break;
    }
  }
  
  std::shared_ptr<Vertex_array_buffer> Renderer::load_text()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:return Gl_renderer::load_text();
      default:return nullptr;
    }
  }
  
  [[maybe_unused]] void Renderer::load_dynamic_data(const void *vertices, size_t size, uint64_t vbo_index)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
      {
        Gl_renderer::load_dynamic_data(vertices, size, vbo_index);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::load_object(const Object_3D *object)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
      {
        Gl_renderer::load_object(object);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::draw_object(const Object_3D *object)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
      {
        Gl_renderer::draw_object(object);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::draw_objects(const std::vector<Object_3D> *objects)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
      {
        Gl_renderer::draw_objects(objects);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::draw_loaded_objects(uint32_t object_count)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
      {
        Gl_renderer::draw_loaded_objects(object_count);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::draw_text(const std::shared_ptr<Text> &text, const std::shared_ptr<Vertex_array_buffer> &vao)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
      {
        Gl_renderer::draw_text(text, vao);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::begin_scene(Camera &camera)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
      {
        Gl_renderer::begin_scene(camera);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::end_scene()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
      {
        Gl_renderer::end_scene();
        break;
      }
      default:break;
    }
  }
  
  void Renderer::flush()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
      {
        Gl_renderer::flush();
        break;
      }
      default:break;
    }
  }
  
  void Renderer::send(const std::shared_ptr<Shader> &shader,
                      const std::shared_ptr<Vertex_array_buffer> &vertexArray,
                      const Matrix_4f &transform)
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
      {
        Gl_renderer::send(shader, vertexArray, transform);
        break;
      }
      default:break;
    }
  }
  
  void Renderer::shutdown()
  {
    switch (Renderer::api_in_use)
    {
      case Renderer_api::Opengl:
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
      case Renderer_api::Opengl:return Gl_renderer::has_crashed();
      default:return true;
    }
  }
}