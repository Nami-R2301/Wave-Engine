//
// Created by nami on 23/04/23.
//

#pragma once

#include <wave.h>

namespace Wave
{
  
  class Editor : public Engine
  {
    public:
    Editor();
    ~Editor() override = default;
    
    void load() override;
    void on_update(float time_step) override;
    void on_render() override;
    
    std::vector<std::shared_ptr<Wave::Shader>> demo_shaders;
    std::shared_ptr<Wave::Camera> editor_camera;
    std::vector<std::shared_ptr<Wave::Object>> demo_objects;
    std::vector<std::shared_ptr<Text_box>> demo_texts;
    std::shared_ptr<Framebuffer> viewport_framebuffer;
    Color background_clear_color = Color(78.0f, 255.0f, false);  // Light gray.
    Vector_2f viewport_resolution = {0, 0};
    Vector_4f viewport_framebuffer_boundaries;
  };
}
