//
// Created by nami on 5/20/22.
//

#pragma once

#include <Renderer/vertex.h>
#include <Renderer/texture.h>

namespace Wave
{
  
  typedef struct Face
  {
    int first_vertex_index;
    int first_texture_index;
    int first_normal_index;
    
    int second_vertex_index;
    int second_texture_index;
    int second_normal_index;
    
    int third_vertex_index;
    int third_texture_index;
    int third_normal_index;
  } Face;
  
  typedef struct Mesh_2D
  {
    std::vector<Vertex_3D> vertices;
    std::vector<Vector_2f> tex_coords;
  } Mesh_2D;
  
  typedef struct Mesh_3D
  {
    std::vector<Vertex_3D> vertices;
    std::vector<Face> face_indices;
    std::vector<Vector_3f> normals;
    std::vector<Vector_2f> tex_coords;
  } Mesh_3D;
  
  class Res_loader_3D
  {
  public:
    explicit Res_loader_3D(const char *file_path);
    ~Res_loader_3D();
    
    Mesh_2D load_2D_mesh();
    Mesh_3D load_3D_mesh();
    static std::string load_shader_source(const char *file_path);
    static Texture load_texture(const char *file_path);
  private:
    std::vector<Vertex_3D> vertices;
    std::vector<Face> face_indices;
    std::vector<Vector_3f> normals;
    std::vector<Vector_2f> tex_coords;
    const char *file_path = nullptr;
    FILE *file_ptr = nullptr;
    
    void load_obj_vertex();
    void load_obj_face();
    void load_obj_normal();
    void load_obj_texture_coords();
  };
}