//
// Created by nami on 11/05/23.
//

#include <Renderer/gl_renderer.h>
#include <Renderer/gl_uniform_buffer.h>

namespace Wave
{
  
  std::shared_ptr<Uniform_buffer> Uniform_buffer::create(uint32_t size, uint32_t binding)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::OpenGL: return std::make_shared<Gl_uniform_buffer>(size, binding);
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Vertex_array_buffer::create()",
                                                   "vertex_array_buffer.cpp", __LINE__ - 2);
      }
    }
    return std::make_shared<Gl_uniform_buffer>(size, binding);
  }
} // Wave