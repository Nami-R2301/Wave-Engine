//
// Created by nami on 2023-02-23.
//

#pragma once

#include <Math/vector.h>
#include <Renderer/color.h>
#include <Math/matrix_4f.h>

namespace Wave
{
  
  class Vertex_2D
  {
    public:
    Vertex_2D() = default;
    Vertex_2D(float x_coord, float y_coord, float red, float green, float blue, float alpha);
    Vertex_2D(const Math::Vector_2f &position, const Color &color_);
    Vertex_2D(const Math::Vector_2f &position, const Math::Vector_2f &tex_coord_);
    Vertex_2D(const Math::Vector_2f &position, const Color &color, const Math::Vector_2f &tex_coord_);
    Vertex_2D(const Math::Vector_2f &position_, const Math::Vector_2f &normal_, const Color &color_,
              const Math::Vector_2f &tex_coord_);
    Vertex_2D(const Vertex_2D &other_vertex);
    ~Vertex_2D() = default;
    
    [[nodiscard]] float length() const;
    [[nodiscard]] std::string to_string() const;
    void print() const;
    
    [[nodiscard]] Math::Vector_2f get_position() const;
    [[nodiscard]] Math::Vector_2f get_normal() const;
    [[nodiscard]] const Color &get_color() const;
    [[nodiscard]] const Math::Vector_2f &get_tex_coord() const;
    
    void set_id(int32_t id_);
    
    void set_position(const Math::Vector_2f &position_);
    void set_position(float x, float y);
    
    void set_normal(const Math::Vector_2f &normal_);
    void set_normal(float x, float y);
    
    void set_color(const Color &color_);
    void set_color(float r, float g, float b, float a);
    
    void set_tex_coord(const Math::Vector_2f &tex_coord_);
    void set_tex_coord(float x, float y);
    
    bool operator==(const Vertex_2D &other_vertex);
    Vertex_2D &operator=(const Vertex_2D &other_vertex);
    private:
    int32_t id = -1;
    Math::Vector_2f position = Math::Vector_2f(0.0f);
    Math::Vector_2f normal = Math::Vector_2f(0.0f);
    Color color;
    Math::Vector_2f tex_coord = Math::Vector_2f(0.0f);
    float model_matrix[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f},
                                {0.0f, 1.0f, 0.0f, 0.0f},
                                {0.0f, 0.0f, 1.0f, 0.0f},
                                {0.0f, 0.0f, 0.0f, 1.0f}};
  };
  
  // Manipulation of vertices to pass on to our opengl vertex_source shaders.
  class Vertex_3D
  {
    public:
    Vertex_3D() = default;
    Vertex_3D(float x_coord, float y_coord, float z_coord, float red, float green, float blue, float alpha);
    Vertex_3D(const Math::Vector_2f &position, const Color &color_);
    Vertex_3D(const Math::Vector_3f &position, const Color &color_);
    Vertex_3D(const Math::Vector_3f &position, const Math::Vector_2f &tex_coord_);
    Vertex_3D(const Math::Vector_3f &position, const Color &color, const Math::Vector_3f &normal_);
    Vertex_3D(const Math::Vector_3f &position, const Color &color, const Math::Vector_2f &tex_coord_);
    Vertex_3D(const Math::Vector_3f &position, const Math::Vector_3f &normal, const Math::Vector_2f &tex_coord_);
    Vertex_3D(const Math::Vector_3f &position, const Color &color, const Math::Vector_3f &normal,
              const Math::Vector_2f &tex_coord_);
    explicit Vertex_3D(const Vertex_2D &vertex_2D);
    Vertex_3D(const Vertex_3D &other_vertex);
    ~Vertex_3D() = default;
    
    [[nodiscard]] float length() const;
    [[nodiscard]] std::string to_string() const;
    void convert_in_2D();
    void print() const;
    
    [[nodiscard]] Math::Vector_3f get_position() const;
    [[nodiscard]] Math::Vector_3f get_normal() const;
    [[nodiscard]] const Color &get_color() const;
    [[nodiscard]] const Math::Vector_2f &get_tex_coord() const;
    
    void set_id(int32_t id_);
    
    void set_position(const Math::Vector_3f &position_);
    void set_position(float x, float y, float z);
    
    void set_normal(const Math::Vector_3f &normal_);
    void set_normal(float x, float y, float z);
    
    void set_color(const Color &color_);
    void set_color(float r, float g, float b, float a);
    
    void set_tex_coord(const Math::Vector_2f &tex_coord_);
    void set_tex_coord(float x, float y);
    
    void set_model_matrix(const Math::Matrix_4f &model_matrix_);
    
    explicit operator Vertex_2D() const;
    bool operator==(const Vertex_3D &);
    Vertex_3D &operator=(const Vertex_3D &);
    private:
    int32_t id = -1;
    Math::Vector_3f position = Math::Vector_3f(0.0f);
    Math::Vector_3f normal = Math::Vector_3f(0, 0, 1);
    Color color;
    Math::Vector_2f tex_coord = Math::Vector_2f(0);
    float model_matrix[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f},
                                {0.0f, 1.0f, 0.0f, 0.0f},
                                {0.0f, 0.0f, 1.0f, 0.0f},
                                {0.0f, 0.0f, 0.0f, 1.0f}};
  };
}