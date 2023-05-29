//
// Created by nami on 23/04/23.
//

#include <Renderer/gl_renderer.h>

namespace Wave
{
  
  std::shared_ptr<Framebuffer> Framebuffer::create(const Framebuffer_options &opt)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_framebuffer>(opt);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:return nullptr;
    }
  }
}