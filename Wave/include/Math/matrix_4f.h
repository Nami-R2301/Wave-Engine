//
// Created by nami on 5/14/22.
//

#pragma once

#include <memory>
#include <Math/vector.h>
#include <Core/core_macros.h>
#include <Utilities/logger.h>
#include <Core/interfaces.h>

namespace Wave::Math
{
  
  class Matrix_4f : public I_printable
  {
    public:
    Matrix_4f();
    explicit Matrix_4f(float identity);
    Matrix_4f(const Matrix_4f &other_matrix);
    ~Matrix_4f() override = default;
    
    void init_identity();
    void init_translation(const Vector_3f &vector_3f);
    void init_translation(float x, float y, float z);
    void init_rotation(const Vector_3f &vector_3f);
    void init_rotation(float x, float y, float z);
    void init_scale(Vector_3f vector_3f);
    void init_scale(float x, float y, float z);
    void init_perspective_projection(float fov_, float z_near_, float z_far_);
    void
    init_orthographic_projection(float left, float right_, float top_, float bottom_, float z_near_, float z_far_);
    void init_camera(Vector_3f direction, Vector_3f up);
    
    [[nodiscard]] int length() const;
    [[nodiscard]] const std::shared_ptr<float[4][4]> &get_matrix() const;
    [[nodiscard]] float get_value(int row, int col) const;
    [[nodiscard]] int get_num_rows() const;
    [[nodiscard]] int get_num_cols() const;
    
    INTERFACE_PRINTABLE;
    
    void set_value(int row, int col, float value);
    void transpose();
    static void decompose_matrix(const Matrix_4f &matrix_to_decompose, Math::Vector_3f &translation_address,
                                 Math::Vector_3f &rotation_address, Math::Vector_3f &scale_address);
    
    Matrix_4f operator*(const Matrix_4f &other_matrix) const;
    Vector_4f operator*(const Vector_4f &vector_4f) const;
    Matrix_4f &operator=(const Matrix_4f &other_matrix);
    bool operator==(const Matrix_4f &mtx) const;
    private:
    int num_rows = 0;
    int num_cols = 0;
    std::shared_ptr<float[4][4]> matrix;
  };
}