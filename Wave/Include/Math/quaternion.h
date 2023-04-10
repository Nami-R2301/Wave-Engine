//
// Created by nami on 5/14/22.
//

#pragma once

#include <Math/vector.h>
#include <wave_pch.h>

namespace Wave
{
  
  // 4D- Vectors with the imaginary component.
  class Quaternion : public Printable
  {
  public:
    Quaternion(float x, float y, float z, float w);
    
    [[nodiscard]] float get_x() const;
    [[nodiscard]] float get_y() const;
    [[nodiscard]] float get_z() const;
    [[nodiscard]] float get_w() const;
    
    void set_x(float x);
    void set_y(float y);
    void set_z(float z);
    void set_w(float w);
    
    INTERFACE_PRINT
    
    [[nodiscard]] float length() const;
    Quaternion normalize();
    [[nodiscard]] Quaternion conjugate() const;
    [[nodiscard]] Quaternion multiply(Vector_3f vector_3f) const;
    
    Quaternion operator *(const Quaternion &other_quaternion) const;
  private:
    float x;
    float y;
    float z;
    float w;
  };
}
