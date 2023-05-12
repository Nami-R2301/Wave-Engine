//
// Created by nami on 22/04/23.
//

#include <Events/mouse_event.h>
#include <Scene/camera.h>

namespace Wave
{
  
  class Editor_camera : public Perspective_camera
  {
    public:
    Editor_camera() = default;
    Editor_camera(float width_, float height_, float fov_, float near_clip_, float far_clip_);
    
    void on_update(float ts);
    void on_event(Event &e) override;
    
    void set_distance(float distance_);
    void set_focal_point(float x, float y, float z);
    void set_focal_point(const Vector_3f &focal_point_);
    void calculate_orientation();
    void update_editor_view();
    
    INTERFACE_PRINT
    
    [[nodiscard]] float get_distance() const;
    [[nodiscard]] Vector_3f get_focal_point() const;
    [[nodiscard]] Matrix_4f get_view_projection() const;
    [[nodiscard]] float get_pitch() const;
    [[nodiscard]] float get_yaw() const;
    private:
    bool on_mouse_scroll(On_mouse_wheel_scroll &event);
    
    void mouse_pan(const Vector_2f &delta);
    void mouse_rotate(const Vector_2f &delta);
    void mouse_zoom(float delta);
    
    [[nodiscard]] Vector_3f calculate_position() const;
    
    [[nodiscard]] Vector_2f pan_speed() const;
    [[nodiscard]] static float rotation_speed();
    [[nodiscard]] float zoom_speed() const;
    private:
    Vector_3f focal_point = {0.0f, 0.0f, 5.0f};
    Vector_2f initial_mouse_position = {0.0f, 0.0f};
    float distance = 5.0f;
    float pitch = 0.0f, yaw = 0.0f;
  };
}