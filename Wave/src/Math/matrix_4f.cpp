//
// Created by nami on 5/14/22.
//

#include <Math/matrix_4f.h>
#include <Utilities/logger.h>

namespace Wave
{
  
  Matrix_4f::Matrix_4f()
  {
    this->matrix = std::shared_ptr<float[4][4]>(new float[4][4]());
    this->num_rows = 4;
    this->num_cols = 4;
    init_identity();
  }
  
  Matrix_4f::Matrix_4f(const Matrix_4f &other_matrix)
  {
    this->matrix = std::shared_ptr<float[4][4]>(new float[4][4]());
    this->num_rows = other_matrix.get_num_rows();
    this->num_cols = other_matrix.get_num_cols();
    for (int i = 0; i < this->num_rows; i++)
    {
      for (int j = 0; j < this->num_cols; j++)
      {
        set_value(i, j, other_matrix.get_value(i, j));
      }
    }
    init_identity();
  }
  
  const std::shared_ptr<float[4][4]> &Matrix_4f::get_matrix() const
  {
    return this->matrix;
  }
  
  float Matrix_4f::get_value(int row, int col) const
  {
    return this->matrix.get()[row][col];
  }
  
  void Matrix_4f::set_value(int row, int col, float value)
  {
    this->matrix.get()[row][col] = value;
  }
  
  int Matrix_4f::get_num_rows() const
  {
    return num_rows;
  }
  
  int Matrix_4f::get_num_cols() const
  {
    return num_cols;
  }
  
  std::string Matrix_4f::to_string() const
  {
    std::string output("[Matrix] :\n");
    char buffer_c[FILENAME_MAX] = {'\0'};
    for (int i = 0; i < get_num_rows(); ++i)
    {
      for (int j = 0; j < get_num_cols(); ++j)
      {
        // Last column to print.
        if ((i + 1) * (j + 1) == length())
        {
          if (snprintf(buffer_c, FILENAME_MAX, "%.2f", get_value(i, j)) >= 0)
          {
            output += buffer_c;
          }
        }
        else if (snprintf(buffer_c, FILENAME_MAX, "%.2f,\t", get_value(i, j)) >= 0)
        {
          output += buffer_c;
        }
      }
      output += "\n";
    }
    return output;
  }
  
  int Matrix_4f::length() const
  {
    return num_rows * num_cols;
  }
  
  void Matrix_4f::init_identity()
  {
    this->matrix.get()[0][0] = 1.0f, this->matrix.get()[1][1] = 1.0f, this->matrix.get()[2][2] = 1.0f, this->matrix.get()[3][3] = 1.0f;
  }
  
  void Matrix_4f::init_translation(float x, float y, float z)
  {
    // For every row except w.
    this->matrix.get()[0][3] = x, this->matrix.get()[1][3] = y, this->matrix.get()[2][3] = z;
  }
  
  void Matrix_4f::init_translation(const Vector_3f &vector_3f)
  {
    // For every row except w.
    this->matrix.get()[0][3] = vector_3f.get_x(), this->matrix.get()[1][3] = vector_3f.get_y(), this->matrix.get()[2][3] = vector_3f.get_z();
  }
  
  void Matrix_4f::init_rotation(Vector_3f vector_3f)
  {
    Matrix_4f rotation_x, rotation_y, rotation_z; // Init rotation matrices.
    
    // Get angles and convert to radiant.
    vector_3f.set_x((float) (vector_3f.get_x() * (M_PI / 180)));
    vector_3f.set_y((float) (vector_3f.get_y() * (M_PI / 180)));
    vector_3f.set_z((float) (vector_3f.get_z() * (M_PI / 180)));
    
    // Set angles for rotation matrix z.
    rotation_z.set_value(0, 0, cosf(vector_3f.get_z()));
    rotation_z.set_value(0, 1, -(sinf(vector_3f.get_z())));
    rotation_z.set_value(1, 0, sinf(vector_3f.get_z()));
    rotation_z.set_value(1, 1, cosf(vector_3f.get_z()));
    
    // Set angles for rotation matrix x.
    rotation_x.set_value(1, 1, cosf(vector_3f.get_x()));
    rotation_x.set_value(1, 2, -(sinf(vector_3f.get_x())));
    rotation_x.set_value(2, 1, sinf(vector_3f.get_x()));
    rotation_x.set_value(2, 2, cosf(vector_3f.get_x()));
    
    // Set angles for rotation matrix y.
    rotation_y.set_value(0, 0, cosf(vector_3f.get_y()));
    rotation_y.set_value(0, 2, -(sinf(vector_3f.get_y())));
    rotation_y.set_value(2, 0, sinf(vector_3f.get_y()));
    rotation_y.set_value(2, 2, cosf(vector_3f.get_y()));
    
    this->matrix = (rotation_z * (rotation_y * rotation_x)).get_matrix();  // Init current matrix instance.
  }
  
