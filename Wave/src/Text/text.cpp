//
// Created by nami on 31/12/22.
//

#include <Text/gl_text.h>
#include <Utilities/logger.h>
#include <Renderer/gl_renderer.h>
#include <Text/text.h>



namespace Wave
{
  
  std::shared_ptr<Text_box> Text_box::create()
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>();
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>();
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(int32_t id_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(id_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(id_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(associated_shader_, id_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(associated_shader_, id_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const Math::Vector_2f &pixel_size,
                                             const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(pixel_size, associated_shader_, id_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(pixel_size, associated_shader_, id_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const Math::Vector_2f &pixel_size_, const std::string &text_,
                                             const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(pixel_size_, text_, associated_shader_, id_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(pixel_size_, text_, associated_shader_, id_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const std::string &string_,
                                             const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(string_, associated_shader_, id_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(string_, associated_shader_, id_);
      }
    }
  }
  
  std::shared_ptr<Text_box>
  Text_box::create(const Math::Vector_2f &pixel_size_, const std::string &text_, const Text_format_s &format_,
                   const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:
        return std::make_shared<Gl_text_box>(pixel_size_, text_, format_, associated_shader_, id_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(pixel_size_, text_, format_, associated_shader_, id_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const Math::Vector_2f &pixel_size_, const std::string &text_,
                                             const char *font_file_path_,
                                             const std::shared_ptr<Shader> &associated_shader_,
                                             int32_t id_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:
        return std::make_shared<Gl_text_box>(pixel_size_, text_, font_file_path_, associated_shader_, id_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(pixel_size_, text_, font_file_path_, associated_shader_, id_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const char *font_file_path, const std::string &string_,
                                             const std::shared_ptr<Shader> &associated_shader_,
                                             int32_t id_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(font_file_path, string_, associated_shader_, id_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(font_file_path, string_, associated_shader_, id_);
      }
    }
  }
  
  std::shared_ptr<Text_box>
  Text_box::create(const char *font_file_path, const std::string &string_, const Text_format_s &format_,
                   const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:
        return std::make_shared<Gl_text_box>(font_file_path, string_, format_, associated_shader_, id_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(font_file_path, string_, format_, associated_shader_, id_);
      }
    }
  }
  
  uint64_t Text_box::get_vertex_size()
  {
    return sizeof(Glyph_quad_s);
  }
}
