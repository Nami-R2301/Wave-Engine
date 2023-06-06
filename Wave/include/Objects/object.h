//
// Created by nami on 06/01/23.
//

#pragma once

#include <Math/vector.h>
#include <Math/matrix_4f.h>
#include <Math/transform.h>

#include <Renderer/texture.h>
#include <Renderer/vertex.h>
#include <Renderer/shader.h>

namespace Wave
{
  
  typedef struct Face_2D_s
  {
    uint32_t first_vertex_index;
    uint32_t first_normal_index;
    uint32_t first_texture_index;
    
    uint32_t second_vertex_index;
    uint32_t second_texture_index;
    uint32_t second_normal_index;
  } Face_2D_s;
  
  
  typedef struct Face_3D_s
  {
    uint32_t first_vertex_index;
    uint32_t first_texture_index;
    uint32_t first_normal_index;
    
    uint32_t second_vertex_index;
    uint32_t second_texture_index;
    uint32_t second_normal_index;
    
    uint32_t third_vertex_index;
    uint32_t third_texture_index;
    uint32_t third_normal_index;
  } Face_3D_s;
  
  typedef struct Object_2D_data_s
  {
    Vector_2f origin;
    std::vector<Vertex_2D> vertices;
    std::vector<Face_2D_s> indices;
    std::vector<Vector_2f> tex_coords;
    std::vector<std::shared_ptr<Texture>> textures;
  } Object_2D_data_s;
  
  typedef struct Object_3D_data_s
  {
    Vector_3f origin;
    std::vector<Vertex_3D> vertices;
    std::vector<Face_3D_s> indices;
    std::vector<Vector_3f> normals;
    std::vector<Vector_2f> tex_coords;
    std::vector<Texture> textures;
  } Object_3D_data_s;
  
  enum class Object_type_e
  {
    Object_2D,
    Object_3D
  };
  
  class Object : public Printable, public Movable, public Rotatable, public Copiable, public Scalable, public Sendable
  {
    public:
    ~Object() override = default;
    
    static std::shared_ptr<Object> create();
    static std::shared_ptr<Object> create(const Object_2D_data_s &object_2D_data);
    static std::shared_ptr<Object> create(const Object_3D_data_s &object_3D_data);
    
    [[nodiscard]] virtual const void *get_vertices() const = 0;
    [[nodiscard]] virtual uint64_t get_vertex_count() const = 0;
    [[nodiscard]] virtual uint64_t get_vertex_size() const = 0;
    [[nodiscard]] virtual const void *get_faces() const = 0;
    [[nodiscard]] virtual uint64_t get_face_count() const = 0;
    [[nodiscard]] virtual const std::vector<std::shared_ptr<Texture>> &get_textures() const = 0;
    [[nodiscard]] virtual uint64_t get_texture_count() const = 0;
    [[nodiscard]] virtual const Matrix_4f &get_model_matrix() const = 0;
    [[nodiscard]] virtual const Transform &get_model_transform() const = 0;
    
    virtual void set_color(const Color &color) = 0;
    virtual void add_texture(const std::shared_ptr<Texture> &texture_) = 0;
    virtual void replace_texture(uint64_t index, const std::shared_ptr<Texture> &texture_) = 0;
    virtual void set_textures(const std::vector<std::shared_ptr<Texture>> &textures_) = 0;
    
    virtual void set_texture_coord(uint64_t index, const Vector_2f &tex_coords_) = 0;
    virtual void set_texture_coords(const std::vector<Vector_2f> &tex_coords_) = 0;
    virtual void set_model_transform(const Transform &model_transform_) = 0;
    virtual void set_model_matrix(const Matrix_4f &mat) = 0;
    
    virtual void normalize() = 0;
    virtual void update_model_matrix() = 0;
  };
  
  /************************************** 2D OBJECT *************************************/
  
  class Object_2D : public Object
  {
    public:
    Object_2D() = default;
    Object_2D(const Object_2D &sprite);
    explicit Object_2D(const Object_2D_data_s &sprite_data);
    ~Object_2D() override;
    
    // Interfaces.
    INTERFACE_SENDABLE
    
    INTERFACE_PRINTABLE
    INTERFACE_COPIABLE
    INTERFACE_MOVABLE
    INTERFACE_ROTATABLE
    INTERFACE_SCALABLE
    
    [[nodiscard]] Object_type_e get_type() const;
    [[nodiscard]] const Vector_2f &get_position() const;
    [[nodiscard]] const void *get_vertices() const override;
    [[nodiscard]] const Vertex_2D &get_vertex(uint64_t index) const;
    [[nodiscard]] uint64_t get_vertex_count() const override;
    [[nodiscard]] uint64_t get_vertex_size() const override;
    [[nodiscard]] const void *get_faces() const override;
    [[nodiscard]] uint64_t get_face_count() const override;
    [[nodiscard]] const std::vector<Vector_2f> &get_normals() const;
    [[nodiscard]] const std::vector<Vector_2f> &get_tex_coords() const;
    [[nodiscard]] const std::vector<std::shared_ptr<Texture>> &get_textures() const override;
    [[nodiscard]] uint64_t get_texture_count() const override;
    [[nodiscard]] const Matrix_4f &get_model_matrix() const override;
    [[nodiscard]] const Transform &get_model_transform() const override;
    
    void set_origin(const Vector_2f &origin_);
    void set_color(const Color &color) override;
    void set_position(const Vector_3f &position);
    void reset_position();
    
    void add_face(uint32_t face);
    void replace_face(uint64_t index, uint32_t face_);
    void set_faces(const std::vector<uint32_t> &faces_);
    
