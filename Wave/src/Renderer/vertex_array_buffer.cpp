//
// Created by nami on 29/03/23.
//

#include <Renderer/gl_renderer.h>

namespace Wave
{
  
  std::shared_ptr<Vertex_array_buffer> Vertex_array_buffer::create()
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Vertex_array_buffer::create()",
                                                   "vertex_array_buffer.cpp", __LINE__ - 2);
        return create_shared_pointer<Gl_vertex_array_buffer>();
      }
      case Renderer_api::Opengl:return create_shared_pointer<Gl_vertex_array_buffer>();
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Vertex_array_buffer::create()",
                                                   "vertex_array_buffer.cpp", __LINE__ - 2);
        return create_shared_pointer<Gl_vertex_array_buffer>();
      }
    }
  }
}