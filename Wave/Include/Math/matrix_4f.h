//
// Created by nami on 5/14/22.
//

#pragma once
#define _USE_MATH_DEFINES  // For Visual Studio.

#include <cmath>  // Cos, sin and sqrt functions.
#include <Math/vector.h>
#include <Core/core_macros.h>

namespace Wave
{
  
  class Matrix_4f : public Printable
  {
  public:
    Matrix_4f();
    Matrix_4f(const Matrix_4f &other_matrix);
    ~Matrix_4f() override = default;
    
    void init_identity();
    void init_translation(const Vector_3f &vector_3f);
    void init_translation(float x, float y, float z);
    void init_rotation(Vector_3f vector_3f);
    void init_rotation(float x, float y, float z);
    void init_scale(Vector_3f vector_3f);
    void init_scale(float x, float y, float z);
    void init_perspective_projection(float fov_, float z_near_, float z_far_);
    void init_orthographic_projection(float left, float right_, float top_, float bottom_, float z_near_, float z_far_);
    void init_camera(Vector_3f direction, Vector_3f up);
    
    [[nodiscard]] int length() const;
    [[nodiscard]] const std::shared_ptr<float[4][4]> &get_matrix() const;
    [[nodiscard]] float get_value(int row, int col) const;
    [[nodiscard]] int get_num_rows() const;
    [[nodiscard]] int get_num_cols() const;
    void set_value(int row, int col, float value);
    
    INTERFACE_PRINT
  
    Matrix_4f operator *(const Matrix_4f &other_matrix) const;
    Vector_4f operator *(const Vector_4f &vector_4f) const;
    Matrix_4f &operator =(const Matrix_4f &other_matrix);
    bool operator ==(const Matrix_4f &mtx) const;
  private:
    int num_rows = 0;
    int num_cols = 0;
    std::shared_ptr<float[4][4]> matrix;
  };
}