  void Matrix_4f::init_rotation(float x, float y, float z)
  {
    Matrix_4f rotation_x, rotation_y, rotation_z; // Init rotation matrices.
    Vector_3f vector_3f(x, y, z);
    
    // Get angles and convert to radiant.
    vector_3f.set_x((float) (vector_3f.get_x() * (M_PI / 180)));
    vector_3f.set_y((float) (vector_3f.get_y() * (M_PI / 180)));
    vector_3f.set_z((float) (vector_3f.get_z() * (M_PI / 180)));
    
    // Set angles for rotation matrix z.
    rotation_z.set_value(0, 0, cosf(vector_3f.get_z()));
    rotation_z.set_value(0, 1, -(sinf(vector_3f.get_z())));
    rotation_z.set_value(1, 0, sinf(vector_3f.get_z()));
    rotation_z.set_value(1, 1, cosf(vector_3f.get_z()));
    
    // Set angles for rotation matrix x.
    rotation_x.set_value(1, 1, cosf(vector_3f.get_x()));
    rotation_x.set_value(1, 2, -(sinf(vector_3f.get_x())));
    rotation_x.set_value(2, 1, sinf(vector_3f.get_x()));
    rotation_x.set_value(2, 2, cosf(vector_3f.get_x()));
    
    // Set angles for rotation matrix y.
    rotation_y.set_value(0, 0, cosf(vector_3f.get_y()));
    rotation_y.set_value(0, 2, -(sinf(vector_3f.get_y())));
    rotation_y.set_value(2, 0, sinf(vector_3f.get_y()));
    rotation_y.set_value(2, 2, cosf(vector_3f.get_y()));
    
    this->matrix = (rotation_z * (rotation_y * rotation_x)).get_matrix();  // Init current matrix instance.
  }
  
  void Matrix_4f::init_scale(Vector_3f vector_3f)
  {
    // For every row except w.
    this->matrix.get()[0][0] = vector_3f.get_x(), this->matrix.get()[1][1] = vector_3f.get_y(), this->matrix.get()[2][2] = vector_3f.get_z();
  }
  
  void Matrix_4f::init_scale(float x_, float y_, float z_)
  {
    this->matrix.get()[0][0] = x_, this->matrix.get()[1][1] = y_, this->matrix.get()[2][2] = z_;
  }
  
  void Matrix_4f::init_perspective_projection(float fov_, float z_near_, float z_far_)
  {
    auto fov_to_rad = (float) (fov_ * (M_PI / 180) / 2);
    float tan_half_fov = tanf(fov_to_rad);
    float z_range = z_near_ - z_far_;
    
    set_value(0, 0, 1.0f / (tan_half_fov * 2));
    set_value(1, 1, 1.0f / tan_half_fov);
    set_value(2, 2, (-z_near_ - z_far_) / z_range);  // Scaling the z index to the range.
    set_value(2, 3, 2 * z_far_ * z_near_ / z_range);  // Scaling the z index to the range.
    set_value(3, 2, get_value(2, 2));  // Copy whatever z value we have.
    set_value(3, 3, 0.0f);  // Get rid of w.
  }
  
  void Matrix_4f::init_orthographic_projection(float left_, float right_, float top_, float bottom_, float z_near_,
                                               float z_far_)
  {
    if (right_ - left_ == 0 || top_ - bottom_ == 0) return;
    
    float scale_x = (right_ - left_) / 2;
    float scale_y = (top_ - bottom_) / 2;
    float scale_z = -(z_far_ - z_near_);
    
    init_scale(scale_x, scale_y, scale_z);
    init_translation(0, 0, -z_near_ * 2);
  }
  
