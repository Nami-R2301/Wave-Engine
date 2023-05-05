//
// Created by nami on 23/04/23.
//

#pragma once

#include <wave.h>

namespace Wave
{
  
  typedef struct Framebuffer_draw_data
  {
    std::shared_ptr<Shader> framebuffer_viewport_shader;
    std::shared_ptr<Framebuffer> viewport;
  } Framebuffer_draw_data;
  
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
    
    static void draw_viewport_quad(const ImDrawList *parentList, const ImDrawCmd *cmd);
    bool dockSpace_open = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    Framebuffer_draw_data framebuffer_viewport_data;
    ImGuiID dockSpace_id = 0, viewport_panel_dock_id = 0, scene_panel_dock_id = 0, stats_panel_dock_id = 0;
    private:
    std::shared_ptr<Wave::Camera> camera;
    std::vector<std::shared_ptr<Wave::Shader>> shaders;
    std::vector<std::shared_ptr<Wave::Object_3D>> objects;
  };
}
