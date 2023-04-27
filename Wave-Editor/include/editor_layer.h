//
// Created by nami on 23/04/23.
//

#pragma once

#include <wave.h>

namespace Wave
{
  
  class Editor_layer : public Layer
  {
  public:
    Editor_layer(const std::shared_ptr<Wave::Camera> &demo_perspective_camera_,
                 const std::vector<std::shared_ptr<Wave::Shader>> &shaders_,
                 const std::vector<std::shared_ptr<Wave::Object_3D>> &objects_,
                 const std::shared_ptr<Framebuffer> &viewport_);
    ~Editor_layer() override = default;
    
    void on_attach() override;
    void on_detach() override;
    
    void on_update(float time_step) override;
    void on_event(Wave::Event &e) override;
    void on_ui_render(float time_step) override;
  private:
    std::shared_ptr<Wave::Camera> camera;
    std::vector<std::shared_ptr<Wave::Shader>> shaders;
    std::vector<std::shared_ptr<Wave::Object_3D>> objects;
    std::shared_ptr<Framebuffer> viewport;
  };
}
