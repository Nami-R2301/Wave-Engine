//
// Created by nami on 5/25/22.
//

#include <Renderer/camera.h>

namespace Wave
{
  
  static const Vector_3f y_axis = Vector_3f(0, 1, 0);
  
  /**************************************** PERSPECTIVE CAMERA *************************************/
  
  Perspective_camera::Perspective_camera(float width_, float height_, float fov_, float z_near_, float z_far_)
  {
    this->width = width_;
    this->height = height_;
    this->fov = fov_;
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
  
  void Perspective_camera::on_event(Event &event)
  {
    switch(event.get_event_type())
    {
      case Event_type::On_window_resize:
      {
        on_window_resize(dynamic_cast<On_window_resize &>(event));
      }
      default:break;
    }
  }
  
  void Perspective_camera::on_window_resize(On_window_resize &resize_event)
  {
    auto width_ = static_cast<float>(resize_event.get_width());
    auto height = static_cast<float>(resize_event.get_height());
    set_viewport(width_, height);
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
    this->projection_matrix.init_perspective_projection(this->fov,
                                                        this->z_near,
                                                        this->z_far);
  }
  
  /******************************************** ORTHOGRAPHIC CAMERA ******************************************/
  
  Orthographic_camera::Orthographic_camera(float width_, float height_, float z_near_, float z_far_)
  {
    this->width = width_;
    this->height = height_;
    this->aspect_ratio = width_ / height_;
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
  
  void Orthographic_camera::on_event(Event &event)
  {
  
  }
  
  void Orthographic_camera::on_window_resize(On_window_resize &resize_event)
  {
  
  }
  
  void Orthographic_camera::move(const Vector_3f &direction_, const float amount)
  {
    //TODO
    this->position += (direction_.normalize(this->width, this->height) * this->aspect_ratio * amount);
    update_view_matrix();
  }
  
  void Orthographic_camera::move(float x, float y, float z, float amount)
  {
    //TODO
    Vector_3f direction(x, y, z);
    this->position += (direction.normalize(this->width, this->height) * this->aspect_ratio * amount);
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
    this->projection_matrix.init_orthographic_projection(0,
                                                         this->width * this->aspect_ratio,
                                                         this->height * this->aspect_ratio,
                                                         0,
                                                         this->z_near,
                                                         this->z_far);
  }
}