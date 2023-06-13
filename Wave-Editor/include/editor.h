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
    
    void on_init() override;
    void on_event(Event &event) override;
    bool on_viewport_resize(On_framebuffer_resize &resize);
    void on_update(float time_step) override;
    void on_game_render() override;
    void on_ui_render(float time_step) override;
    void on_destroy() override;
    
    std::vector<std::shared_ptr<Wave::Shader>> demo_shaders;
    std::vector<Entity> entities;
    std::shared_ptr<Scene> active_scene;
    std::shared_ptr<Framebuffer> viewport_framebuffer;
    Color background_clear_color = Color(75.0f, 255.0f, false);  // Light gray.
    Vector_2f viewport_resolution = {0, 0};
    Vector_4f viewport_framebuffer_boundaries;
  };
}
