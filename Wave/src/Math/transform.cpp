//
// Created by nami on 5/19/22.
//

#include <Math/transform.h>

namespace Wave::Math
{
  
  Transform::Transform()
  {
    this->translation = Vector_3f(0, 0, 0);
    this->rotation = Vector_3f(0, 0, 0);
    this->scale = Vector_3f(1, 1, 1);
  }
  
  Transform::Transform(const Vector_3f &translation_, const Vector_3f &rotation_, const Vector_3f &scale_)
  {
    this->translation = translation_;
    this->rotation = rotation_;
    this->scale = scale_;
  }
  
  const Vector_3f &Transform::get_translation() const
  {
    return this->translation;
  }
  
  Vector_3f &Transform::get_translation()
  {
    return this->translation;
  }
  
  void Transform::set_translation(float x, float y, float z)
  {
    this->translation = Vector_3f(x, y, z);
  }
  
  void Transform::set_translation(const Vector_3f &translation_)
  {
    this->translation = translation_;
  }
  
  const Vector_3f &Transform::get_rotation() const
  {
    return this->rotation;
  }
  
  Vector_3f &Transform::get_rotation()
  {
    return this->rotation;
  }
  
  void Transform::set_rotation(float x, float y, float z)
  {
    this->rotation = Vector_3f(y, x, z) + get_rotation();
  }
  
  void Transform::set_rotation(const Vector_3f &rotation_)
  {
    this->rotation = Vector_3f(rotation_.get_y(), rotation_.get_x(), rotation_.get_z()) + get_rotation();
  }
  
  const Vector_3f &Transform::get_scale() const
  {
    return this->scale;
  }
  
  Vector_3f &Transform::get_scale()
  {
    return this->scale;
  }
  
  void Transform::set_scale(const Vector_3f &scale_)
  {
    this->scale = scale_;
  }
  
  void Transform::set_scale(float x, float y, float z)
  {
    this->scale = Vector_3f(x, y, z);
  }
  
  // Final matrix to apply to each vertex in the vertex shader.
  Matrix_4f Transform::get_transform_matrix() const
  {
    Matrix_4f rotation_matrix, translation_matrix, scale_matrix;
    rotation_matrix.init_rotation(this->rotation);
    translation_matrix.init_translation(this->translation);
    scale_matrix.init_scale(this->scale);
    
    return translation_matrix * (rotation_matrix * scale_matrix);
  }
  
  std::string Transform::to_string() const
  {
    std::string output("[Transform] :\n");
    
    output += "Translation vector --> " + this->translation.to_string();
    output += "Rotation vector --> " + this->rotation.to_string();
    output += "Scale vector --> " + this->scale.to_string();
    
    return output;
  }
  
  Transform Transform::operator+(const Transform &transform) const
  {
    return {this->translation + transform.translation,
            this->rotation + transform.rotation,
            this->scale + transform.scale};
  }
  
  Transform Transform::operator-(const Transform &transform) const
  {
    return {this->translation - transform.translation,
            this->rotation - transform.rotation,
            this->scale - transform.scale};
  }
  
  Transform Transform::operator*(const Transform &transform) const
  {
    return {this->translation * transform.translation,
            this->rotation * transform.rotation,
            this->scale * transform.scale};
  }
  
  Transform Transform::operator/(const Transform &transform) const
  {
    return {this->translation / transform.translation,
            this->rotation / transform.rotation,
            this->scale / transform.scale};
  }
  
  bool Transform::operator==(const Transform &trsf) const
  {
    if (this == &trsf) return true;
    return this->scale == trsf.scale && this->rotation == trsf.rotation && this->translation == trsf.translation;
  }
  
  Transform &Transform::operator=(const Transform &transform)
  {
    if (this == &transform) return *this;
    this->translation = transform.translation;
    this->rotation = transform.rotation;
    this->scale = transform.scale;
    return *this;
  }
  
  void Transform::operator+=(const Transform &transform)
  {
    *this = *this + transform;
  }
}