  void Matrix_4f::init_camera(Vector_3f direction, Vector_3f up)
  {
    Vector_3f right = up.cross(direction);
    
    this->matrix.get()[0][0] = right.get_x(), this->matrix.get()[0][1] = right.get_y(), this->matrix.get()[0][2] = right.get_z(),
    this->matrix.get()[1][0] = up.get_x(), this->matrix.get()[1][1] = up.get_y(), this->matrix.get()[1][2] = up.get_z();
    this->matrix.get()[2][0] = direction.get_x(), this->matrix.get()[2][1] = direction.get_y(), this->matrix.get()[2][2] = direction.get_z();
  }
  
  Quaternion Matrix_4f::operator *(const Quaternion &vector_4f) const
  {
    Quaternion result(vector_4f);
    
    result.set_x(result.get_x() * get_value(0, 0) +
                 result.get_y() * get_value(0, 1) +
                 result.get_z() * get_value(0, 2) +
                 result.get_w() * get_value(0, 3));
    
    result.set_y(result.get_x() * get_value(1, 0) +
                 result.get_y() * get_value(1, 1) +
                 result.get_z() * get_value(1, 2) +
                 result.get_w() * get_value(1, 3));
    
    result.set_z(result.get_x() * get_value(1, 0) +
                 result.get_y() * get_value(1, 1) +
                 result.get_z() * get_value(1, 2) +
                 result.get_w() * get_value(1, 3));
    
    result.set_w(result.get_x() * get_value(1, 0) +
                 result.get_y() * get_value(1, 1) +
                 result.get_z() * get_value(1, 2) +
                 result.get_w() * get_value(1, 3));
    return result;
  }
  
