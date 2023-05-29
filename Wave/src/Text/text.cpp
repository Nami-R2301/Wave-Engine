//
// Created by nami on 31/12/22.
//

#include <Text/gl_text.h>
#include <Utilities/logger.h>
#include <Renderer/gl_renderer.h>
#include <Text/text.h>



namespace Wave
{
  
  std::shared_ptr<Text> Text::create()
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text>();
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
                                                   "Text::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text>();
      }
    }
  }
  
  std::shared_ptr<Text> Text::create(const std::string &string_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text>(string_);
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
                                                   "Text::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text>(string_);
      }
    }
  }
  
  std::shared_ptr<Text> Text::create(const char *font_file_path, const std::string &string_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text>(font_file_path, string_);
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
                                                   "Text::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text>(font_file_path, string_);
      }
    }
  }
  
  std::shared_ptr<Text> Text::create(const char *font_file_path, const std::string &string_, const Text_format &format_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text>(font_file_path, string_, format_);
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
                                                   "Text::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text>(font_file_path, string_, format_);
      }
    }
  }
}
