//
// Created by nami on 5/13/22.
//

#include <Math/vector.h>

namespace Wave
{
  
  Vector_2f::Vector_2f(float uniform_scalar)
  {
    this->x = uniform_scalar;
    this->y = uniform_scalar;
  }
  
  Vector_2f::Vector_2f(float x, float y)
  {
    this->x = x;
    this->y = y;
  }
  
  float Vector_2f::get_x() const
  {
    return this->x;
  }
  
  float Vector_2f::get_y() const
  {
    return this->y;
  }
  
  void Vector_2f::set_x(float x_)
  {
    this->x = x_;
  }
  
  void Vector_2f::set_y(float y_)
  {
    this->y = y_;
  }
  
  float Vector_2f::length() const
  {
    return std::sqrt((this->x * this->x) + (this->y * this->y));
  }
  
  float Vector_2f::dot(const Vector_2f &vector_) const
  {
    return (this->x * vector_.get_x()) + (this->y * vector_.get_y());
  }
  
  Vector_2f Vector_2f::normalize() const
  {
    return {this->x / length(),
            this->y / length()};
  }
  
  Vector_2f Vector_2f::absolute() const
  {
    return {std::abs(this->x),
            std::abs(this->y)};
  }
  
  Vector_2f Vector_2f::rotate(float angle) const
  {
    double rad_angle = angle * (M_PI / 180);
    double cos_angle = std::cos(rad_angle);
    double sin_angle = std::sin(rad_angle);
    
    return {(float) (this->x * cos_angle - this->y * sin_angle),
            (float) (this->x * sin_angle + this->y * cos_angle)};
  }
  
  std::string Vector_2f::to_string() const
  {
    char buffer[29 + sizeof(float) * 2] {0};  // Three 8-digit-long float.
  
    if (snprintf(buffer, 29 + sizeof(float) * 2, "x : %.2f, y : %.2f\n", this->x, this->y) < 0)
    {
      return {"[MINOR] [VECTOR 2D ERROR] --> Error converting vector 2D to string, snprintf failed!\n"};
    }
  
    return {buffer};
  }
  
  float &Vector_2f::operator [](unsigned int index)
  {
    if (index > 1) return this->x;
    return index == 0 ? this->x : this->y;
  }
  
  // Add two vectors.
  Vector_2f Vector_2f::operator +(const Vector_2f &other_vector) const
  {
    return {this->x + other_vector.get_x(),
            this->y + other_vector.get_y()};
  }
  
  Vector_2f Vector_2f::operator -(const Vector_2f &other_vector) const
  {
    return {this->x - other_vector.get_x(),
            this->y - other_vector.get_y()};
  }
  
  Vector_2f Vector_2f::operator *(const Vector_2f &other_vector) const
  {
    return {this->x * other_vector.get_x(),
            this->y * other_vector.get_y()};
  }
  
  Vector_2f Vector_2f::operator /(const Vector_2f &other_vector) const
  {
    return {this->x / other_vector.get_x(),
            this->y / other_vector.get_y()};
  }
  
  Vector_2f Vector_2f::operator +(float coord) const
  {
    return {this->x + coord,
            this->y + coord};
  }
  
  Vector_2f Vector_2f::operator -(float coord) const
  {
    return {this->x - coord,
            this->y - coord};
  }
  
  Vector_2f Vector_2f::operator *(float coord) const
  {
    return {this->x * coord,
            this->y * coord};
  }
  
  Vector_2f Vector_2f::operator /(float coord) const
  {
    return {this->x / coord,
            this->y / coord};
  }
  
  bool Vector_2f::operator ==(const Vector_2f &other_vector) const
  {
    if (this == &other_vector) return true;
    return this->x == other_vector.x && this->y == other_vector.y;
  }
  
  bool Vector_2f::operator !=(const Vector_2f &other_vector) const
  {
    return !(*this == other_vector);
  }
  
  Vector_2f &Vector_2f::operator =(const Vector_2f &other_vector)
  {
    if (this == &other_vector) return *this;
    this->x = other_vector.x;
    this->y = other_vector.y;
    return *this;
  }
  
  void Vector_2f::operator +=(const Vector_2f &other_vector)
  {
    *this = *this + other_vector;
  }
  
  void Vector_2f::operator -=(const Vector_2f &other_vector)
  {
    *this = *this - other_vector;
  }
  
  void Vector_2f::operator *=(const Vector_2f &other_vector)
  {
    *this = *this * other_vector;
  }
  
  void Vector_2f::operator /=(const Vector_2f &other_vector)
  {
    *this = *this / other_vector;
  }
  
  void Vector_2f::operator +=(float scalar)
  {
    *this = *this + scalar;
  }
  
  void Vector_2f::operator -=(float scalar)
  {
    *this = *this - scalar;
  }
  
  void Vector_2f::operator *=(float scalar)
  {
    *this = *this * scalar;
  }
  
  void Vector_2f::operator /=(float scalar)
  {
    *this = *this / scalar;
  }
}



