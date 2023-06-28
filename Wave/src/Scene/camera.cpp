//
// Created by nami on 5/25/22.
//

#include <Scene/camera.h>

namespace Wave
{
  
  static const Math::Vector_3f y_axis = Math::Vector_3f(0, 1, 0);
  
  /**************************************** PERSPECTIVE CAMERA *************************************/
  
  Perspective_camera::Perspective_camera(float width_, float height_, float fov_, float z_near_, float z_far_)
  {
    this->width = width_;
    this->height = height_;
    this->fov = fov_;
    this->z_near = z_near_;
    this->z_far = z_far_;
    this->position = Math::Vector_3f(0, 0, 0);
    this->forward = Math::Vector_3f(0, 0, 1);
    this->up = Math::Vector_3f(0, 1, 0);
    this->center_position = Math::Vector_2f(static_cast<float>(this->width) / 2.0f,
                                            static_cast<float>(this->height) / 2.0f);
    Perspective_camera::update_projection_matrix();
  }
  
  const char *Perspective_camera::get_type() const
  {
    return "Perspective\n";
  }
  
  std::string Perspective_camera::to_string() const
  {
    std::string output("[Perspective camera] :\n");
    
    output += "Camera type --> " + std::string(get_type());
    output += "Y-axis --> " + y_axis.to_string();
    output += "Position --> " + this->position.to_string();
    output += "Target --> " + this->forward.to_string();
    output += "Up --> " + this->up.to_string();
    output += "View matrix --> " + this->view_matrix.to_string();
    output += "Projection matrix --> " + this->projection_matrix.to_string();
    
    return output;
  }
  
  void Perspective_camera::on_update([[maybe_unused]] float time_step)
  {
  }
  
  void Perspective_camera::on_event(Event_system::Event &event)
  {
    switch (event.get_event_type())
    {
      case Event_system::Event_type::On_window_resize:
      {
        on_window_resize(dynamic_cast<Event_system::On_window_resize &>(event));
      }
      default:break;
    }
  }
  
  void Perspective_camera::on_window_resize(Event_system::On_window_resize &resize_event)
  {
    auto width_ = static_cast<float>(resize_event.get_width());
    auto height = static_cast<float>(resize_event.get_height());
    set_viewport(width_, height);
  }
  
  void Perspective_camera::on_framebuffer_resize(Event_system::On_framebuffer_resize &resize_event)
  {
    auto width_ = static_cast<float>(resize_event.get_width());
    auto height = static_cast<float>(resize_event.get_height());
    set_viewport(width_, height);
  }
  
  void Perspective_camera::move(const Math::Vector_3f &direction_, const float amount)
  {
    this->position += (direction_.normalize() * amount);
  }
  
  void Perspective_camera::move(float x, float y, float z, float amount)
  {
    Math::Vector_3f direction(x, y, z);
    this->position += (direction.normalize() * amount);
  }
  
  void Perspective_camera::rotate_x(float angle)
  {
    Math::Vector_3f horizontal_axis = y_axis.cross(this->forward);
    this->forward = this->forward.rotate(angle, horizontal_axis);
    this->up = this->forward.cross(horizontal_axis);
    update_view_matrix();
  }
  
  void Perspective_camera::rotate_y(float angle)
  {
    Math::Vector_3f horizontal_axis = y_axis.cross(this->forward);
    this->forward = this->forward.rotate(angle, y_axis);
    this->up = this->forward.cross(horizontal_axis);
    update_view_matrix();
  }
  
  void Perspective_camera::look_at(const Math::Vector_3f &direction_)
  {
    this->forward = direction_;
    update_view_matrix();
  }
  
  void Perspective_camera::look_at(float x, float y, float z)
  {
    this->forward = Math::Vector_3f(x, y, z);
    update_view_matrix();
  }
  
