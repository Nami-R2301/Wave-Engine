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
  
  void on_init() override;
  void on_update(float time_step) override;
  void on_event(Wave::Event &event) override;
  void on_game_render() override;
  void on_destroy() override;
  
  bool window_closed_callback([[maybe_unused]] Wave::On_window_close &window_closed_event) override;
  bool window_resize_callback([[maybe_unused]] Wave::On_window_resize &window_resized_event) override;
  bool mouse_movement_callback(Wave::On_mouse_movement &mouse_cursor_event) override;
  bool mouse_wheel_callback(Wave::On_mouse_wheel_scroll &mouse_wheel_input) override;
  
  std::vector<Wave::Entity> entities;
  std::shared_ptr<Wave::Scene> active_scene;
  Wave::Color background_clear_color = Wave::Color(78.0f, 255.0f, false);
  Wave::Math::Vector_2f viewport;
};