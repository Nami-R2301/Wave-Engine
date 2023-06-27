//
// Created by nami on 5/13/22.
//

#include <Math/vector.h>

namespace Wave::Math
{
  
  Vector_3f::Vector_3f(float uniform_scalar)
  {
    this->x = uniform_scalar;
    this->y = uniform_scalar;
    this->z = uniform_scalar;
  }
  
  Vector_3f::Vector_3f(float x, float y, float z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  
  Vector_3f::Vector_3f(const Vector_2f &other_vector_2f)
  {
    this->x = other_vector_2f.get_x();
    this->y = other_vector_2f.get_y();
    this->z = 0;
  }
  
  float Vector_3f::get_x() const
  {
    return this->x;
  }
  
  float Vector_3f::get_y() const
  {
    return this->y;
  }
  
  float Vector_3f::get_z() const
  {
    return this->z;
  }
  
  void Vector_3f::set_x(float x_)
  {
    this->x = x_;
  }
  
  void Vector_3f::set_y(float y_)
  {
    this->y = y_;
  }
  
  void Vector_3f::set_z(float z_)
  {
    this->z = z_;
  }
  
  float Vector_3f::length() const
  {
    return std::sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
  }
  
  float Vector_3f::size() const
  {
    return sizeof(this->x) + sizeof(this->y) + sizeof(this->z);
  }
  
  float Vector_3f::dot(const Vector_3f &other_vector) const
  {
    return (this->x * other_vector.get_x()) +
           (this->y * other_vector.get_y()) +
           (this->z * other_vector.get_z());
  }
  
  Vector_3f Vector_3f::normalize(float max_dx, float max_dy, float max_dz) const
  {
    if (max_dx == 1.0f && max_dy == 1.0f && max_dz == 1.0f)
    {
      float length = this->length();
      return {this->x / length,
              this->y / length,
              this->z / length};
    }
    float factor = std::max(max_dx, max_dy);
    factor = std::max(factor, max_dz);
    return {(this->x - max_dx / 2) / factor * 2,
            (this->y - max_dy / 2) / factor * 2,
            (this->z - max_dz / 2) / factor * 2};
  }
  
  Vector_3f Vector_3f::cross(const Vector_3f &other_vector) const
  {
    float x_ = this->y * other_vector.get_z() - this->z * other_vector.get_y();
    float y_ = this->z * other_vector.get_x() - this->x * other_vector.get_z();
    float z_ = this->x * other_vector.get_y() - this->y * other_vector.get_x();
    return {x_,
            y_,
            z_};
  }
  
  Vector_3f Vector_3f::rotate(float angle, const Vector_3f &axis) const
  {
    auto angle_to_rad = (float) (angle * (M_PI / 180));
    float sin_half_angle = sinf(angle_to_rad / 2);
    float cos_half_angle = cosf(angle_to_rad / 2);
    
    float rotation_x = axis.get_x() * sin_half_angle, rotation_y = axis.get_y() * sin_half_angle,
      rotation_z = axis.get_z() * sin_half_angle, rotation_w = cos_half_angle;
    
    Vector_4f rotation(rotation_x, rotation_y, rotation_z, rotation_w);
    Vector_4f conjugate = rotation.conjugate();
    Vector_4f w = (rotation * *this) * conjugate;
    
    return {w.get_x(),
            w.get_y(),
            w.get_z()};
  }
  
  Vector_3f Vector_3f::absolute() const
  {
    return {std::abs(this->x),
            std::abs(this->y),
            std::abs(this->z)};
  }
  
  std::string Vector_3f::to_string() const
  {
    char buffer[29 + sizeof(float) * 3]{0};  // Three 8-digit-long float.
    
    if (snprintf(buffer, 29 + sizeof(float) * 3, "x : %.2f, y : %.2f, z : %.2f\n", this->x, this->y, this->z) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Vector_3f]!";
    }
    return buffer;
  }
  
  float &Vector_3f::operator[](unsigned int index)
  {
    if (index > 2) return this->x;
    return index == 0 ? this->x : index == 1 ? this->y : this->z;
  }
  
  // Add two vectors.
  Vector_3f Vector_3f::operator+(const Vector_3f &other_vector) const
  {
    return {this->x + other_vector.get_x(),
            this->y + other_vector.get_y(),
            this->z + other_vector.get_z()};
  }
  
  Vector_3f Vector_3f::operator-(const Vector_3f &other_vector) const
  {
    return {this->x - other_vector.get_x(),
            this->y - other_vector.get_y(),
            this->z - other_vector.get_z()};
  }
  
  Vector_3f Vector_3f::operator*(const Vector_3f &other_vector) const
  {
    return {this->x * other_vector.get_x(),
            this->y * other_vector.get_y(),
            this->z * other_vector.get_z()};
  }
  
  Vector_3f Vector_3f::operator/(const Vector_3f &other_vector) const
  {
    return {this->x / other_vector.get_x(),
            this->y / other_vector.get_y(),
            this->z / other_vector.get_z()};
  }
  
  Vector_3f Vector_3f::operator+(float coord) const
  {
    return {this->x + coord,
            this->y + coord,
            this->z + coord};
  }
  
  Vector_3f Vector_3f::operator-(float coord) const
  {
    return {this->x - coord,
            this->y - coord,
            this->z - coord};
  }
  
  Vector_3f Vector_3f::operator*(float coord) const
  {
    return {this->x * coord,
            this->y * coord,
            this->z * coord};
  }
  
  Vector_3f Vector_3f::operator/(float coord) const
  {
    return {this->x / coord,
            this->y / coord,
            this->z / coord};
  }
  
  bool Vector_3f::operator==(const Vector_3f &other_vector) const
  {
    if (this == &other_vector) return true;
    if (this->length() != other_vector.length()) return false;
    return (this->x == other_vector.get_x() &&
            this->y == other_vector.get_y() &&
            this->z == other_vector.get_z());
  }
  
  bool Vector_3f::operator!=(const Vector_3f &other_vector) const
  {
    if (this == &other_vector) return false;
    return this->x != other_vector.x && this->y != other_vector.y && this->z != other_vector.z;
  }
  
  Vector_3f &Vector_3f::operator=(const Vector_3f &other_vector)
  {
    if (this == &other_vector) return *this;
    this->x = other_vector.x;
    this->y = other_vector.y;
    this->z = other_vector.z;
    return *this;
  }
  
  void Vector_3f::operator+=(const Vector_3f &other_vector)
  {
    *this = *this + other_vector;
  }
  
  void Vector_3f::operator-=(const Vector_3f &other_vector)
  {
    *this = *this - other_vector;
  }
  
  void Vector_3f::operator*=(const Vector_3f &other_vector)
  {
    *this = *this * other_vector;
  }
  
  void Vector_3f::operator/=(const Vector_3f &other_vector)
  {
    *this = *this / other_vector;
  }
  
  Vector_3f::operator Vector_2f() const
  {
    return {this->x, this->y};
  }
}
