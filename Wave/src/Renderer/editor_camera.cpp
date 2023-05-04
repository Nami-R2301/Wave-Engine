//
// Created by nami on 22/04/23.
//

#include <Events/mouse_event.h>
#include <Renderer/editor_camera.h>
#include <Input/input.h>
#include <glm/glm/gtx/quaternion.hpp>

namespace Wave
{
  
  Editor_camera::Editor_camera(float width_, float height_, float fov_, float near_clip_, float far_clip_) :
      Perspective_camera(width_, height_, fov_, near_clip_, far_clip_)
  {
  
  }
  
  void Editor_camera::on_update([[maybe_unused]] float time_step)
  {
    if (Input::is_key_pressed(WAVE_KEY_LEFT_ALT))
    {
      const Vector_2f &mouse {Input::get_mouse_cursor_position().get_x(),
                              Input::get_mouse_cursor_position().get_y()};
      Vector_2f delta = (mouse - this->initial_mouse_position) * 0.003f;
      this->initial_mouse_position = mouse;

      if (Input::is_mouse_button_pressed(WAVE_MOUSE_BUTTON_MIDDLE))
        mouse_pan(delta);
      else if (Input::is_mouse_button_pressed(WAVE_MOUSE_BUTTON_LEFT))
        mouse_rotate(delta);
      else if (Input::is_mouse_button_pressed(WAVE_MOUSE_BUTTON_RIGHT))
        mouse_zoom(delta.get_y());
    }
    Perspective_camera::update_view_matrix();
  }
  
  void Editor_camera::on_event(Event &e)
  {
    switch(e.get_event_type())
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
      default:break;
    }
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
    this->position = this->calculate_position();
    this->calculate_orientation();
    Vector_4f orientation = this->get_orientation();
    glm::quat quat = {orientation.get_x(),
                      orientation.get_y(),
                      orientation.get_z(),
                      orientation.get_w()};
    glm::vec3 pos = {this->position.get_x(),
                     this->position.get_y(),
                     this->position.get_z()};
    auto view = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(quat);
    auto test = glm::inverse(view);
    Perspective_camera::update_view_matrix();
    return false;
  }
  
  void Editor_camera::mouse_pan(const Vector_2f &delta)
  {
    Vector_2f speed = pan_speed();
    Vector_3f inverse_right = {-get_right().get_x(), -get_right().get_y(), -get_right().get_z()};
    this->focal_point += inverse_right * delta.get_x() * speed.get_x() * this->distance;
    this->focal_point += get_right() * delta.get_y() * speed.get_y() * this->distance;
  }
  
  void Editor_camera::mouse_rotate(const Vector_2f &delta)
  {
    float yaw_sign = get_up().get_y() < 0 ? -1.0f : 1.0f;
    this->yaw += yaw_sign * delta.get_x() * rotation_speed();
    this->pitch += delta.get_y() * rotation_speed();
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
  
  Vector_3f Editor_camera::calculate_position() const
  {
    return this->focal_point - get_forward() * this->distance;
  }
  
  Vector_2f Editor_camera::pan_speed() const
  {
    float x = std::min(this->width / 1000.0f, 2.4f); // max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
    
    float y = std::min(this->height / 1000.0f, 2.4f); // max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
    
    return { xFactor, yFactor };
  }
  
  float Editor_camera::rotation_speed() const
  {
    return 0.8f;
  }
  
  float Editor_camera::zoom_speed() const
  {
    float distance_ = this->distance * 0.2f;
    distance_ = std::max(distance_, 0.0f);
    float speed = distance_ * distance_;
    speed = std::min(speed, 100.0f); // max speed = 100
    return speed;
  }
}