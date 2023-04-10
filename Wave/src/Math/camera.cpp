//
// Created by nami on 5/25/22.
//

#include <Math/camera.h>

namespace Wave
{
  
  static const Vector_3f y_axis = Vector_3f(0, 1, 0);
  
  /**************************************** PERSPECTIVE CAMERA *************************************/
  
  Perspective_camera::Perspective_camera(float fov_, float width_, float height_, float z_near_,
                                         float z_far_)
  {
    this->fov = fov_;
    this->width = width_;
    this->height = height_;
    this->z_near = z_near_;
    this->z_far = z_far_;
    this->position = Vector_3f(0, 0, 0);
    this->forward = Vector_3f(0, 0, 1);
    this->up = Vector_3f(0, 1, 0);
    this->center_position = Vector_2f(static_cast<float>(this->width) / 2.0f, static_cast<float>(this->height) / 2.0f);
    Perspective_camera::update_projection_matrix();
  }
  
  std::string Perspective_camera::to_string() const
  {
    std::string output("[Perspective camera] :\n");
    
    output += "Y-axis --> " + y_axis.to_string();
    output += "Position on screen --> " + this->position.to_string();
    output += "Target --> " + this->forward.to_string();
    output += "Up --> " + this->up.to_string();
    output += "View matrix --> " + this->view_matrix.to_string();
    output += "Projection matrix --> " + this->projection_matrix.to_string();
    
    return output;
  }
  
  void Perspective_camera::move(const Vector_3f &direction_, const float amount)
  {
    this->position += (direction_.normalize() * amount);
    update_view_matrix();
  }
  
  void Perspective_camera::move(float x, float y, float z, float amount)
  {
    Vector_3f direction(x, y, z);
    this->position += (direction.normalize() * amount);
    update_view_matrix();
  }
  
  void Perspective_camera::rotate_x(float angle)
  {
    Vector_3f horizontal_axis = y_axis.cross(this->forward);
    this->forward = this->forward.rotate(angle, horizontal_axis);
    this->up = this->forward.cross(horizontal_axis);
    update_view_matrix();
  }
  
  void Perspective_camera::rotate_y(float angle)
  {
    Vector_3f horizontal_axis = y_axis.cross(this->forward);
    this->forward = this->forward.rotate(angle, y_axis);
    this->up = this->forward.cross(horizontal_axis);
    update_view_matrix();
  }
  
  void Perspective_camera::look_at(const Vector_3f &direction_)
  {
    this->forward = direction_;
    update_view_matrix();
  }
  
  void Perspective_camera::look_at(float x, float y, float z)
  {
    this->forward = Vector_3f(x, y, z);
    update_view_matrix();
  }
  
  void Perspective_camera::update_view_matrix()
  {
    Matrix_4f rotation, translation;
    rotation.init_camera(this->forward, this->up);
    translation.init_translation(Vector_3f(-get_position().get_x(),
                                           -get_position().get_y(),
                                           -get_position().get_z()));
    this->view_matrix = rotation * translation;
  }
  
  void Perspective_camera::update_projection_matrix()
  {
    this->projection_matrix.init_perspective_projection(this->fov, this->width,
                                                        this->height, this->z_near, this->z_far);
  }
  
  /******************************************** ORTHOGRAPHIC CAMERA ******************************************/
  
  Orthographic_camera::Orthographic_camera(float width_, float height_, float left_, float right_,
                                           float top_, float bottom_, float z_near_,
                                           float z_far_)
  {
    this->width = width_;
    this->height = height_;
    this->left = left_;
    this->right = right_;
    this->top = top_;
    this->bottom = bottom_;
    this->z_near = z_near_;
    this->z_far = z_far_;
    Orthographic_camera::update_projection_matrix();
  }
  
  std::string Orthographic_camera::to_string() const
  {
    std::string output("[Orthographic camera] :\n");
    
    output += "Y-axis --> " + y_axis.to_string();
    output += "Position on screen --> " + this->position.to_string();
    output += "Target --> " + this->forward.to_string();
    output += "Up --> " + this->up.to_string();
    output += "View matrix --> " + this->view_matrix.to_string();
    output += "Projection matrix --> " + this->projection_matrix.to_string();
    
    return output;
  }
  
  void Orthographic_camera::move(const Vector_3f &direction_, const float amount)
  {
    //TODO
    this->position += (direction_.normalize() * amount);
    update_view_matrix();
  }
  
  void Orthographic_camera::move(float x, float y, float z, float amount)
  {
    //TODO
    Vector_3f direction(x, y, z);
    this->position += (direction.normalize() * amount);
    update_view_matrix();
  }
  
  void Orthographic_camera::rotate_x(float angle)
  {
    //TODO
    Vector_3f horizontal_axis = y_axis.cross(this->forward);
    this->forward = this->forward.rotate(angle, horizontal_axis);
    this->up = this->forward.cross(horizontal_axis);
    update_view_matrix();
  }
  
  void Orthographic_camera::rotate_y(float angle)
  {
    //TODO
    Vector_3f horizontal_axis = y_axis.cross(this->forward);
    this->forward = this->forward.rotate(angle, y_axis);
    this->up = this->forward.cross(horizontal_axis);
    update_view_matrix();
  }
  
  void Orthographic_camera::update_view_matrix()
  {
    Matrix_4f rotation, translation;
    rotation.init_camera(get_forward(), get_up());
    translation.init_translation(Vector_3f(-get_position().get_x(),
                                           -get_position().get_y(),
                                           -get_position().get_z()));
    this->view_matrix = rotation * translation;
  }
  
  void Orthographic_camera::update_projection_matrix()
  {
    this->projection_matrix.init_orthographic_projection(this->width, this->height, this->left, this->right, this->top,
                                                         this->bottom,
                                                         this->z_near, this->z_far);
  }
  
}