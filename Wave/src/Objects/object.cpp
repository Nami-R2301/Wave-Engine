//
// Created by nami on 07/01/23.
//

#include <Objects/object.h>
#include <Utilities/resource_loader.h>

namespace Wave
{
  
  std::shared_ptr<Object> Object::create()
  {
    return Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/cube.obj"));
  }
  
  std::shared_ptr<Object> Object::create(const Object_2D_data_s &object_2D_data)
  {
    return std::make_shared<Object_2D>(object_2D_data);
  }
  
  std::shared_ptr<Object> Object::create(const Object_3D_data_s &object_3D_data)
  {
    return std::make_shared<Object_3D>(object_3D_data);
  }
  
  /*********************** OBJECT 2D***************************/
  
  Object_2D::Object_2D(const Object_2D &obj)
  {
    this->vertices = obj.vertices;
    this->faces = obj.faces;
    this->tex_coords = obj.tex_coords;
    this->textures = obj.textures;
    this->model_matrix = obj.model_matrix;
    this->model_transform = obj.model_transform;
  }
  
  Object_2D::Object_2D(const Object_2D_data_s &object_2D_data)
  {
    for (const Vertex_2D &vertex: object_2D_data.vertices)
    {
      this->vertices.emplace_back(Vector_2f(vertex.get_position().get_x(), vertex.get_position().get_y()),
                                  vertex.get_color(), vertex.get_tex_coord());
    }
    this->tex_coords = object_2D_data.tex_coords;
    this->textures = object_2D_data.textures;
  }
  
  Object_2D Object_2D::clone() const
  {
    return {*this};
  }
  
  void Object_2D::normalize()
  {
    //TODO
  }
  
  Object_type_e Object_2D::get_type()
  {
    return Object_type_e::Object_2D;
  }
  
  const void *Object_2D::get_vertices() const
  {
    return this->vertices.data();
  }
  
  const Vertex_2D &Object_2D::get_vertex(uint64_t index) const
  {
    if (index >= this->get_vertex_count() && !this->vertices.empty()) return this->vertices[0];
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
  
  void Object_2D::move(const Vector_3f &position_)
  {
    this->set_position({position_.get_x(),
                        position_.get_y(),
                        0});
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
  
  void Object_2D::apply_vertex_properties(const Object_2D_data_s &sprite)
  {
    for (const Face_2D_s &face: sprite.indices)
    {
      add_face(face.first_vertex_index);
      add_face(face.second_vertex_index);
      
      replace_vertex(face.first_vertex_index,
                     Vertex_2D(
                       this->vertices[face.first_vertex_index].get_position() + sprite.origin,
                       this->normals[face.first_normal_index] + sprite.origin,
                       this->vertices[face.first_vertex_index].get_color(),
                       this->tex_coords[face.first_texture_index]));
      
      replace_vertex(face.second_vertex_index,
                     Vertex_2D(
                       this->vertices[face.second_vertex_index].get_position() + sprite.origin,
                       this->normals[face.second_normal_index] + sprite.origin,
                       this->vertices[face.second_vertex_index].get_color(),
                       this->tex_coords[face.second_texture_index]));
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
  
  /****************************** 3D ********************************/
  
  Object_3D::Object_3D(const Object_3D_data_s &mesh)
  {
    this->object_type_e = Object_type_e::Object_3D;
    this->vertices = mesh.vertices;
    this->tex_coords = mesh.tex_coords;
    this->normals = mesh.normals;
    
    apply_vertex_properties(mesh);
  }
  
  Object_3D::Object_3D(const Object_3D &mesh)
  {
    this->object_type_e = Object_type_e::Object_3D;
    this->vertices = mesh.vertices;
    this->faces = mesh.faces;
    this->normals = mesh.normals;
    this->tex_coords = mesh.tex_coords;
    this->model_matrix = mesh.model_matrix;
    this->model_transform = mesh.model_transform;
  }
  
  Object_3D Object_3D::clone() const
  {
    return {*this};
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
  
  Object_type_e Object_3D::get_type()
  {
    return Object_type_e::Object_3D;
  }
  
  const void *Object_3D::get_vertices() const
  {
    return this->vertices.data();
  }
  
  const Vertex_3D &Object_3D::get_vertex(uint64_t index) const
  {
    if (index >= this->get_vertex_count()) return {};
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
  
  void Object_3D::rotate(const Vector_3f &rotation_)
  {
    this->model_transform.set_rotation(rotation_);
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
  
  void Object_3D::move(const Vector_3f &position_)
  {
    this->translate(position_ - this->origin);
  }
  
  void Object_3D::set_model_matrix(const Matrix_4f &mat)
  {
    this->model_matrix = mat;
  }
  
  void Object_3D::update_model_matrix()
  {
    this->model_matrix = this->model_transform.get_transform_matrix();
    this->model_matrix.transpose();
  }
  
  void Object_3D::apply_vertex_properties(const Object_3D_data_s &mesh)
  {
    for (const Face_3D_s &face: mesh.indices)
    {
      add_face(face.first_vertex_index);
      add_face(face.second_vertex_index);
      add_face(face.third_vertex_index);
      
      replace_vertex(face.first_vertex_index,
                     Vertex_3D(
                       this->vertices[face.first_vertex_index].get_position() + mesh.origin,
                       this->vertices[face.first_vertex_index].get_color(),
                       this->normals[face.first_normal_index] + mesh.origin,
                       this->tex_coords[face.first_texture_index]));
      
      replace_vertex(face.second_vertex_index,
                     Vertex_3D(
                       this->vertices[face.second_vertex_index].get_position() + mesh.origin,
                       this->vertices[face.second_vertex_index].get_color(),
                       this->normals[face.second_normal_index] + mesh.origin,
                       this->tex_coords[face.second_texture_index]));
      
      replace_vertex(face.third_vertex_index,
                     Vertex_3D(
                       this->vertices[face.third_vertex_index].get_position() + mesh.origin,
                       this->vertices[face.third_vertex_index].get_color(),
                       this->normals[face.third_normal_index] + mesh.origin,
                       this->tex_coords[face.third_texture_index]));
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
  
  /******************** CUBE *************************/
  
  Cube::Cube(const Object_3D_data_s &mesh, const Vector_3f &scale_, const Color &color)
  {
    this->origin = mesh.origin;
    this->vertices = mesh.vertices;
    this->tex_coords = mesh.tex_coords;
    this->normals = mesh.normals;
    this->apply_vertex_properties(mesh);
    this->scale(scale_);
    this->set_color(color);
  }
  
  bool Cube::operator==(const Object &other_object)
  {
    return this->model_transform == other_object.get_model_transform() &&
           this->model_matrix == other_object.get_model_matrix();
  }
}
