//
// Created by nami on 5/13/22.
//

#pragma once

#include <Core/core_macros.h>
#include <cmath>
#include <string>

namespace Wave
{
  
  #define VECTOR2F_SIZE ((sizeof(float) * 2))
  
  class Vector_2f
  {
  public:
    Vector_2f() = default;
    explicit Vector_2f(float uniform_scalar);
    Vector_2f(float x, float y);
    ~Vector_2f() = default;
    
    [[nodiscard]] float get_x() const;
    [[nodiscard]] float get_y() const;
  
    void set_x(float x);
    void set_y(float y);
  
    [[nodiscard]] float length() const;
    [[nodiscard]] float dot(const Vector_2f &vector_) const;
    [[nodiscard]] Vector_2f normalize() const;
    [[nodiscard]] Vector_2f absolute() const;
    [[nodiscard]] Vector_2f rotate(float angle) const;
  
    [[nodiscard]] std::string to_string() const;
  
    float &operator [](unsigned int index);
  
    Vector_2f operator +(const Vector_2f &other_vector) const;
    Vector_2f operator +(float coord) const;
    Vector_2f operator -(const Vector_2f &other_vector) const;
    Vector_2f operator -(float coord) const;
    Vector_2f operator *(const Vector_2f &other_vector) const;
    Vector_2f operator *(float coord) const;
    Vector_2f operator /(const Vector_2f &other_vector) const;
    Vector_2f operator /(float coord) const;
    
    bool operator ==(const Vector_2f &other_vector) const;
    Vector_2f &operator =(const Vector_2f &other_vec2);
    void operator +=(const Vector_2f &other_vec2);
    void operator +=(float scalar);
    void operator -=(const Vector_2f &other_vec2);
    void operator -=(float scalar);
    void operator *=(const Vector_2f &other_vec2);
    void operator *=(float scalar);
    void operator /=(const Vector_2f &other_vec2);
    void operator /=(float scalar);
    bool operator !=(const Vector_2f &other_vector) const;
  private:
    float x = 0;
    float y = 0;
  };
  
  #define VECTOR3F_SIZE (sizeof(float) * 3)
  
  class Vector_3f
  {
  public:
    Vector_3f() = default;
    explicit Vector_3f(float uniform_scalar);
    Vector_3f(float x, float y, float z);
    explicit Vector_3f(const Vector_2f &vector_2f);
    ~Vector_3f() = default;
    
    [[nodiscard]] float get_x() const;
    [[nodiscard]] float get_y() const;
    [[nodiscard]] float get_z() const;
    
    void set_x(float x);
    void set_y(float y);
    void set_z(float z_);
  
    [[nodiscard]] float length() const;
    [[nodiscard]] float dot(const Vector_3f &other_vector) const;
    [[nodiscard]] Vector_3f normalize(float max_dx = 1.0f, float max_dy = 1.0f, float max_dz = 1.0f) const;
    [[nodiscard]] Vector_3f cross(const Vector_3f &other_vector) const;
    [[nodiscard]] Vector_3f rotate(float angle, const Vector_3f &axis) const;
    [[nodiscard]] Vector_3f absolute() const;
    [[nodiscard]] std::string to_string() const;
  
    float &operator [](unsigned int index);
  
    Vector_3f operator +(const Vector_3f &other_vector) const;
    Vector_3f operator +(float coord) const;
    Vector_3f operator -(const Vector_3f &other_vector) const;
    Vector_3f operator -(float coord) const;
    Vector_3f operator *(const Vector_3f &other_vector) const;
    Vector_3f operator *(float coord) const;
    Vector_3f operator /(const Vector_3f &other_vector) const;
    Vector_3f operator /(float coord) const;
    bool operator ==(const Vector_3f &other_vector) const;
    Vector_3f &operator =(const Vector_3f &other_vec2);
    void operator +=(const Vector_3f &other_vec2);
    void operator -=(const Vector_3f &other_vec2);
    void operator *=(const Vector_3f &other_vec2);
    void operator /=(const Vector_3f &other_vec2);
    bool operator !=(const Vector_3f &other_vector) const;
  private:
    float x = 0;
    float y = 0;
    float z = 0;
  };
  
  class Vector_4f
  {
  public:
    Vector_4f() = default;
    explicit Vector_4f(float uniform_scalar);
    Vector_4f(float x_, float y_, float z_, float w_);
    explicit Vector_4f(const Vector_3f &vector_3f);
    ~Vector_4f() = default;
    
    [[nodiscard]] float get_x() const;
    [[nodiscard]] float get_y() const;
    [[nodiscard]] float get_z() const;
    [[nodiscard]] float get_w() const;
    
    void set_x(float x);
    void set_y(float y);
    void set_z(float z_);
    void set_w(float w_);
    
    [[nodiscard]] float length() const;
    [[nodiscard]] float dot(const Vector_4f &other_vector) const;
    [[nodiscard]] Vector_4f
    normalize(float max_dx = 1.0f, float max_dy = 1.0f, float max_dz = 1.0f, float max_dw = 1.0f) const;
    [[nodiscard]] Vector_4f conjugate() const;
    [[nodiscard]] Vector_4f cross(const Vector_4f &other_vector) const;
    [[nodiscard]] Vector_4f rotate(float angle, const Vector_4f &axis) const;
    [[nodiscard]] Vector_4f absolute() const;
    [[nodiscard]] std::string to_string() const;
    
    float &operator [](unsigned int index);
    
    Vector_4f operator +(const Vector_4f &other_vector) const;
    Vector_4f operator +(float coord) const;
    Vector_4f operator -(const Vector_4f &other_vector) const;
    Vector_4f operator -(float coord) const;
    Vector_4f operator *(const Vector_4f &other_vector) const;
    Vector_4f operator *(const Vector_3f &other_vector) const;
    Vector_4f operator *(float coord) const;
    Vector_4f operator /(const Vector_4f &other_vector) const;
    Vector_4f operator /(float coord) const;
    bool operator ==(const Vector_4f &other_vector) const;
    Vector_4f &operator =(const Vector_4f &other_vector);
    void operator +=(const Vector_4f &other_vector);
    void operator -=(const Vector_4f &other_vector);
    void operator *=(const Vector_4f &other_vector);
    void operator /=(const Vector_4f &other_vector);
    bool operator !=(const Vector_4f &other_vector) const;
  private:
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;
  };
}
