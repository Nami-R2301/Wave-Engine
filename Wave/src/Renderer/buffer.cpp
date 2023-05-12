//
// Created by nami on 29/03/23.
//

#include <Renderer/gl_renderer.h>

namespace Wave
{
  std::shared_ptr<Vertex_buffer> Vertex_buffer::create(uint64_t size)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Opengl:return std::make_shared<Gl_vertex_buffer>(size);
      case Renderer_api::Vulkan:
        alert(WAVE_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
    }
    
    alert(WAVE_ERROR, "[BUFFER] --> Unknown renderer API! (on line %d in file %s) !", __LINE__, __FILE__);
    return nullptr;
  }
  
  std::shared_ptr<Vertex_buffer> Vertex_buffer::create(const void *vertices, uint64_t size, Buffer_type buffer_type)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Opengl:return std::make_shared<Gl_vertex_buffer>(vertices, size, buffer_type);
      case Renderer_api::Vulkan:
        alert(WAVE_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
    }
    
    alert(WAVE_ERROR, "[BUFFER] --> Unknown renderer API! (on line %d in file %s) !", __LINE__, __FILE__);
    return nullptr;
  }
  
  std::shared_ptr<Index_buffer> Index_buffer::create(const uint32_t *indices, uint32_t size)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Opengl:return std::make_shared<Gl_index_buffer>(indices, size);
      case Renderer_api::Vulkan:
        alert(WAVE_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
    }
    
    alert(WAVE_ERROR, "[BUFFER] --> Unknown renderer API! (on line %d in file %s) !", __LINE__, __FILE__);
    return nullptr;
  }
}
