//
// Created by nami on 29/03/23.
//

#include <Renderer/gl_renderer.h>

namespace Wave
{
  std::shared_ptr<Vertex_buffer> Vertex_buffer::create(uint64_t size, uint64_t count)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_vertex_buffer>(size, count);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
    }
    
    alert(WAVE_LOG_ERROR, "[BUFFER] --> Unknown renderer API! (on line %d in file %s) !", __LINE__, __FILE__);
    return nullptr;
  }
  
  std::shared_ptr<Vertex_buffer> Vertex_buffer::create(const void *vertices, uint64_t size, uint64_t count,
                                                       Buffer_type buffer_type)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:
        return std::make_shared<Gl_vertex_buffer>(vertices, size,
                                                  count, buffer_type);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
    }
    
    alert(WAVE_LOG_ERROR, "[BUFFER] --> Unknown renderer API! (on line %d in file %s) !", __LINE__, __FILE__);
    return nullptr;
  }
  
  std::shared_ptr<Index_buffer> Index_buffer::create(const uint32_t *indices, uint32_t size)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_index_buffer>(indices, size);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
    }
    
    alert(WAVE_LOG_ERROR, "[BUFFER] --> Unknown renderer API! (on line %d in file %s) !", __LINE__, __FILE__);
    return nullptr;
  }
}
