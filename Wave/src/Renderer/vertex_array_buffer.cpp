//
// Created by nami on 29/03/23.
//

#include <Renderer/vertex_array_buffer.h>
#include <Renderer/renderer.h>

namespace Wave
{
  
  std::shared_ptr<Vertex_array_buffer> Vertex_array_buffer::create()
  {
    switch (Gl_renderer::get_api())
    {
      case Renderer_api::Opengl:return create_shared_pointer<Gl_vertex_array_buffer>();
      default:
        Gl_renderer::gl_error_callback(static_cast<GLenum>(Renderer_error_type::API_ERROR),
                                       "vertex_array_buffer.cpp",
                                       "Api not supported at the moment! Please select OpenGL instead.",
                                       "Vertex_array_buffer::create()", __LINE__ - 2);
        return nullptr;
    }
  }
}