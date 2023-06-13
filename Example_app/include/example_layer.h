//
// Created by nami on 29/03/23.
//

#pragma once

#include <wave.h>

class Example_scene_3D : public Wave::Layer
{
  public:
  Example_scene_3D(std::vector<Wave::Entity> &entities_,
                   bool imgui_render_ = false);
  ~Example_scene_3D() override = default;
  
  void on_attach() override;
  void on_detach() override;
  
  void on_update(float time_step) override;
  void on_event(Wave::Event &e) override;
  void on_render() override;
  void on_ui_render(float time_step) override;
  private:
  std::shared_ptr<Wave::Camera> camera;
  std::vector<std::shared_ptr<Wave::Shader>> shaders;
  std::vector<std::shared_ptr<Wave::Object>> objects;
  bool imgui_render = false;
};
