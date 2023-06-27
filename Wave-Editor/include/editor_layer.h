//
// Created by nami on 23/04/23.
//

#pragma once

#include <wave.h>
#include <api_OpenGL.h>

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
    Editor_layer(const std::shared_ptr<Scene> &active_scene_,
                 const std::vector<Entity> &entities_,
                 const std::shared_ptr<Framebuffer> &viewport_);
    ~Editor_layer() override = default;
    
    void on_attach() override;
    void on_detach() override;
    
    void on_update(float time_step) override;
    void on_event(Wave::Event &e) override;
    void on_render() override;
    void on_ui_render(float time_step) override;
    private:
    static Scene_ui_panel scene_panel;
    std::vector<Entity> entities;
    std::shared_ptr<Wave::Camera> camera;
    std::vector<std::shared_ptr<Wave::Object>> objects;
    Framebuffer_draw_data framebuffer_viewport_data;
    int32_t guizmo_type = -1;
    private:
    static void draw_viewport_quad(const ImDrawList *parentList, const ImDrawCmd *cmd);
    
    friend class Editor;
  };
}
