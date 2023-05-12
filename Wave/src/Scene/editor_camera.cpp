//
// Created by nami on 22/04/23.
//

#include <Scene/editor_camera.h>
#include <Input/input.h>

namespace Wave
{
  
  Editor_camera::Editor_camera(float width_, float height_, float fov_, float near_clip_, float far_clip_) :
    Perspective_camera(width_, height_, fov_, near_clip_, far_clip_)
  {
  }
  
  std::string Editor_camera::to_string() const
  {
    std::string output("[Editor camera] :\n");
    
    output += "Camera type --> " + std::string(get_type());
    output += "Position --> " + this->position.to_string();
    output += "Focal point --> " + this->focal_point.to_string();
    output += "Forward --> " + this->forward.to_string();
    output += "Up --> " + this->up.to_string();
    output += "View matrix --> " + this->view_matrix.to_string();
    output += "Projection matrix --> " + this->projection_matrix.to_string();
    
    return output;
  }
  
  void Editor_camera::update_editor_view()
  {
    this->position = calculate_position();
    calculate_orientation();
  }
  
  void Editor_camera::on_update(float time_step)
  {
    if (Input::is_key_held(WAVE_KEY_LEFT_CONTROL))
    {
      Vector_2f delta = (Input::get_mouse_cursor_position() - this->initial_mouse_position) * 0.04587f;
      this->initial_mouse_position = Input::get_mouse_cursor_position();
      
      if (Input::is_mouse_button_held(WAVE_MOUSE_BUTTON_LEFT)) mouse_rotate(delta);
      if (Input::is_mouse_button_held(WAVE_MOUSE_BUTTON_MIDDLE)) mouse_pan(delta);
      if (Input::is_mouse_button_held(WAVE_MOUSE_BUTTON_RIGHT)) mouse_zoom(delta.get_y());
      update_editor_view();
    }
    // Synchronous tasks.
    float velocity = 15.0f;
    if (Input::is_key_held(WAVE_KEY_W)) this->move(this->get_up(), velocity * time_step);
    if (Input::is_key_held(WAVE_KEY_A)) this->move(this->get_left(), velocity * time_step);
    if (Input::is_key_held(WAVE_KEY_S)) this->move(this->get_up(), -velocity * time_step);
    if (Input::is_key_held(WAVE_KEY_D)) this->move(this->get_right(), velocity * time_step);
    Perspective_camera::update_view_matrix();
  }
  
  void Editor_camera::on_event(Event &e)
  {
    switch (e.get_event_type())
    {
      case Event_type::On_window_resize:
      {
        on_window_resize(dynamic_cast<On_window_resize &>(e));
        break;
      }
      case Event_type::On_mouse_wheel_scroll:
      {
        on_mouse_scroll(dynamic_cast<On_mouse_wheel_scroll &>(e));
        break;
      }
      default:return;
    }
  }
  
  Vector_3f Editor_camera::get_focal_point() const
  {
    return this->focal_point;
  }
  
  void Editor_camera::set_focal_point(float x, float y, float z)
  {
    this->focal_point = Vector_3f(x, y, z);
  }
  
  void Editor_camera::set_focal_point(const Vector_3f &focal_point_)
  {
    this->focal_point = focal_point_;
  }
  
  float Editor_camera::get_distance() const
  {
    return this->distance;
  }
  
  void Editor_camera::set_distance(float distance_)
  {
    this->distance = distance_;
  }
  
  Matrix_4f Editor_camera::get_view_projection() const
  {
    return this->projection_matrix * this->view_matrix;
  }
  
  void Editor_camera::calculate_orientation()
  {
    this->orientation = Vector_4f(Vector_3f(-this->pitch, -this->yaw, 0.0f));
  }
  
  Vector_3f Editor_camera::calculate_position() const
  {
    return this->focal_point - get_forward() * this->distance;
  }
  
  float Editor_camera::get_pitch() const
  {
    return this->pitch;
  }
  
  float Editor_camera::get_yaw() const
  {
    return this->yaw;
  }
  
  bool Editor_camera::on_mouse_scroll(On_mouse_wheel_scroll &event)
  {
    float delta = event.get_mouse_wheel_offset().get_y() * 0.1f;
    this->mouse_zoom(delta);
    update_editor_view();
    Perspective_camera::update_view_matrix();
    return false;
  }
  
  void Editor_camera::mouse_pan(const Vector_2f &delta)
  {
    Vector_2f speed = pan_speed();
    Vector_3f inverse_right = {-get_right().get_x(), -get_right().get_y(), -get_right().get_z()};
    
    this->focal_point += inverse_right * delta.get_x() * speed.get_x() * this->distance;
    this->focal_point += get_up() * delta.get_y() * speed.get_y() * this->distance;
  }
  
  void Editor_camera::mouse_rotate(const Vector_2f &delta)
  {
    float yaw_sign = get_up().get_y() < 0 ? -1.0f : 1.0f;
    this->yaw += yaw_sign * delta.get_x() * rotation_speed();
    this->pitch += delta.get_y() * (rotation_speed() * this->aspect_ratio);
  }
  
  void Editor_camera::mouse_zoom(float delta)
  {
    this->distance -= delta * zoom_speed();
    if (this->distance < 1.0f)
    {
      this->focal_point += get_forward();
      this->distance = 1.0f;
    }
  }
  
  Vector_2f Editor_camera::pan_speed() const
  {
    float x = std::min(this->width / 1000.0f, 2.7f);
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
    
    float y = std::min(this->height / 1000.0f, 2.7f);
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
    
    return {xFactor, yFactor};
  }
  
  float Editor_camera::rotation_speed()
  {
    return 0.65f;
  }
  
  float Editor_camera::zoom_speed() const
  {
    float distance_ = this->distance * 0.1f;
    distance_ = std::max(distance_, 0.0f);
    float speed = distance_ * distance_;
    speed = std::min(speed, 100.0f); // max speed = 100
    return speed;
  }
}