    void set_normal(uint64_t index, const Vector_2f &normal_);
    void set_normals(const std::vector<Vector_2f> &normal_);
    
    void add_vertex(const Vertex_2D &vertex_);
    void replace_vertex(uint64_t index, const Vertex_2D &new_vertex);
    void set_vertices(const std::vector<Vertex_2D> &vertices_);
    
    void add_texture(const std::shared_ptr<Texture> &texture_) override;
    void replace_texture(uint64_t index, const std::shared_ptr<Texture> &texture_) override;
    void set_textures(const std::vector<std::shared_ptr<Texture>> &textures_) override;
    
    void set_texture_coord(uint64_t index, const Vector_2f &tex_coords_) override;
    void set_texture_coords(const std::vector<Vector_2f> &tex_coords_) override;
    void set_model_transform(const Transform &model_transform_) override;
    void set_model_matrix(const Matrix_4f &mat) override;
    
    void normalize() override;
    void update_model_matrix() override;
    
    [[nodiscard]] Object_2D &operator=(const Object_2D &obj);
    bool operator==(const Object_2D &obj) const;
    protected:
    Object_type_e object_type_e = Object_type_e::Object_2D;
    uint64_t vertex_size = sizeof(Vertex_2D);
    Vector_2f origin{0.0f};
    std::vector<Vertex_2D> vertices;
    std::vector<Vector_2f> normals;
    std::vector<uint32_t> faces;
    std::vector<Vector_2f> tex_coords;
    std::vector<std::shared_ptr<Texture>> textures;
    Matrix_4f model_matrix = Matrix_4f(1.0f);
    Transform model_transform{};
    bool sent = false;
    protected:
    void apply_vertex_properties(const Object_2D_data_s &sprite);
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
  
  class Object_3D : public Object
  {
    public:
    Object_3D() = default;
    Object_3D(const Object_3D &mesh);
    explicit Object_3D(const Object_3D_data_s &mesh);
    ~Object_3D() override;
    
    // Interfaces.
    INTERFACE_SENDABLE
    
    INTERFACE_PRINTABLE
    INTERFACE_COPIABLE
    INTERFACE_MOVABLE
    INTERFACE_ROTATABLE
    INTERFACE_SCALABLE
    
    [[nodiscard]] Object_type_e get_type() const;
    [[nodiscard]] const Vector_3f &get_position() const;
    [[nodiscard]] const void *get_vertices() const override;
    [[nodiscard]] const Vertex_3D &get_vertex(uint64_t index) const;
    [[nodiscard]] uint64_t get_vertex_count() const override;
    [[nodiscard]] uint64_t get_vertex_size() const override;
    [[nodiscard]] const void *get_faces() const override;
    [[nodiscard]] uint64_t get_face_count() const override;
    [[nodiscard]] const std::vector<Vector_3f> &get_normals() const;
    [[nodiscard]] const std::vector<Vector_2f> &get_tex_coords() const;
    [[nodiscard]] const std::vector<std::shared_ptr<Texture>> &get_textures() const override;
    [[nodiscard]] uint64_t get_texture_count() const override;
    [[nodiscard]] const Matrix_4f &get_model_matrix() const override;
    [[nodiscard]] const Transform &get_model_transform() const override;
    
    void set_origin(const Vector_3f &origin_);
    void set_color(const Color &color) override;
    void set_position(const Vector_3f &position);
    void reset_position();
    
    void add_face(uint32_t face);
    void replace_face(uint64_t index, uint32_t face_);
    void set_faces(const std::vector<uint32_t> &faces_);
    
    void set_normal(uint64_t index, const Vector_3f &normal_);
    void set_normals(const std::vector<Vector_3f> &normal_);
    
    void add_vertex(const Vertex_3D &vertex_);
    void replace_vertex(uint64_t index, const Vertex_3D &new_vertex);
    void set_vertices(const std::vector<Vertex_3D> &vertices_);
    
    void add_texture(const std::shared_ptr<Texture> &texture_) override;
    void replace_texture(uint64_t index, const std::shared_ptr<Texture> &texture_) override;
    void set_textures(const std::vector<std::shared_ptr<Texture>> &textures_) override;
    
    void set_texture_coord(uint64_t index, const Vector_2f &tex_coords_) override;
    void set_texture_coords(const std::vector<Vector_2f> &tex_coords_) override;
    void set_model_transform(const Transform &model_transform_) override;
    void set_model_matrix(const Matrix_4f &mat) override;
    
    void convert_in_2D();
    void normalize() override;
    void update_model_matrix() override;
    
    explicit operator Object_2D() const;
    [[nodiscard]] Object_3D &operator=(const Object_3D &obj);
    bool operator==(const Object_3D &obj) const;
    protected:
    Object_type_e object_type_e = Object_type_e::Object_3D;
    uint64_t vertex_size = sizeof(Vertex_3D);
    Vector_3f origin{0.0f};
    std::vector<Vertex_3D> vertices;
    std::vector<Vector_3f> normals;
    std::vector<uint32_t> faces;
    std::vector<Vector_2f> tex_coords;
    std::vector<std::shared_ptr<Texture>> textures;
    Matrix_4f model_matrix = Matrix_4f(1.0f);
    Transform model_transform{};
    bool sent = false;
    protected:
    void apply_vertex_properties(const Object_3D_data_s &mesh);
  };
  
  /************************************** 3D OBJECTS *************************************/
  
  class Cube : public Object_3D
  {
    public:
    explicit Cube(const Object_3D_data_s &mesh,
                  const Vector_3f &scale = Vector_3f(1.0f), const Color &color = Color());
    ~Cube() override = default;
    
    bool operator==(const Object &other_object);
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
