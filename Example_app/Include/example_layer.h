//
// Created by nami on 29/03/23.
//

#pragma once

#include "Events/key_event.h"
#include <wave.h>
#include <ImGUI/imgui.h>

class Example_scene_3D : public Wave::Layer
{
public:
  Example_scene_3D(const std::shared_ptr<Wave::Camera> &demo_perspective_camera_,
                   const std::vector<std::shared_ptr<Wave::Shader>> &shaders_,
                   const std::shared_ptr<Wave::Object_3D> &demo_object_);
  ~Example_scene_3D() override = default;
  
  void on_attach() override;
  void on_detach() override;
  
  void on_update(float time_step) override;
  void on_event(Wave::Event &e) override;
private:
  std::shared_ptr<Wave::Camera> demo_camera;
  std::vector<std::shared_ptr<Wave::Shader>> shaders;
  std::shared_ptr<Wave::Object_3D> object;
};
