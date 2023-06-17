//
// Created by nami on 07/01/23.
//

#include <Objects/object.h>
#include <Utilities/resource_loader.h>
#include <Renderer/renderer.h>

namespace Wave
{
  
  std::shared_ptr<Object> Object::create()
  {
    return Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/cube.obj"));
  }
  
  std::shared_ptr<Object> Object::create(const Object_2D_data_s &object_2D_data,
                                         const std::shared_ptr<Shader> &associated_shader_,
                                         int32_t id_)
  {
    return std::make_shared<Object_2D>(object_2D_data, associated_shader_, id_);
  }
  
  std::shared_ptr<Object> Object::create(const Object_3D_data_s &object_3D_data,
                                         const std::shared_ptr<Shader> &associated_shader_,
                                         int32_t id_)
  {
    return std::make_shared<Object_3D>(object_3D_data, associated_shader_, id_);
  }
  
  /*********************** OBJECT 2D***************************/
  
  Object_2D::Object_2D(const Object_2D &obj) : object_type_e(obj.object_type_e),
                                               vertices(obj.vertices),
                                               normals(obj.normals),
                                               faces(obj.faces),
                                               tex_coords(obj.tex_coords),
                                               textures(obj.textures),
                                               model_matrix(obj.model_matrix),
                                               model_transform(obj.model_transform)
  {
  }
  
  Object_2D::Object_2D(const Object_2D_data_s &object_2D_data, int32_t id_) : vertices(object_2D_data.vertices),
                                                                              normals(object_2D_data.normals),
                                                                              tex_coords(object_2D_data.tex_coords),
                                                                              textures(object_2D_data.textures)
  {
    this->id = id_;
    this->object_type_e = Object_type_e::Object_2D;
    this->associated_shader = Shader::create("Default Object 2D",
                                             Resource_loader::load_shader_source(
                                               "../Wave/res/Shaders/default_2D.vert"),
                                             Resource_loader::load_shader_source(
                                               "../Wave/res/Shaders/default_2D.frag"));
    
    prepare_vertices(object_2D_data);
  }
  
