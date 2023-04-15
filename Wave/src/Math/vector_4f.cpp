//
// Created by nami on 5/14/22.
//

#include <Math/vector.h>

namespace Wave
{
  
  Vector_4f::Vector_4f(float uniform_scalar)
  {
    this->x = uniform_scalar;
    this->y = uniform_scalar;
    this->z = uniform_scalar;
    this->w = uniform_scalar;
  }
  
  Vector_4f::Vector_4f(float x, float y, float z, float w)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  
  Vector_4f::Vector_4f(const Vector_3f &vector_3f)
  {
    this->x = vector_3f.get_x();
    this->y = vector_3f.get_y();
    this->z = vector_3f.get_z();
    this->w = 1.0f;
  }
  
  float Vector_4f::get_x() const
  {
    return x;
  }
  
  void Vector_4f::set_x(float x_)
  {
    Vector_4f::x = x_;
  }
  
  float Vector_4f::get_y() const
  {
    return y;
  }
  
  void Vector_4f::set_y(float y_)
  {
    Vector_4f::y = y_;
  }
  
  float Vector_4f::get_z() const
  {
    return z;
  }
  
  void Vector_4f::set_z(float z_)
  {
    Vector_4f::z = z_;
  }
  
  float Vector_4f::get_w() const
  {
    return w;
  }
  
  void Vector_4f::set_w(float w_)
  {
    Vector_4f::w = w_;
  }
  
  float Vector_4f::length() const
  {
    return std::sqrt((this->x * this->x) +
                     (this->y * this->y) +
                     (this->z * this->z) +
                     (this->w * this->w));
  }
  
  Vector_4f Vector_4f::cross(const Vector_4f &other_vector) const
  {
    float x_ = this->y * other_vector.get_z() - this->z * other_vector.get_y();
    float y_ = this->z * other_vector.get_x() - this->x * other_vector.get_z();
    float z_ = this->x * other_vector.get_y() - this->y * other_vector.get_x();
    float w_ = this->z * other_vector.get_x() - this->y * other_vector.get_z();
    return {x_,
            y_,
            z_,
            w_};
  }
  
  float Vector_4f::dot(const Vector_4f &other_vector) const
  {
    return (this->x * other_vector.get_x()) +
           (this->y * other_vector.get_y()) +
           (this->z * other_vector.get_z() +
            (this->w * other_vector.get_w()));
  }
  
  Vector_4f Vector_4f::normalize(float max_dx, float max_dy, float max_dz, float max_dw) const
  {
    if (max_dx == 1.0f && max_dy == 1.0f && max_dz == 1.0f && max_dw == 1.0f)
    {
      float length = this->length();
      return {this->x / length,
              this->y / length,
              this->z / length,
              this->w / length};
    }
    float factor = std::max(max_dx, max_dy);
    factor = std::max(factor, max_dz);
    return {(this->x - max_dx / 2) / factor * 2,
            (this->y - max_dy / 2) / factor * 2,
            (this->z - max_dz / 2) / factor * 2,
            (this->w - max_dw / 2) / factor * 2};
  }
  
  Vector_4f Vector_4f::conjugate() const
  {
    return {-this->x,
            -this->y,
            -this->z,
            this->w};
  }
  
  Vector_4f Vector_4f::rotate(float angle, const Vector_4f &axis) const
  {
    auto angle_to_rad = (float) (angle * (M_PI / 180));
    float sin_half_angle = sinf(angle_to_rad / 2);
    float cos_half_angle = cosf(angle_to_rad / 2);
    
    float rotation_x = axis.get_x() * sin_half_angle, rotation_y = axis.get_y() * sin_half_angle,
        rotation_z = axis.get_z() * sin_half_angle, rotation_w = cos_half_angle;
    
    Vector_4f rotation(rotation_x, rotation_y, rotation_z, rotation_w);
    Vector_4f conjugate = rotation.conjugate();
    Vector_4f w_ = (rotation * *this) * conjugate;
    
    return {w_.get_x(),
            w_.get_y(),
            w_.get_z(),
            1.0f};
  }
  
  Vector_4f Vector_4f::absolute() const
  {
    return {std::abs(this->x),
            std::abs(this->y),
            std::abs(this->z),
            std::abs(this->w)};
  }
  