  void Perspective_camera::update_view_matrix()
  {
    Math::Matrix_4f rotation, translation, orientation;
    orientation.init_rotation(this->orientation.get_x(), this->orientation.get_y(), this->orientation.get_z());
    rotation.init_camera(this->forward, this->up);
    translation.init_translation(Math::Vector_3f(-get_position().get_x(),
                                                 -get_position().get_y(),
                                                 -get_position().get_z()));
    this->view_matrix = orientation * rotation * translation;
    this->view_matrix.transpose();
  }
  
  void Perspective_camera::update_projection_matrix()
  {
    this->projection_matrix.init_perspective_projection(this->fov,
                                                        this->z_near,
                                                        this->z_far);
    this->projection_matrix.transpose();
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
  
  const char *Orthographic_camera::get_type() const
  {
    return "Orthographic\n";
  }
  
  std::string Orthographic_camera::to_string() const
  {
    std::string output("[Orthographic camera] :\n");
    
    output += "Camera type --> " + std::string(get_type());
    output += "Position --> " + this->position.to_string();
    output += "Forward --> " + this->forward.to_string();
    output += "Up --> " + this->up.to_string();
    output += "View matrix --> " + this->view_matrix.to_string();
    output += "Projection matrix --> " + this->projection_matrix.to_string();
    
    return output;
  }
  
  void Orthographic_camera::on_update([[maybe_unused]] float time_step)
  {
  }
  
  void Orthographic_camera::on_event([[maybe_unused]] Event_system::Event &event)
  {
  }
  
  void Orthographic_camera::on_window_resize([[maybe_unused]] Event_system::On_window_resize &resize_event)
  {
    auto width_ = static_cast<float>(resize_event.get_width());
    auto height = static_cast<float>(resize_event.get_height());
    set_viewport(width_, height);
  }
  
  void Orthographic_camera::on_framebuffer_resize(Event_system::On_framebuffer_resize &resize_event)
  {
    auto width_ = static_cast<float>(resize_event.get_width());
    auto height = static_cast<float>(resize_event.get_height());
    set_viewport(width_, height);
    alert(WAVE_LOG_DEBUG, "Framebuffer resized to (%.2f, %.2f)", resize_event.get_width(),
          resize_event.get_height());
  }
  
  void Orthographic_camera::move(const Math::Vector_3f &direction_, const float amount)
  {
    //TODO
    this->position += (direction_.normalize(this->width, this->height) * this->aspect_ratio * amount);
    update_view_matrix();
  }
  
  void Orthographic_camera::move(float x, float y, float z, float amount)
  {
    //TODO
    Math::Vector_3f direction(x, y, z);
    this->position += (direction.normalize(this->width, this->height) * this->aspect_ratio * amount);
    update_view_matrix();
  }
  
  void Orthographic_camera::rotate_x(float angle)
  {
    //TODO
    Math::Vector_3f horizontal_axis = y_axis.cross(this->forward);
    this->forward = this->forward.rotate(angle, horizontal_axis);
    this->up = this->forward.cross(horizontal_axis);
    update_view_matrix();
  }
  
  void Orthographic_camera::rotate_y(float angle)
  {
    //TODO
    Math::Vector_3f horizontal_axis = y_axis.cross(this->forward);
    this->forward = this->forward.rotate(angle, y_axis);
    this->up = this->forward.cross(horizontal_axis);
    update_view_matrix();
  }
  
  void Orthographic_camera::update_view_matrix()
  {
    Math::Matrix_4f rotation, translation;
    rotation.init_camera(get_forward(), get_up());
    translation.init_translation(Math::Vector_3f(-get_position().get_x(),
                                                 -get_position().get_y(),
                                                 -get_position().get_z()));
    this->view_matrix = rotation * translation;
  }
  
  void Orthographic_camera::update_projection_matrix()
  {
    this->projection_matrix.init_orthographic_projection(0,
                                                         this->width,
                                                         this->height,
                                                         0,
                                                         this->z_near,
                                                         this->z_far);
    this->projection_matrix.transpose();
  }
}