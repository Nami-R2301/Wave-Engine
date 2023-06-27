//
// Created by nami on 5/19/22.
//

#pragma once

#include <Core/interfaces.h>
#include <Math/matrix_4f.h>

namespace Wave::Math
{
  
  class Transform : public I_printable
  {
    public:
    Transform();
    Transform(const Transform &) = default;
    Transform(const Vector_3f &translation_, const Vector_3f &rotation_, const Vector_3f &scale_);
    ~Transform() override = default;
    
    [[nodiscard]] const Vector_3f &get_translation() const;
    [[nodiscard]] Vector_3f &get_translation();
    [[nodiscard]] const Vector_3f &get_rotation() const;
    [[nodiscard]] Vector_3f &get_rotation();
    [[nodiscard]] const Vector_3f &get_scale() const;
    [[nodiscard]] Vector_3f &get_scale();
    [[nodiscard]] Matrix_4f get_transform_matrix() const;
    
    void set_rotation(const Vector_3f &rotation_);
    void set_rotation(float x, float y, float z);
    void set_translation(const Vector_3f &translation);
    void set_translation(float x, float y, float z);
    void set_scale(const Vector_3f &scale_);
    void set_scale(float x, float y, float z);
    
    INTERFACE_PRINTABLE
    
    Transform operator+(const Transform &transform) const;
    Transform operator-(const Transform &transform) const;
    Transform operator*(const Transform &transform) const;
    Transform operator/(const Transform &transform) const;
    void operator+=(const Transform &transform);
    bool operator==(const Transform &transform) const;
    Transform &operator=(const Transform &transform);
    private:
    Vector_3f translation = Vector_3f(0);
    Vector_3f rotation = Vector_3f(0);
    Vector_3f scale = Vector_3f(1);
  };
}
