//
// Created by nami on 06/01/23.
//

#pragma once

#include <Math/vector.h>
#include <Math/matrix_4f.h>
#include <Math/transform.h>

#include <Renderer/shader.h>
#include <Utilities/res_loader.h>

namespace Wave
{
  
  /************************************** 2D OBJECT *************************************/
  
  class Object_2D : public Printable, Movable
  {
    public:
    Object_2D() = default;
    Object_2D(const Vector_2f &position_on_screen_, const std::vector<Vertex_2D> &vertices_);
    explicit Object_2D(const std::vector<Vertex_2D> &vertices_);
    Object_2D(const std::vector<Vertex_2D> &vertices_,
              const std::vector<std::shared_ptr<Texture>> &textures_);
    Object_2D(const std::vector<Vertex_2D> &vertices_,
              const std::vector<std::shared_ptr<Texture>> &textures_, const Matrix_4f &model_matrix_);
    Object_2D(const Vector_2f &position_on_screen_, const std::vector<Vertex_2D> &vertices_,
              const std::vector<std::shared_ptr<Texture>> &textures_, const Matrix_4f &model_matrix_);
    Object_2D(const Object_2D &obj);
    explicit Object_2D(const Mesh_2D &mesh);
    ~Object_2D() override = default;
    
    [[nodiscard]] Object_2D clone() const;
    void normalize();
    
    INTERFACE_PRINT
    
    void move(const Vector_3f &vector) override;
    
    [[nodiscard]] const Vector_3f &get_position() const;
    [[nodiscard]] const void *get_vertices() const;
    [[nodiscard]] Vertex_2D get_vertex(uint64_t index) const;
    [[nodiscard]] uint64_t get_vertex_count() const;
    [[nodiscard]] static uint64_t get_vertex_size();
    [[nodiscard]] const std::vector<uint32_t> &get_faces() const;
    [[nodiscard]] const std::vector<Vector_2f> &get_tex_coords() const;
    [[nodiscard]] const std::vector<std::shared_ptr<Texture>> &get_textures() const;
    [[nodiscard]] const Matrix_4f &get_model_matrix() const;
    [[nodiscard]] const Transform &get_model_transform() const;
    
    void set_color(const Color &color);
    void set_position(const Vector_2f &position);
    void reset_position();
    
    void add_face(uint32_t face);
    void replace_face(uint64_t index, uint32_t face_);
    void set_faces(const std::vector<uint32_t> &faces_);
    
    void add_vertex(const Vertex_2D &vertex_);
    void replace_vertex(uint64_t index, const Vertex_2D &new_vertex);
    void set_vertices(const std::vector<Vertex_2D> &vertices_);
    
    void add_texture(const std::shared_ptr<Texture> &texture_);
    void replace_texture(uint64_t index, const std::shared_ptr<Texture> &texture_);
    void set_textures(const std::vector<std::shared_ptr<Texture>> &textures_);
    
    void set_texture_coord(uint64_t index, const Vector_2f &tex_coords_);
    void set_texture_coords(const std::vector<Vector_2f> &tex_coords_);
    void set_model_transform(const Transform &model_transform_);
    void set_model_matrix(const Matrix_4f &mat);
    
    void rotate(const Vector_3f &rotation_);
    void rotate(float x, float y, float z);
    void translate(const Vector_3f &translation_);
    void translate(float x, float y, float z);
    void scale(const Vector_3f &scalar_);
    void scale(float x, float y, float z_);
    
    void update_model_matrix();
    
    [[nodiscard]] Object_2D &operator=(const Object_2D &obj);
    bool operator==(const Object_2D &obj) const;
    protected:
    Vector_3f position{0.0f};
    std::vector<Vertex_2D> vertices;
    std::vector<uint32_t> faces;
    std::vector<Vector_2f> tex_coords;
    std::vector<std::shared_ptr<Texture>> textures;
    Matrix_4f model_matrix{};
    Transform model_transform{};
  };
  
  /************************************** 2D OBJECTS *************************************/
  
  class Point : public Object_2D
  {
    public:
    explicit Point(const std::vector<Vertex_3D> &vertices_);
    Point(float point);
    ~Point() override = default;
  };
  
  class Line : public Object_2D
  {
    public:
    Line(float point_a, float point_b);
    Line(const Point &a, const Point &b);
    Line(const std::vector<Vertex_3D> &vertices_);
    ~Line() override = default;
  };
  
  class Square : public Object_2D
  {
    public:
    Square(float point_a, float point_b, float point_c, float point_d);
    Square(const Point &a, const Point &b, const Point &c, const Point &d);
    Square(const std::vector<Vertex_3D> &vertices_);
    ~Square() override = default;
  };
  
  class Triangle : public Object_2D
  {
    public:
    Triangle(float point_a, float point_b, float point_c);
    Triangle(const Point &a, const Point &b, const Point &c);
    explicit Triangle(const std::vector<Vertex_3D> &vertices_);
    ~Triangle() override = default;
  };
  
  class Circle : public Object_2D
  {
    public:
    Circle(float radius);
    ~Circle() override = default;
  };
  
