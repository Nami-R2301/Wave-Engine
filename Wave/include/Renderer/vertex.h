//
// Created by nami on 2023-02-23.
//

#pragma once

#include <Math/vector.h>
#include <Renderer/color.h>

namespace Wave
{
  
  class Vertex_2D
  {
    public:
    Vertex_2D() = default;
    Vertex_2D(float x_coord, float y_coord, float red, float green, float blue, float alpha);
    Vertex_2D(const Vector_2f &position, const Color &color_);
    Vertex_2D(const Vector_2f &position, const Vector_2f &tex_coord_);
    Vertex_2D(const Vector_2f &position, const Color &color, const Vector_2f &tex_coord_);
    Vertex_2D(const Vector_2f &position_, const Vector_2f &normal_, const Color &color_, const Vector_2f &tex_coord_);
    Vertex_2D(const Vertex_2D &other_vertex);
    ~Vertex_2D() = default;
    
    [[nodiscard]] float length() const;
    [[nodiscard]] std::string to_string() const;
    void print() const;
    
    [[nodiscard]] Vector_2f get_position() const;
    [[nodiscard]] Vector_2f get_normal() const;
    [[nodiscard]] const Color &get_color() const;
    [[nodiscard]] const Vector_2f &get_tex_coord() const;
    
    void set_position(const Vector_2f &position_);
    void set_position(float x, float y);
    
    void set_normal(const Vector_2f &normal_);
    void set_normal(float x, float y);
    
    void set_color(const Color &color_);
    void set_color(float r, float g, float b, float a);
    
    void set_tex_coord(const Vector_2f &tex_coord_);
    void set_tex_coord(float x, float y);
    
    bool operator==(const Vertex_2D &other_vertex);
    Vertex_2D &operator=(const Vertex_2D &other_vertex);
    private:
    Vector_2f position = Vector_2f(0.0f);
    Vector_2f normal = Vector_2f(0.0f);
    Color color;
    Vector_2f tex_coord = Vector_2f(0.0f);
  };
  
  // Manipulation of vertices to pass on to our opengl vertex_source shaders.
  class Vertex_3D
  {
    public:
    Vertex_3D() = default;
    Vertex_3D(float x_coord, float y_coord, float z_coord, float red, float green, float blue, float alpha);
    Vertex_3D(const Vector_2f &position, const Color &color_);
    Vertex_3D(const Vector_3f &position, const Color &color_);
    Vertex_3D(const Vector_3f &position, const Vector_2f &tex_coord_);
    Vertex_3D(const Vector_3f &position, const Color &color, const Vector_3f &normal_);
    Vertex_3D(const Vector_3f &position, const Color &color, const Vector_2f &tex_coord_);
    Vertex_3D(const Vector_3f &position, const Vector_3f &normal, const Vector_2f &tex_coord_);
    Vertex_3D(const Vector_3f &position, const Color &color, const Vector_3f &normal, const Vector_2f &tex_coord_);
    explicit Vertex_3D(const Vertex_2D &vertex_2D);
    Vertex_3D(const Vertex_3D &other_vertex);
    ~Vertex_3D() = default;
    
    [[nodiscard]] float length() const;
    [[nodiscard]] std::string to_string() const;
    void convert_in_2D();
    void print() const;
    
    [[nodiscard]] Vector_3f get_position() const;
    [[nodiscard]] Vector_3f get_normal() const;
    [[nodiscard]] const Color &get_color() const;
    [[nodiscard]] const Vector_2f &get_tex_coord() const;
    
    void set_position(const Vector_3f &position_);
    void set_position(float x, float y, float z);
    
    void set_normal(const Vector_3f &normal_);
    void set_normal(float x, float y, float z);
    
    void set_color(const Color &color_);
    void set_color(float r, float g, float b, float a);
    
    void set_tex_coord(const Vector_2f &tex_coord_);
    void set_tex_coord(float x, float y);
    
    explicit operator Vertex_2D() const;
    bool operator==(const Vertex_3D &);
    Vertex_3D &operator=(const Vertex_3D &);
    private:
    Vector_3f position = Vector_3f(0);
    Vector_3f normal = Vector_3f(0, 0, 1);
    Color color;
    Vector_2f tex_coord = Vector_2f(0);
  };
}