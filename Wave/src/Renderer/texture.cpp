//
// Created by nami on 5/26/22.
//



#include <Renderer/gl_renderer.h>
#include <Renderer/texture.h>



namespace Wave
{
  
  std::shared_ptr<Texture> Texture::create(const char *file_path, Texture_data_s texture_data)
  {
    
    switch (texture_data.type)
    {
      case Texture_type::Texture_2D: return Texture_2D::create(file_path, texture_data);
      case Texture_type::Texture_3D: return Texture_3D::create(file_path, texture_data);
      default: return Texture_2D::create(file_path, texture_data);
    }
  }
  
  std::shared_ptr<Texture_2D> Texture_2D::create(const char *file_path, Texture_data_s texture_data)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::OpenGL: return std::make_shared<Gl_texture_2D>(file_path, texture_data);
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Texture_2D::create()",
                                                   "texture.cpp", __LINE__ - 2);
        return std::make_shared<Gl_texture_2D>(file_path, texture_data);
      }
    }
  }
  
  std::shared_ptr<Texture_3D> Texture_3D::create(const char *file_path, Texture_data_s texture_data)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::OpenGL: return std::make_shared<Gl_texture_3D>(file_path, texture_data);
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Texture_3D::create()",
                                                   "texture.cpp", __LINE__ - 2);
        return std::make_shared<Gl_texture_3D>(file_path, texture_data);
      }
    }
  }
}