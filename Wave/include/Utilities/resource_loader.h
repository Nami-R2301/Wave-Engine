//
// Created by nami on 5/20/22.
//

#pragma once

#include <Renderer/vertex.h>
#include <Renderer/texture.h>
#include <Objects/object.h>
#include <vector>

namespace Wave
{
  
  class Resource_loader
  {
    public:
    ~Resource_loader() = default;
    
    static std::string load_shader_source(const char *file_path);
    static Object_2D_data_s load_object_2D_source(const char *file_path);
    static Object_3D_data_s load_object_3D_source(const char *file_path);
//    static Font_data_s load_font_source(const char *file_path);
    static std::shared_ptr<Texture> load_texture_source(const char *file_path);
    
    private:
    static Math::Vector_2f load_object_2D_vertex(FILE *file_ptr);
    static Face_2D_s load_object_2D_face(FILE *file_ptr);
    static Math::Vector_2f load_object_2D_normal(FILE *file_ptr);
    
    static Math::Vector_3f load_object_3D_vertex(FILE *file_ptr);
    static Face_3D_s load_object_3D_face(FILE *file_ptr);
    static Math::Vector_3f load_object_3D_normal(FILE *file_ptr);
    static Math::Vector_2f load_object_texture_coords(FILE *file_ptr);
  };
}