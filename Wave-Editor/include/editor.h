//
// Created by nami on 23/04/23.
//

#pragma once

#include <wave.h>
#include <imGUI/imgui_internal.h>

namespace Wave
{
  
  class Editor : public Engine
  {
    public:
    Editor();
    ~Editor() override = default;
    
    void init() override;
    void on_update(float time_step) override;
    
    std::vector<std::shared_ptr<Wave::Shader>> demo_shaders;
    std::shared_ptr<Wave::Editor_camera> editor_camera;
    std::vector<std::shared_ptr<Wave::Object_3D>> demo_objects;
    std::vector<std::shared_ptr<Text>> demo_texts;
    std::shared_ptr<Framebuffer> viewport_framebuffer;
    Color background_clear_color = Color(78.0f, 255.0f, false);  // Light gray.
    Vector_2f viewport_resolution = {0, 0};
    Vector_4f viewport_framebuffer_boundaries;
  };
}
