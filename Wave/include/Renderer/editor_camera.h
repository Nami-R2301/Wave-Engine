//
// Created by nami on 22/04/23.
//

#include <Events/mouse_event.h>
#include <Renderer/camera.h>

namespace Wave
{
  
  class Editor_camera : public Perspective_camera
  {
  public:
    Editor_camera() = default;
    Editor_camera(float width_, float height_, float fov_, float near_clip_, float far_clip_);
    
    void on_update(float ts);
    void on_event(Event& e) override;
    
    void set_distance(float distance_);
    [[nodiscard]] float get_distance() const;
    [[nodiscard]] Matrix_4f get_view_projection() const;
    [[nodiscard]] Vector_4f get_orientation() const;
    [[nodiscard]] float get_pitch() const;
    [[nodiscard]] float get_yaw() const;
  private:
    bool on_mouse_scroll(On_mouse_wheel_scroll& event);
    
    void mouse_pan(const Vector_2f& delta);
    void mouse_rotate(const Vector_2f& delta);
    void mouse_zoom(float delta);
    
    [[nodiscard]] Vector_3f calculate_position() const;
    
    [[nodiscard]] Vector_2f pan_speed() const;
    [[nodiscard]] float rotation_speed() const;
    [[nodiscard]] float zoom_speed() const;
  private:
    Vector_3f focal_point = {0.0f, 0.0f, 0.0f };
    Vector_2f initial_mouse_position = {0.0f, 0.0f };
    float distance = 10.0f;
    float pitch = 0.0f, yaw = 0.0f;
  };
}