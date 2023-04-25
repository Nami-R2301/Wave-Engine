//
// Created by nami on 01/04/23.
//

#pragma once

#include <wave.h>

class Example_app : public Wave::Engine
{
public:
  Example_app();
  ~Example_app() override = default;
  
  void init() override;
  void on_update(float time_step) override;
  void on_event(Wave::Event &event) override;
  
  bool window_closed_callback([[maybe_unused]] Wave::On_window_close &window_closed_event) override;
  bool window_resize_callback([[maybe_unused]] Wave::On_window_resize &window_resized_event) override;
  bool mouse_movement_callback(Wave::On_mouse_movement &mouse_cursor_event) override;
  bool mouse_wheel_callback(Wave::On_mouse_wheel_scroll &mouse_wheel_input) override;
  
  std::vector<std::shared_ptr<Wave::Shader>> demo_object_shaders;
  std::vector<std::shared_ptr<Wave::Shader>> demo_text_shaders;
  std::shared_ptr<Wave::Editor_camera> demo_perspective_camera;
  std::vector<std::shared_ptr<Wave::Object_3D>> demo_objects;
  std::vector<std::shared_ptr<Wave::Text>> demo_text;
};