//
// Created by nami on 2022-02-23.
//

#include <Renderer/gl_renderer.h>


namespace Wave
{
  
  std::shared_ptr<Shader> Shader::create(const std::string &name, const std::string &vertex_source,
                                         const std::string &fragment_source)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::OpenGL:return std::make_shared<Gl_shader>(name, vertex_source, fragment_source);
      default:
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Please select OpenGL instead.",
                                                   "Shader::create(const std::string &name, const char *vertex_source, "
                                                   "const char *fragment_source)", "shader.cpp", __LINE__ - 9);
    }
    return std::make_shared<Gl_shader>(name, vertex_source, fragment_source);
  }
  
  bool Shader::operator!=(const Shader &other_shader) const
  {
    return !(*this == other_shader);
  }
}