//
// Created by nami on 23/04/23.
//

#include <Renderer/gl_framebuffer.h>
#include <Renderer/renderer.h>

namespace Wave
{
  
  std::shared_ptr<Framebuffer> Framebuffer::create(const Framebuffer_options &opt)
  {
    switch (Gl_renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Opengl:return create_shared_pointer<Gl_framebuffer>(opt);
      case Renderer_api::Vulkan:
        alert(WAVE_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:return nullptr;
    }
  }
}