  Object_2D::Object_2D(const Object_2D_data_s &object_2D_data, const std::shared_ptr<Shader> &associated_shader_,
                       int32_t id_)
    : vertices(object_2D_data.vertices),
      normals(object_2D_data.normals),
      tex_coords(object_2D_data.tex_coords),
      textures(object_2D_data.textures),
      associated_shader(associated_shader_)
  {
    this->id = id_;
    this->object_type_e = Object_type_e::Object_2D;
    if (this->associated_shader == nullptr)
    {
      this->associated_shader = Shader::create("Default Object 2D",
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/default_2D.vert"),
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/default_2D.frag"));
    }
    
    prepare_vertices(object_2D_data);
  }
  
  Object_2D::~Object_2D()
  {
    Object_2D::free_gpu(1);
  }
  
  void Object_2D::send_gpu(uint64_t instance_count)
  {
    for (const auto &texture: this->textures) if (!texture->is_sent()) texture->send_gpu(instance_count);
    Renderer::send_object(*this->associated_shader, this->textures, this->get_vertices(), this->get_vertex_count(),
                          this->get_vertex_size(), this->get_faces(), this->get_face_count());
    this->sent = true;
  }
  
  void Object_2D::free_gpu(uint64_t instance_count)
  {
    for (const auto &texture: this->textures) texture->free_gpu(instance_count);
    this->sent = false;
  }
  
  void Object_2D::normalize()
  {
    //TODO
  }
  
  void *Object_2D::copy() const
  {
    std::shared_ptr<Object> copy = std::make_shared<Object_2D>(*this);
    return (void *) &(*copy);
  }
  
  Object_type_e Object_2D::get_type() const
  {
    return Object_type_e::Object_2D;
  }
  
  const void *Object_2D::get_vertices() const
  {
    return this->vertices.data();
  }
  
  const Vertex_2D &Object_2D::get_vertex(uint64_t index) const
  {
    if (index >= this->get_vertex_count() && !this->vertices.empty())
      throw std::out_of_range("Vertex index out of bounds!");
    if (!this->vertices.empty()) return this->vertices[index];
    return {};
  }
  
  uint64_t Object_2D::get_vertex_count() const
  {
    return this->vertices.size();
  }
  
  uint64_t Object_2D::get_vertex_size() const
  {
    return this->vertex_size;
  }
  
  const void *Object_2D::get_faces() const
  {
    return this->faces.data();
  }
  
  uint64_t Object_2D::get_face_count() const
  {
    return this->faces.size();
  }
  
  const std::vector<Vector_2f> &Object_2D::get_tex_coords() const
  {
    return this->tex_coords;
  }
  
  const std::vector<std::shared_ptr<Texture>> &Object_2D::get_textures() const
  {
    return this->textures;
  }
  
  uint64_t Object_2D::get_texture_count() const
  {
    return this->textures.size();
  }
  
  const Matrix_4f &Object_2D::get_model_matrix() const
  {
    return this->model_matrix;
  }
  
  const Transform &Object_2D::get_model_transform() const
  {
    return this->model_transform;
  }
  
  const std::shared_ptr<Shader> &Object_2D::get_shader() const
  {
    return this->associated_shader;
  }
  
  const Vector_2f &Object_2D::get_position() const
  {
    return this->origin;
  }
  
  void Object_2D::set_origin(const Vector_2f &origin_)
  {
    this->origin = origin_;
  }
  
  void Object_2D::set_color(const Color &color)
  {
    for (Vertex_2D &vertex: this->vertices) if (vertex.get_color() != Color(0xFFFFFF)) vertex.set_color(color);
  }
  
  void Object_2D::set_position(const Vector_3f &position_)
  {
    this->origin = (Vector_2f) position_;
  }
  
  void Object_2D::reset_position()
  {
    this->origin = Vector_2f(0.0f);
  }
  
  void Object_2D::add_vertex(const Vertex_2D &vertex_)
  {
    this->vertices.emplace_back(vertex_);
  }
  
  void Object_2D::replace_vertex(uint64_t index, const Vertex_2D &new_vertex)
  {
    if (index >= this->vertices.size()) return;
    this->vertices[index] = new_vertex;
  }
  
  void Object_2D::set_vertices(const std::vector<Vertex_2D> &vertices_)
  {
    if (vertices_.empty()) return;
    this->vertices = vertices_;
  }
  
  void Object_2D::add_face(uint32_t face)
  {
    this->faces.emplace_back(face);
  }
  
  void Object_2D::replace_face(uint64_t index, uint32_t face)
  {
    if (index >= this->faces.size()) return;
    this->faces[index] = face;
  }
  
  void Object_2D::set_faces(const std::vector<uint32_t> &faces_)
  {
    this->faces = faces_;
  }
  
  const std::vector<Vector_2f> &Object_2D::get_normals() const
  {
    return this->normals;
  }
  
  void Object_2D::set_normal(uint64_t index, const Vector_2f &normal_)
  {
    if (index <= this->normals.size()) this->normals[index] = normal_;
  }
  
  void Object_2D::set_normals([[maybe_unused]] const std::vector<Vector_2f> &normals_)
  {
    if (normals_.empty()) return;
    this->normals.reserve(normals_.size());
    
    for (int64_t i = 0; i < (int64_t) normals_.size(); ++i) this->normals[i] = normals_[i];
  }
  
  void Object_2D::add_texture(const std::shared_ptr<Texture> &texture_)
  {
    this->textures.emplace_back(texture_);
  }
  
  void Object_2D::replace_texture(uint64_t index, const std::shared_ptr<Texture> &texture_)
  {
    if (index >= this->textures.size()) return;
    this->textures[index] = texture_;
  }
  
  void Object_2D::set_textures(const std::vector<std::shared_ptr<Texture>> &textures_)
  {
    this->textures = textures_;
  }
  
  void Object_2D::set_texture_coord(uint64_t index, const Vector_2f &tex_coords_)
  {
    if (index >= this->vertices.size()) return;
    this->vertices[index].set_tex_coord(tex_coords_);
  }
  
  void Object_2D::set_texture_coords(const std::vector<Vector_2f> &tex_coords_)
  {
    if (tex_coords_.empty()) return;
    for (uint64_t i = 0; i < this->vertices.size(); ++i) this->vertices[i].set_tex_coord(tex_coords_[i]);
  }
  
  void Object_2D::set_model_transform(const Transform &model_transform_)
  {
    this->model_transform = model_transform_;
    this->update_model_matrix();
  }
  
  void Object_2D::rotate(const Vector_3f &rotation_)
  {
    this->model_matrix.init_rotation(rotation_);
    this->update_model_matrix();
  }
  
  void Object_2D::rotate(float x, float y, float z)
  {
    this->model_matrix.init_rotation(x, y, z);
    this->update_model_matrix();
  }
  
  void Object_2D::translate(const Vector_3f &translation_)
  {
    this->model_matrix.init_translation(translation_);
    this->update_model_matrix();
  }
  
  void Object_2D::translate(float x, float y, float z)
  {
    this->model_matrix.init_translation(x, y, z);
    this->update_model_matrix();
  }
  
  void Object_2D::scale(const Vector_3f &scalar_)
  {
    this->model_matrix.init_scale(scalar_);
    this->update_model_matrix();
  }
  
  void Object_2D::scale(float x, float y, float z)
  {
    this->model_matrix.init_scale(x, y, z);
    this->update_model_matrix();
  }
  
  void Object_2D::set_model_matrix(const Matrix_4f &mat)
  {
    this->model_matrix = mat;
    Object_2D::update_model_matrix();
  }
  
  void Object_2D::update_model_matrix()
  {
  }
  
  void Object_2D::prepare_vertices(const Object_2D_data_s &sprite)
  {
    for (const Face_2D_s &face: sprite.indices)
    {
      add_face(face.first_vertex_index);
      add_face(face.second_vertex_index);
      
      replace_vertex(face.first_vertex_index,
                     Vertex_2D(this->vertices[face.first_vertex_index].get_position() + sprite.origin,
                               this->normals[face.first_normal_index] + sprite.origin,
                               this->vertices[face.first_vertex_index].get_color(),
                               this->tex_coords[face.first_texture_index]));
      
      this->vertices[face.first_vertex_index].set_id(this->id);
      
      replace_vertex(face.second_vertex_index,
                     Vertex_2D(this->vertices[face.second_vertex_index].get_position() + sprite.origin,
                               this->normals[face.second_normal_index] + sprite.origin,
                               this->vertices[face.second_vertex_index].get_color(),
                               this->tex_coords[face.second_texture_index]));
      
      this->vertices[face.second_vertex_index].set_id(this->id);
    }
  }
  
  std::string Object_2D::to_string() const
  {
    std::string output;
    output.reserve(this->vertices.size() + this->textures.size());
    output = "[Object 2D] :\n";
    
    output += "Object type --> Object 2D\n";
    
    char *big_buffer = (char *) calloc(this->vertices.size() + this->textures.size(), sizeof(char));
    for (unsigned int i = 0; i < this->vertices.size(); ++i)
    {
      if (snprintf(big_buffer, this->vertices.size() + this->textures.size(), "%50s[%d] = %s", DEFAULT, i,
                   this->vertices[i].to_string().c_str()) < 0)
      {
        output += "ERROR : Snprintf error when trying to display [Object_2D]!";
        continue;
      }
      output += big_buffer;
    }
    output += "\n";
    
    for (unsigned int i = 0; i < this->textures.size(); ++i)
    {
      if (snprintf(big_buffer, this->vertices.size() + this->textures.size(), "%50s[%d] = %s", DEFAULT, i,
                   this->textures[i]->to_string().c_str()) < 0)
      {
        output += "ERROR : Snprintf error when trying to display [Object_2D]!";
        continue;
      }
      output += big_buffer;
    }
    return output;
  }
  
  bool Object_2D::operator==(const Object_2D &obj) const
  {
    if (this == &obj) return true;
    return this->object_type_e == obj.object_type_e && this->model_transform == obj.model_transform
           && this->model_matrix == obj.model_matrix;
  }
  
  Object_2D &Object_2D::operator=(const Object_2D &obj)
  {
    if (this == &obj) return *this;
    if (*this == obj) return *this;
    this->object_type_e = obj.object_type_e;
    this->origin = obj.origin;
    this->vertices = obj.vertices;
    this->faces = obj.faces;
    if (!this->tex_coords.empty()) this->tex_coords = obj.tex_coords;
    this->textures = obj.textures;
    this->model_transform = obj.model_transform;
    this->model_matrix = obj.model_matrix;
    return *this;
  }
  
  void Object_2D::calculate_average_normals()
  {
    for (size_t i = 0; i < this->get_face_count(); i += 3)
    {
      unsigned int in0 = this->faces[i];
      unsigned int in1 = this->faces[i + 1];
      unsigned int in2 = this->faces[i + 2];
      Vector_2f v1(this->vertices[in1].get_position().get_x() - this->vertices[in0].get_position().get_x(),
                   this->vertices[in1].get_position().get_y() - this->vertices[in0].get_position().get_y());
      
      Vector_2f v2(this->vertices[in2].get_position().get_x() - this->vertices[in0].get_position().get_x(),
                   this->vertices[in2].get_position().get_y() - this->vertices[in0].get_position().get_y());
      Vector_2f normal = v1.cross(v2);
      normal = normal.normalize();
      
      this->vertices[in0].set_normal(this->vertices[in0].get_normal() + normal);
      this->vertices[in1].set_normal(this->vertices[in1].get_normal() + normal);
      this->vertices[in2].set_normal(this->vertices[in2].get_normal() + normal);
    }
    
    for (auto &vertex: this->vertices)
    {
      Vector_2f vec(vertex.get_normal());
      vec = vec.normalize();
      vertex.set_normal(vec);
    }
  }
  
  void Object_2D::calculate_effect_by_light(const Light &light_source)
  {
    switch (light_source.get_type())
    {
      case Light::Light_type_e::Point_light:
      {
        auto point_light = dynamic_cast<const Point_light &>(light_source);
        
        this->associated_shader->bind();
        
        this->associated_shader->set_uniform("u_point_light_count", (int) point_light.get_count());
        
        this->associated_shader->set_uniform("u_point_lights[0].base.color", point_light.get_color());
        this->associated_shader->set_uniform("u_point_lights[0].base.ambient_intensity",
                                             point_light.get_ambient_intensity());
        this->associated_shader->set_uniform("u_point_lights[0].base.diffuse_intensity",
                                             point_light.get_diffuse_intensity());
        
        this->associated_shader->set_uniform("u_point_lights[0].position", point_light.get_position());
        this->associated_shader->set_uniform("u_point_lights[0].constant", point_light.get_constant());
        this->associated_shader->set_uniform("u_point_lights[0].linear", point_light.get_linear());
        this->associated_shader->set_uniform("u_point_lights[0].exponent", point_light.get_exponent());
        break;
      }
      default: break;
    }
  }
  
  /****************************** 3D ********************************/
  
  Object_3D::Object_3D(const Object_3D_data_s &mesh, int32_t id_) : vertices(mesh.vertices), normals(mesh.normals),
                                                                    tex_coords(mesh.tex_coords)
  {
    this->id = id_;
    this->object_type_e = Object_type_e::Object_3D;
    this->associated_shader = Shader::create("Default Object 3D",
                                             Wave::Resource_loader::load_shader_source(
                                               "../Wave/res/Shaders/default_3D.vert"),
                                             Wave::Resource_loader::load_shader_source(
                                               "../Wave/res/Shaders/default_3D.frag"));
    
    prepare_vertices(mesh);
  }
  
  Object_3D::Object_3D(const Wave::Object_3D_data_s &mesh_data, const std::shared_ptr<Shader> &associated_shader_,
                       int32_t id_) :
    vertices(mesh_data.vertices),
    normals(mesh_data.normals),
    tex_coords(mesh_data.tex_coords),
    textures(mesh_data.textures),
    associated_shader(associated_shader_)
  {
    this->id = id_;
    this->object_type_e = Object_type_e::Object_3D;
    if (this->associated_shader == nullptr)
    {
      this->associated_shader = Shader::create("Default Object 3D",
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/default_3D.vert"),
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/default_3D.frag"));
    }
    
    prepare_vertices(mesh_data);
  }
  
  Object_3D::Object_3D(const Object_3D &mesh) : object_type_e(mesh.object_type_e),
                                                vertices(mesh.vertices),
                                                normals(mesh.normals),
                                                faces(mesh.faces),
                                                tex_coords(mesh.tex_coords),
                                                textures(mesh.textures),
                                                model_matrix(mesh.model_matrix),
                                                model_transform(mesh.model_transform),
                                                associated_shader(mesh.associated_shader)
  {
  }
  
  Object_3D::~Object_3D()
  {
    Object_3D::free_gpu(1);
  }
  
  void Object_3D::send_gpu(uint64_t instance_count)
  {
    for (const auto &texture: this->textures) texture->send_gpu(instance_count);
    Renderer::send_object(*this->associated_shader, this->textures, this->get_vertices(), this->get_vertex_count(),
                          this->get_vertex_size(), this->get_faces(), this->get_face_count());
    this->sent = true;
  }
  
  void Object_3D::free_gpu(uint64_t instance_count)
  {
    for (const auto &texture: this->textures) texture->free_gpu(instance_count);
    this->sent = false;
  }
  
  void *Object_3D::copy() const
  {
    std::shared_ptr<Object> copy = std::make_shared<Object_3D>(*this);
    return (void *) &(*copy);
  }
  
  void Object_3D::convert_in_2D()
  {
    this->set_position({this->origin.get_x(),
                        this->origin.get_y(),
                        0});
    for (int i = 0; i < (int) this->get_vertex_count(); ++i)
    {
      this->vertices[i].convert_in_2D();
      this->replace_vertex(i, this->vertices[i]);
    }
    this->set_faces(this->faces);
    this->set_texture_coords(this->tex_coords);
    this->set_textures(this->textures);
    this->set_model_transform(this->model_transform);
  }
  
  void Object_3D::normalize()
  {
    //TODO
  }
  
  Object_type_e Object_3D::get_type() const
  {
    return this->object_type_e;
  }
  
  const void *Object_3D::get_vertices() const
  {
    return this->vertices.data();
  }
  
  const Vertex_3D &Object_3D::get_vertex(uint64_t index) const
  {
    if (index >= this->get_vertex_count()) throw std::out_of_range("Index for vertex is out of bounds!");
    return this->vertices[index];
  }
  
  uint64_t Object_3D::get_vertex_count() const
  {
    return this->vertices.size();
  }
  
  uint64_t Object_3D::get_vertex_size() const
  {
    return this->vertex_size;
  }
  
  const void *Object_3D::get_faces() const
  {
    return this->faces.data();
  }
  
  uint64_t Object_3D::get_face_count() const
  {
    return this->faces.size();
  }
  
  const std::vector<Vector_2f> &Object_3D::get_tex_coords() const
  {
    return this->tex_coords;
  }
  
  const std::vector<std::shared_ptr<Texture>> &Object_3D::get_textures() const
  {
    return this->textures;
  }
  
  uint64_t Object_3D::get_texture_count() const
  {
    return this->textures.size();
  }
  
  const Matrix_4f &Object_3D::get_model_matrix() const
  {
    return this->model_matrix;
  }
  
  const Transform &Object_3D::get_model_transform() const
  {
    return this->model_transform;
  }
  
  const std::shared_ptr<Shader> &Object_3D::get_shader() const
  {
    return this->associated_shader;
  }
  
  const Vector_3f &Object_3D::get_position() const
  {
    return this->origin;
  }
  
  void Object_3D::set_origin(const Vector_3f &origin_)
  {
    this->origin = origin_;
  }
  
  void Object_3D::set_color(const Color &color)
  {
    for (Vertex_3D &vertex: this->vertices) if (vertex.get_color() != Color(0xFFFFFF)) vertex.set_color(color);
  }
  
  void Object_3D::set_position(const Vector_3f &position_)
  {
    this->origin = position_;
  }
  
  void Object_3D::reset_position()
  {
    this->origin = Vector_3f(0, 0, 0);
  }
  
  void Object_3D::add_vertex(const Vertex_3D &vertex_)
  {
    this->vertices.emplace_back(vertex_);
  }
  
  void Object_3D::replace_vertex(uint64_t index, const Vertex_3D &new_vertex)
  {
    if (index >= this->vertices.size()) return;
    this->vertices[index] = new_vertex;
  }
  
  void Object_3D::set_vertices(const std::vector<Vertex_3D> &vertices_)
  {
    this->vertices = vertices_;
  }
  
  void Object_3D::add_face(uint32_t face)
  {
    this->faces.emplace_back(face);
  }
  
  void Object_3D::replace_face(uint64_t index, uint32_t face)
  {
    if (index >= this->faces.size()) return;
    this->faces[index] = face;
  }
  
  void Object_3D::set_faces(const std::vector<uint32_t> &faces_)
  {
    this->faces = faces_;
  }
  
  void Object_3D::add_texture(const std::shared_ptr<Texture> &texture_)
  {
    this->textures.emplace_back(texture_);
  }
  
  void Object_3D::replace_texture(uint64_t index, const std::shared_ptr<Texture> &texture_)
  {
    if (index >= this->textures.size()) return;
    this->textures[index] = texture_;
  }
  
  void Object_3D::set_textures(const std::vector<std::shared_ptr<Texture>> &textures_)
  {
    this->textures = textures_;
  }
  
  const std::vector<Vector_3f> &Object_3D::get_normals() const
  {
    return this->normals;
  }
  
  void Object_3D::set_normal(uint64_t index, const Vector_3f &normal)
  {
    if (index >= this->vertices.size()) return;
    this->vertices[index].set_normal(normal);
  }
  
  void Object_3D::set_normals(const std::vector<Vector_3f> &normals_)
  {
    for (uint64_t i = 0; i < this->vertices.size(); ++i) this->vertices[i].set_normal(normals_[i]);
  }
  
  void Object_3D::set_texture_coord(uint64_t index, const Vector_2f &tex_coords_)
  {
    if (index >= this->vertices.size()) return;
    this->vertices[index].set_tex_coord(tex_coords_);
  }
  
  void Object_3D::set_texture_coords(const std::vector<Vector_2f> &tex_coords_)
  {
    for (uint64_t i = 0; i < this->vertices.size(); ++i) this->vertices[i].set_tex_coord(tex_coords_[i]);
  }
  
  void Object_3D::set_model_transform(const Transform &model_transform_)
  {
    this->model_transform = model_transform_;
    this->update_model_matrix();
  }
  
  void Object_3D::rotate(const Vector_3f &angle)
  {
    this->model_transform.set_rotation(angle);
    this->update_model_matrix();
  }
  
  void Object_3D::rotate(float x, float y, float z)
  {
    this->model_transform.set_rotation(x, y, z);
    this->update_model_matrix();
  }
  
  void Object_3D::translate(const Vector_3f &translation_)
  {
    this->model_transform.set_translation(translation_ + this->origin);
    this->update_model_matrix();
  }
  
  void Object_3D::translate(float x, float y, float z)
  {
    this->model_transform.set_translation(Vector_3f(x, y, z) + this->origin);
    this->update_model_matrix();
  }
  
  void Object_3D::scale(const Vector_3f &scalar_)
  {
    this->model_transform.set_scale(scalar_);
    this->update_model_matrix();
  }
  
  void Object_3D::scale(float x, float y, float z)
  {
    this->model_transform.set_scale(x, y, z);
    this->update_model_matrix();
  }
  
  void Object_3D::set_model_matrix(const Matrix_4f &mat)
  {
    this->model_matrix = mat;
  }
  
  void Object_3D::update_model_matrix()
  {
    this->model_matrix = this->model_transform.get_transform_matrix();
    this->model_matrix.transpose();
    
    for (Vertex_3D &vertex: this->vertices) vertex.set_model_matrix(this->model_matrix);
  }
  
  void Object_3D::prepare_vertices(const Object_3D_data_s &mesh)
  {
    for (const Face_3D_s &face: mesh.indices)
    {
      add_face(face.first_vertex_index);
      add_face(face.second_vertex_index);
      add_face(face.third_vertex_index);
      
      replace_vertex(face.first_vertex_index,
                     Vertex_3D(this->vertices[face.first_vertex_index].get_position() + mesh.origin,
                               this->vertices[face.first_vertex_index].get_color(),
                               this->normals[face.first_normal_index] + mesh.origin,
                               this->tex_coords[face.first_texture_index]));
      
      this->vertices[face.first_vertex_index].set_id(this->id);
      
      replace_vertex(face.second_vertex_index,
                     Vertex_3D(this->vertices[face.second_vertex_index].get_position() + mesh.origin,
                               this->vertices[face.second_vertex_index].get_color(),
                               this->normals[face.second_normal_index] + mesh.origin,
                               this->tex_coords[face.second_texture_index]));
      
      this->vertices[face.second_vertex_index].set_id(this->id);
      
      replace_vertex(face.third_vertex_index,
                     Vertex_3D(this->vertices[face.third_vertex_index].get_position() + mesh.origin,
                               this->vertices[face.third_vertex_index].get_color(),
                               this->normals[face.third_normal_index] + mesh.origin,
                               this->tex_coords[face.third_texture_index]));
      
      this->vertices[face.third_vertex_index].set_id(this->id);
    }
  }
  
  std::string Object_3D::to_string() const
  {
    int64_t max_size = UINT8_MAX * 4;
    int64_t index = 0;
    auto data_size = static_cast<int64_t>(this->vertices.size() + this->textures.size() - 2);
    if (max_size > data_size) max_size = data_size;
    
    std::string output;
    output.reserve(max_size);
    char *big_buffer = (char *) calloc(max_size * UINT16_MAX, sizeof(char));
    
    output = "[Object 3D] : \n";
    for (const Vertex_3D &vertex: this->vertices)
    {
      if (snprintf(big_buffer, max_size * UINT16_MAX, "%50s[%ld] = %s", DEFAULT, index,
                   vertex.to_string().c_str()) < 0)
      {
        output += "ERROR : Snprintf error when trying to display [Object_3D]!";
        index++;
        continue;
      }
      output += big_buffer;
      index++;
    }
    
    for (const auto &texture: this->textures)
    {
      if (snprintf(big_buffer, max_size * UINT16_MAX, "%50s[%ld] = %s", DEFAULT, index,
                   texture->to_string().c_str()) < 0)
      {
        output += "ERROR : Snprintf error when trying to display [Object_3D]!";
        index++;
        continue;
      }
      output += big_buffer;
      index++;
    }
    free(big_buffer);
    return output;
  }
  
  Object_3D::operator Object_2D() const
  {
    Object_2D object;
    object.set_position({this->origin.get_x(),
                         this->origin.get_y(),
                         0});
    for (const Vertex_3D &vertex: this->vertices) object.add_vertex((Vertex_2D) vertex);
    object.set_faces(this->faces);
    object.set_texture_coords(this->tex_coords);
    if (!object.get_textures().empty()) object.set_textures(this->textures);
    object.set_model_transform(this->model_transform);
    return object;
  }
  
  Object_3D &Object_3D::operator=(const Object_3D &obj)
  {
    if (this == &obj) return *this;
    this->origin = obj.origin;
    this->vertices = obj.vertices;
    this->faces = obj.faces;
    this->normals = obj.normals;
    this->tex_coords = obj.tex_coords;
    this->textures = obj.textures;
    this->model_transform = obj.model_transform;
    this->model_matrix = obj.model_matrix;
    return *this;
  }
  
  bool Object_3D::operator==(const Object_3D &obj) const
  {
    if (this == &obj) return true;
    return this->model_transform == obj.model_transform && this->model_matrix == obj.model_matrix;
  }
  
  void Object_3D::calculate_average_normals()
  {
    for (size_t i = 0; i < this->get_face_count(); i += 3)
    {
      unsigned int in0 = this->faces[i];
      unsigned int in1 = this->faces[i + 1];
      unsigned int in2 = this->faces[i + 2];
      Vector_3f v1(this->vertices[in1].get_position().get_x() - this->vertices[in0].get_position().get_x(),
                   this->vertices[in1].get_position().get_y() - this->vertices[in0].get_position().get_y(),
                   this->vertices[in1].get_position().get_z() - this->vertices[in0].get_position().get_z());
      
      Vector_3f v2(this->vertices[in2].get_position().get_x() - this->vertices[in0].get_position().get_x(),
                   this->vertices[in2].get_position().get_y() - this->vertices[in0].get_position().get_y(),
                   this->vertices[in2].get_position().get_z() - this->vertices[in0].get_position().get_z());
      Vector_3f normal = v1.cross(v2);
      normal = normal.normalize();
      
      this->vertices[in0].set_normal(this->vertices[in0].get_normal() + normal);
      this->vertices[in1].set_normal(this->vertices[in1].get_normal() + normal);
      this->vertices[in2].set_normal(this->vertices[in2].get_normal() + normal);
    }
    
    for (auto &vertex: this->vertices)
    {
      Vector_3f vec(vertex.get_normal());
      vec = vec.normalize();
      vertex.set_normal(vec);
    }
  }
  
  void Object_3D::calculate_effect_by_light(const Light &light_source)
  {
    switch (light_source.get_type())
    {
      case Light::Light_type_e::Point_light:
      {
        auto point_light = dynamic_cast<const Point_light &>(light_source);
        
        this->associated_shader->bind();
        
        this->associated_shader->set_uniform("u_point_light_count", (int) point_light.get_count());
        
        this->associated_shader->set_uniform("u_point_lights[0].base.color", point_light.get_color());
        this->associated_shader->set_uniform("u_point_lights[0].base.ambient_intensity",
                                             point_light.get_ambient_intensity());
        this->associated_shader->set_uniform("u_point_lights[0].base.diffuse_intensity",
                                             point_light.get_diffuse_intensity());
        
        this->associated_shader->set_uniform("u_point_lights[0].position", point_light.get_position());
        this->associated_shader->set_uniform("u_point_lights[0].constant", point_light.get_constant());
        this->associated_shader->set_uniform("u_point_lights[0].linear", point_light.get_linear());
        this->associated_shader->set_uniform("u_point_lights[0].exponent", point_light.get_exponent());
        break;
      }
      default: break;
    }
  }
  
  
  /******************** CUBE *************************/
  
  Cube::Cube(const Vector_3f &scale, const Color &color, int32_t id_)
  {
    this->id = id_;
    this->origin = Vector_3f(0.0f);
//    float vertices[] = {
//      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//
//      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//      0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//
//      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//
//      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//
//      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//
//      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//    };
  }
  
  Cube::Cube(const Object_3D_data_s &mesh, const Vector_3f &scale_, const Color &color, int32_t id_) :
    Object_3D(mesh, id_)
  {
    this->scale(scale_);
    this->set_color(color);
  }
  
  bool Cube::operator==(const Object &other_object)
  {
    return this->model_transform == other_object.get_model_transform() &&
           this->model_matrix == other_object.get_model_matrix();
  }
}
