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
    void on_event(Wave::Event &event) override;
    
    bool window_closed_callback([[maybe_unused]] Wave::On_window_close &window_closed_event) override;
    bool window_resize_callback([[maybe_unused]] Wave::On_window_resize &window_resized_event) override;
    
    std::vector<std::shared_ptr<Wave::Shader>> demo_shaders;
    std::shared_ptr<Wave::Editor_camera> demo_perspective_camera;
    std::vector<std::shared_ptr<Wave::Object_3D>> demo_objects;
    std::vector<std::shared_ptr<Text>> demo_texts;
    std::shared_ptr<Framebuffer> viewport_framebuffer;
    Vector_2f viewport_resolution = {0,
                                     0};
    Vector_4f viewport_framebuffer_boundaries;
  };
}