  class Diamond_2D : public Object_2D
  {
    public:
    Diamond_2D(float point_a, float point_b, float point_c, float point_d);
    Diamond_2D(const Point &a, const Point &b, const Point &c, const Point &d);
    Diamond_2D(const std::vector<Vertex_3D> &vertices_);
    ~Diamond_2D() override = default;
  };
  
  /************************************** 3D OBJECT *************************************/
  
  class Object_3D : public Printable, Movable
  {
    public:
    Object_3D() = default;
    Object_3D(const std::vector<Vertex_3D> &vertices_, const std::vector<uint32_t> &faces_);
    Object_3D(const std::vector<Vertex_3D> &vertices_, const std::vector<uint32_t> &faces_,
              const std::vector<std::shared_ptr<Texture>> &textures_);
    Object_3D(const std::vector<Vertex_3D> &vertices_, const std::vector<uint32_t> &faces_,
              const std::vector<Vector_3f> &normals_);
    explicit Object_3D(const Mesh_3D &mesh);
    Object_3D(const Object_3D &mesh);
    ~Object_3D() override = default;
    
    [[nodiscard]] Object_3D clone() const;
    void convert_in_2D();
    void normalize();
    
    INTERFACE_PRINT
    void move(const Vector_3f &vector) override;
    
    [[nodiscard]] const Vector_3f &get_position() const;
    [[nodiscard]] const void *get_vertices() const;
    [[nodiscard]] Vertex_3D get_vertex(uint64_t index) const;
    [[nodiscard]] uint64_t get_vertex_count() const;
    [[nodiscard]] static uint64_t get_vertex_size();
    [[nodiscard]] const std::vector<uint32_t> &get_faces() const;
    [[nodiscard]] const std::vector<Vector_2f> &get_tex_coords() const;
    [[nodiscard]] const std::vector<std::shared_ptr<Texture>> &get_textures() const;
    [[nodiscard]] const Matrix_4f &get_model_matrix() const;
    [[nodiscard]] const Transform &get_model_transform() const;
    
    void set_color(const Color &color);
    void set_position(const Vector_3f &position);
    void reset_position();
    
    void add_vertex(const Vertex_3D &vertex_);
    void replace_vertex(uint64_t index, const Vertex_3D &new_vertex);
    void set_vertices(const std::vector<Vertex_3D> &vertices_);
    
    void add_face(uint32_t face);
    void replace_face(uint64_t index, uint32_t face_);
    void set_faces(const std::vector<uint32_t> &faces_);
    
    void add_texture(const std::shared_ptr<Texture> &texture_);
    void replace_texture(uint64_t index, const std::shared_ptr<Texture> &texture_);
    void set_textures(const std::vector<std::shared_ptr<Texture>> &textures_);
    
    void set_normal(uint64_t index, const Vector_3f &normal);
    void set_normals(const std::vector<Vector_3f> &normals);
    
    void set_texture_coord(uint64_t index, const Vector_2f &tex_coords_);
    void set_texture_coords(const std::vector<Vector_2f> &tex_coords_);
    void set_model_transform(const Transform &model_transform_);
    void set_model_matrix(const Matrix_4f &mat);
    
    void rotate(const Vector_3f &rotation_);
    void rotate(float x, float y, float z);
    void translate(const Vector_3f &translation_);
    void translate(float x, float y, float z);
    void scale(const Vector_3f &scalar_);
    void scale(float x, float y, float z_);
    
    void update_model_matrix();
    void apply_vertex_properties(const Mesh_3D &mesh);
    
    explicit operator Object_2D() const;
    [[nodiscard]] Object_3D &operator=(const Object_3D &obj);
    bool operator==(const Object_3D &obj) const;
    protected:
    Vector_3f position{0.0f};
    std::vector<Vertex_3D> vertices;
    std::vector<Vector_3f> normals;
    std::vector<uint32_t> faces;
    std::vector<Vector_2f> tex_coords;
    std::vector<std::shared_ptr<Texture>> textures;
    Matrix_4f model_matrix{};
    Transform model_transform{};
  };
  
  /************************************** 3D OBJECTS *************************************/
  
  class Cube : public Object_3D
  {
    public:
    explicit Cube(const Mesh_3D &mesh, const Vector_3f &scale = Vector_3f(1.0f), const Color &color = Color());
    ~Cube() override = default;
  };
  
  class Pyramid : public Object_3D
  {
    public:
    Pyramid(const std::vector<Vertex_3D> &vertices_);
    Pyramid(const std::vector<Vertex_3D> &vertices_, const std::vector<uint64_t> &faces_);
    ~Pyramid() override = default;
  };
  
  class Sphere : public Object_3D
  {
    public:
    Sphere(const std::vector<Vertex_3D> &vertices_);
    Sphere(const std::vector<Vertex_3D> &vertices_, const std::vector<uint64_t> &faces_);
    ~Sphere() override = default;
  };
  
  class Diamond_3D : public Object_3D
  {
    public:
    Diamond_3D(const std::vector<Vertex_3D> &vertices_);
    Diamond_3D(const std::vector<Vertex_3D> &vertices_, const std::vector<uint64_t> &faces_);
    ~Diamond_3D() override = default;
  };
}