  std::string Vector_4f::to_string() const
  {
    char buffer[35 + sizeof(float) * 4] {0};  // Three 8-digit-long float.
    
    if (snprintf(buffer, 29 + sizeof(float) * 3, "x : %.2f, y : %.2f, z : %.2f, w : %.2f\n",
                 this->x, this->y, this->z, this->w) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Vector_3f]!";
    }
    return buffer;
  }
  
  float &Vector_4f::operator [](unsigned int index)
  {
    if (index > 3) return this->x;
    return index == 0 ? this->x : index == 1 ? this->y : index == 2 ? this->z : this->w;
  }
  
  Vector_4f Vector_4f::operator *(const Vector_4f &other_vector) const
  {
    float w_ = this->w * other_vector.get_w() - this->x * other_vector.get_x() - this->y * other_vector.get_y() -
               this->z * other_vector.get_z();
    float x_ = this->x * other_vector.get_w() + this->w * other_vector.get_x() + this->y * other_vector.get_z() -
               this->z * other_vector.get_y();
    float y_ = this->y * other_vector.get_w() + this->w * other_vector.get_y() + this->z * other_vector.get_x() -
               this->x * other_vector.get_z();
    float z_ = this->z * other_vector.get_w() + this->w * other_vector.get_z() + this->x * other_vector.get_y() -
               this->y * other_vector.get_x();
    return {x_,
            y_,
            z_,
            w_};
  }
  
  Vector_4f Vector_4f::operator *(const Vector_3f &other_vector) const
  {
    float w_ = -this->x * other_vector.get_x() - this->y * other_vector.get_y() - this->z * other_vector.get_z();
    float x_ = this->w * other_vector.get_x() + this->y * other_vector.get_z() - this->z * other_vector.get_y();
    float y_ = this->w * other_vector.get_y() + this->z * other_vector.get_x() - this->x * other_vector.get_z();
    float z_ = this->w * other_vector.get_z() + this->x * other_vector.get_y() - this->y * other_vector.get_x();
    return {x_,
            y_,
            z_,
            w_};
  }
  
  Vector_4f Vector_4f::operator +(const Vector_4f &other_vector) const
  {
    return {this->x + other_vector.get_x(),
            this->y + other_vector.get_y(),
            this->z + other_vector.get_z(),
            this->w + other_vector.get_w()};
  }
  
  Vector_4f Vector_4f::operator +(float coord) const
  {
    return {this->x + coord,
            this->y + coord,
            this->z + coord,
            this->w + coord};
  }
  
  Vector_4f Vector_4f::operator -(const Vector_4f &other_vector) const
  {
    return {this->x - other_vector.get_x(),
            this->y - other_vector.get_y(),
            this->z - other_vector.get_z(),
            this->w - other_vector.get_w()};
  }
  
  Vector_4f Vector_4f::operator -(float coord) const
  {
    return {this->x - coord,
            this->y - coord,
            this->z - coord,
            this->w - coord};
  }
  
  Vector_4f Vector_4f::operator *(float coord) const
  {
    return {this->x * coord,
            this->y * coord,
            this->z * coord,
            this->w * coord};
  }
  
  Vector_4f Vector_4f::operator /(const Vector_4f &other_vector) const
  {
    return {this->x / other_vector.get_x(),
            this->y / other_vector.get_y(),
            this->z / other_vector.get_z(),
            this->w / other_vector.get_w()};
  }
  
  Vector_4f Vector_4f::operator /(float coord) const
  {
    return {this->x / coord,
            this->y / coord,
            this->z / coord,
            this->w / coord};
  }
  
  bool Vector_4f::operator ==(const Vector_4f &other_vector) const
  {
    if (this == &other_vector) return true;
    if (this->length() != other_vector.length()) return false;
    return (this->x == other_vector.get_x() &&
            this->y == other_vector.get_y() &&
            this->z == other_vector.get_z() &&
            this->w == other_vector.get_w());
  }
  
  bool Vector_4f::operator !=(const Vector_4f &other_vector) const
  {
    if (this == &other_vector) return false;
    return this->x != other_vector.x && this->y != other_vector.y && this->z != other_vector.z &&
           this->w != other_vector.w;
  }
  
  Vector_4f &Vector_4f::operator =(const Vector_4f &other_vector)
  {
    if (this == &other_vector) return *this;
    this->x = other_vector.x;
    this->y = other_vector.y;
    this->z = other_vector.z;
    this->w = other_vector.w;
    return *this;
  }
  
  void Vector_4f::operator +=(const Vector_4f &other_vector)
  {
    *this = *this + other_vector;
  }
  
  void Vector_4f::operator -=(const Vector_4f &other_vector)
  {
    *this = *this - other_vector;
  }
  
  void Vector_4f::operator *=(const Vector_4f &other_vector)
  {
    *this = *this * other_vector;
  }
  
  void Vector_4f::operator /=(const Vector_4f &other_vector)
  {
    *this = *this / other_vector;
  }
}