//
// Created by nami on 5/25/22.
//

#pragma once

#include <Events/app_event.h>
#include <Math/matrix_4f.h>

namespace Wave
{
  
  class Camera : public Printable
  {
    public:
    Camera() = default;
    ~Camera() override = default;
    
    [[nodiscard]] float get_width() const
    {
      return this->width;
    };
    
    [[nodiscard]] float get_height() const
    {
      return this->height;
    };
    
    [[nodiscard]] const Vector_2f &get_center_position() const
    {
      return this->center_position;
    };
    
    [[nodiscard]] const Vector_3f &get_position() const
    {
      return this->position;
    };
    
    [[nodiscard]] Vector_3f &get_position()
    {
      return this->position;
    };
    
    [[nodiscard]] Vector_3f get_forward() const
    {
      return this->forward;
    };
    
    [[nodiscard]] const Vector_4f &get_orientation() const
    {
      return this->orientation;
    };
    
    [[nodiscard]] Vector_3f get_up() const
    {
      return this->up;
    };
    
    [[nodiscard]] Vector_3f get_left() const
    {
      return this->forward.cross(this->up).normalize();
    };
    
    [[nodiscard]] Vector_3f get_right() const
    {
      return this->up.cross(this->forward).normalize();
    };
    
    [[nodiscard]] const Matrix_4f &get_view_matrix() const
    {
      return this->view_matrix;
    };
    
    [[nodiscard]] Matrix_4f &get_view_matrix()
    {
      return this->view_matrix;
    };
    
    [[nodiscard]] const Matrix_4f &get_projection_matrix() const
    {
      return this->projection_matrix;
    };
    
    [[nodiscard]] Matrix_4f &get_projection_matrix()
    {
      return this->projection_matrix;
    };
    
    void set_width(float width_)
    {
      this->width = width_;
    };
    
    void set_height(float height_)
    {
      this->height = height_;
    };
    
    void set_aspect_ratio(float ar)
    {
      this->aspect_ratio = ar;
    };
    
    void set_viewport(float width_, float height_)
    {
      set_width(width_);
      set_height(height_);
      set_aspect_ratio(width / height_);
      set_center_position((width_ / 2.0f), (height_ / 2.0f));
    }
    
    void set_center_position(float x, float y)
    {
      this->center_position = Vector_2f(x, y);
    };
    
    void set_center_position(const Vector_2f &coords)
    {
      this->center_position = coords;
    };
    
    void set_position(const Vector_3f &position_)
    {
      this->position = position_;
      update_view_matrix();
    };
    
    void set_orientation(float x, float y, float z, float w)
    {
      this->orientation = Vector_4f(x, y, z, w);
    }
    
    void set_orientation(const Vector_4f &orientation_)
    {
      this->orientation = orientation_;
    };
    
    void set_position(float x, float y, float z)
    {
      this->position = Vector_3f(x, y, z);
      update_view_matrix();
    };
    
    void set_view_matrix(const Matrix_4f &matrix)
    {
      this->view_matrix = matrix;
    };
    
    void set_forward(const Vector_3f &forward_)
    {
      this->forward = forward_.normalize();
    };
    
    void set_up(const Vector_3f &up_)
    {
      this->up = up_.normalize();
    };
    
    [[nodiscard]] virtual const char *get_type() const = 0;
    virtual void on_window_resize(On_window_resize &resize_event) = 0;
    virtual void on_framebuffer_resize(On_framebuffer_resize &resize_event) = 0;
    virtual void on_update(float time_step) = 0;
    virtual void on_event(Event &event) = 0;
    virtual void move(const Vector_3f &direction, float amount) = 0;
    virtual void move(float x, float y, float z, float amount) = 0;
    virtual void rotate_x(float angle) = 0;
    virtual void rotate_y(float angle) = 0;
    virtual void update_view_matrix() = 0;
    virtual void update_projection_matrix() = 0;
    protected:
    float width = 640;
    float height = 480;
    float aspect_ratio = 640.0f / 480.0f;
    Vector_2f center_position = Vector_2f(0);
    Vector_3f position = Vector_3f(0, 0, -1);
    Vector_3f forward = Vector_3f(0);
    Vector_3f up = Vector_3f(0);
    Vector_4f orientation = Vector_4f(0);
    Matrix_4f view_matrix{};
    Matrix_4f projection_matrix{};
  };
  
  class Perspective_camera : public Camera
  {
    public:
    Perspective_camera() = default;
    Perspective_camera(float width_, float height_, float fov_, float z_near_, float z_far_);
    ~Perspective_camera() override = default;
    
    void on_window_resize(On_window_resize &resize_event) override;
    void on_framebuffer_resize(On_framebuffer_resize &resize_event) override;
    void update_view_matrix() override;
    void update_projection_matrix() override;
    void look_at(const Vector_3f &direction);
    void look_at(float x, float y, float z);
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] const char *get_type() const override;
    void on_update(float time_step) override;
    void on_event(Event &event) override;
    void move(const Vector_3f &direction, float amount) override;
    void move(float x, float y, float z, float amount) override;
    void rotate_x(float angle) override;
    void rotate_y(float angle) override;
    protected:
    float fov = 45.0f;
    float z_near = 0.1f;
    float z_far = 1000.0f;
  };
  
  class Orthographic_camera : public Camera
  {
    public:
    Orthographic_camera() = default;
    Orthographic_camera(float width_, float height_, float z_near_, float z_far_);
    ~Orthographic_camera() override = default;
    
    void on_event(Event &event) override;
    void on_window_resize(On_window_resize &resize_event) override;
    void on_framebuffer_resize(On_framebuffer_resize &resize_event) override;
    void update_view_matrix() override;
    void update_projection_matrix() override;
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] const char *get_type() const override;
    void on_update(float time_step) override;
    void move(const Vector_3f &direction, float amount) override;
    void move(float x, float y, float z, float amount) override;
    void rotate_x(float angle) override;
    void rotate_y(float angle) override;
    protected:
    float size = 1.0f;
    float z_near = -1.0f;
    float z_far = 1.0f;
  };
}
