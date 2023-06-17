//
// Created by nami on 18/05/23.
//

#pragma once

#include <Core/interfaces.h>
#include <imGUI/imgui.h>
#include <imGUI/imgui_internal.h>
#include <Renderer/color.h>

#include <Scene/scene.h>
#include <Scene/entity.h>

namespace Wave
{
  
  typedef struct Scene_ui_data_s
  {
    Vector_2f origin = Vector_2f(0.0f);  // Position on screen.
    Vector_4f size_boundaries = Vector_4f(0.0f, 0.0f, 8000.0f, 8000.0f); // In pixels.
    Color background_color = Color(0.15f, 1.0f, true);
    float font_scale = 1.0f;  // Size of text in the panel.
  } Scene_ui_data_s;
  
  // Active scene hierarchy displayed in the UI for the shown entities in the framebuffer.
  class Scene_ui_panel : public I_movable, public I_copiable
  {
    public:
    Scene_ui_panel() = default;
    Scene_ui_panel(const std::shared_ptr<Scene> &context_, const Scene_ui_data_s &scene_ui_data_);
    ~Scene_ui_panel() override = default;
    
    // Interfaces implementation.
    INTERFACE_MOVABLE;
    INTERFACE_COPIABLE;
    
    void on_ui_render();
    
    [[nodiscard]] const Vector_2f &get_position() const;
    [[nodiscard]] const Vector_4f &get_size_boundaries() const;
    [[nodiscard]] const Color &get_background_color() const;
    [[nodiscard]] float get_font_scale() const;
    
    void set_position(const Vector_2f &position_);
    void set_size_boundaries(const Vector_4f &size_boundaries_);
    void set_background_color(const Color &background_color_);
    void set_font_scale(float font_scale_);
    private:
    Scene_ui_data_s scene_ui_data;
    std::shared_ptr<Scene> context;
    bool moved = false;
    private:
    void display_entity(Entity entity);
  };
}
