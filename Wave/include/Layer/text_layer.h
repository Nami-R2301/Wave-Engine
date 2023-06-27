//
// Created by nami on 24/04/23.
//

#pragma once

#include <Renderer/renderer.h>
#include <Renderer/shader.h>

#include <Scene/entity.h>
#include <Scene/camera.h>
#include <Layer/layer.h>
#include <Text/gl_text.h>

namespace Wave
{
  
  class Text_layer : public Layer
  {
    public:
    Text_layer(const std::vector<Entity> &entities_,
               const Math::Vector_2f *viewport_size,
               bool imgui_render = false);
    ~Text_layer() override;
    
    void on_attach() override;
    void on_detach() override;
    void on_update(float time_step) override;
    void on_event(Event &event) override;
    void on_render() override;
    void on_ui_render(float time_step) override;
    private:
    std::vector<Entity> entities;
    std::vector<std::shared_ptr<Text_box>> text_boxes;
    Orthographic_camera projection;
    const Math::Vector_2f *viewport_size;
    bool imgui_enabled;
  };
}