  Matrix_4f Matrix_4f::operator *(const Matrix_4f &other_matrix) const
  {
    Matrix_4f result;
    
    result.matrix.get()[0][0] = this->matrix.get()[0][0] * other_matrix.get_value(0, 0) +
                                this->matrix.get()[0][1] * other_matrix.get_value(1, 0) +
                                this->matrix.get()[0][2] * other_matrix.get_value(2, 0) +
                                this->matrix.get()[0][3] * other_matrix.get_value(3, 0);
    result.matrix.get()[0][1] = this->matrix.get()[0][0] * other_matrix.get_value(0, 1) +
                                this->matrix.get()[0][1] * other_matrix.get_value(1, 1) +
                                this->matrix.get()[0][2] * other_matrix.get_value(2, 1) +
                                this->matrix.get()[0][3] * other_matrix.get_value(3, 1);
    result.matrix.get()[0][2] = this->matrix.get()[0][0] * other_matrix.get_value(0, 2) +
                                this->matrix.get()[0][1] * other_matrix.get_value(1, 2) +
                                this->matrix.get()[0][2] * other_matrix.get_value(2, 2) +
                                this->matrix.get()[0][3] * other_matrix.get_value(3, 2);
    result.matrix.get()[0][3] = this->matrix.get()[0][0] * other_matrix.get_value(0, 3) +
                                this->matrix.get()[0][1] * other_matrix.get_value(1, 3) +
                                this->matrix.get()[0][2] * other_matrix.get_value(2, 3) +
                                this->matrix.get()[0][3] * other_matrix.get_value(3, 3);
    result.matrix.get()[1][0] = this->matrix.get()[1][0] * other_matrix.get_value(0, 0) +
                                this->matrix.get()[1][1] * other_matrix.get_value(1, 0) +
                                this->matrix.get()[1][2] * other_matrix.get_value(2, 0) +
                                this->matrix.get()[1][3] * other_matrix.get_value(3, 0);
    result.matrix.get()[1][1] = this->matrix.get()[1][0] * other_matrix.get_value(0, 1) +
                                this->matrix.get()[1][1] * other_matrix.get_value(1, 1) +
                                this->matrix.get()[1][2] * other_matrix.get_value(2, 1) +
                                this->matrix.get()[1][3] * other_matrix.get_value(3, 1);
    result.matrix.get()[1][2] = this->matrix.get()[1][0] * other_matrix.get_value(0, 2) +
                                this->matrix.get()[1][1] * other_matrix.get_value(1, 2) +
                                this->matrix.get()[1][2] * other_matrix.get_value(2, 2) +
                                this->matrix.get()[1][3] * other_matrix.get_value(3, 2);
    result.matrix.get()[1][3] = this->matrix.get()[1][0] * other_matrix.get_value(0, 3) +
                                this->matrix.get()[1][1] * other_matrix.get_value(1, 3) +
                                this->matrix.get()[1][2] * other_matrix.get_value(2, 3) +
                                this->matrix.get()[1][3] * other_matrix.get_value(3, 3);
    result.matrix.get()[2][0] = this->matrix.get()[2][0] * other_matrix.get_value(0, 0) +
                                this->matrix.get()[2][1] * other_matrix.get_value(1, 0) +
                                this->matrix.get()[2][2] * other_matrix.get_value(2, 0) +
                                this->matrix.get()[2][3] * other_matrix.get_value(3, 0);
    result.matrix.get()[2][1] = this->matrix.get()[2][0] * other_matrix.get_value(0, 1) +
                                this->matrix.get()[2][1] * other_matrix.get_value(1, 1) +
                                this->matrix.get()[2][2] * other_matrix.get_value(2, 1) +
                                this->matrix.get()[2][3] * other_matrix.get_value(3, 1);
    result.matrix.get()[2][2] = this->matrix.get()[2][0] * other_matrix.get_value(0, 2) +
                                this->matrix.get()[2][1] * other_matrix.get_value(1, 2) +
                                this->matrix.get()[2][2] * other_matrix.get_value(2, 2) +
                                this->matrix.get()[2][3] * other_matrix.get_value(3, 2);
    result.matrix.get()[2][3] = this->matrix.get()[2][0] * other_matrix.get_value(0, 3) +
                                this->matrix.get()[2][1] * other_matrix.get_value(1, 3) +
                                this->matrix.get()[2][2] * other_matrix.get_value(2, 3) +
                                this->matrix.get()[2][3] * other_matrix.get_value(3, 3);
    result.matrix.get()[3][0] = this->matrix.get()[3][0] * other_matrix.get_value(0, 0) +
                                this->matrix.get()[3][1] * other_matrix.get_value(1, 0) +
                                this->matrix.get()[3][2] * other_matrix.get_value(2, 0) +
                                this->matrix.get()[3][3] * other_matrix.get_value(3, 0);
    result.matrix.get()[3][1] = this->matrix.get()[3][0] * other_matrix.get_value(0, 1) +
                                this->matrix.get()[3][1] * other_matrix.get_value(1, 1) +
                                this->matrix.get()[3][2] * other_matrix.get_value(2, 1) +
                                this->matrix.get()[3][3] * other_matrix.get_value(3, 1);
    result.matrix.get()[3][2] = this->matrix.get()[3][0] * other_matrix.get_value(0, 2) +
                                this->matrix.get()[3][1] * other_matrix.get_value(1, 2) +
                                this->matrix.get()[3][2] * other_matrix.get_value(2, 2) +
                                this->matrix.get()[3][3] * other_matrix.get_value(3, 2);
    result.matrix.get()[3][3] = this->matrix.get()[3][0] * other_matrix.get_value(0, 3) +
                                this->matrix.get()[3][1] * other_matrix.get_value(1, 3) +
                                this->matrix.get()[3][2] * other_matrix.get_value(2, 3) +
                                this->matrix.get()[3][3] * other_matrix.get_value(3, 3);
    
    return result;
  }
  
  Matrix_4f &Matrix_4f::operator =(const Matrix_4f &other_matrix)
  {
    if (this == &other_matrix) return *this;
    this->matrix = other_matrix.matrix;
    this->num_cols = other_matrix.num_cols;
    this->num_rows = other_matrix.num_rows;
    return *this;
  }
  
  bool Matrix_4f::operator ==(const Matrix_4f &mtx) const
  {
    if (this == &mtx) return true;
    for (int i = 0; i < this->num_rows; ++i)
    {
      for (int j = 0; j < this->num_cols; ++j)
      {
        if (this->get_value(i, j) != mtx.get_value(i, j)) return false;
      }
    }
    return true;
  }
}