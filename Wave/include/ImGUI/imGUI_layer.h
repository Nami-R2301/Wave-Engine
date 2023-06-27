//
// Created by nami on 12/04/23.
//

#pragma once

#include <Layer/layer.h>
#include <imgui_internal.h>
#include <imgui.h>
#include <ImGuizmo/ImGuizmo.h>

namespace Wave
{
  
  class ImGui_layer : public Layer
  {
    public:
    typedef struct ImGui_layer_data_s
    {
      float font_size = 16.0f;
      float font_scale = 1.0f;
    } ImGui_layer_data_s;
    public:
    explicit ImGui_layer(ImGui_layer_data_s imgui_data_ = {16.0f, 1.0f});
    ~ImGui_layer() override = default;
    
    void on_attach() override;
    void on_detach() override;
    void on_update(float time_step) override;
    void on_render() override;
    void on_ui_render(float time_step) override;
    void on_event(Event &e) override;
    
    static void begin();
    static void end();
    
    static void draw_property(const std::string &label, Math::Vector_2f &values, Math::Vector_2f min_value,
                              Math::Vector_2f max_value,
                              Math::Vector_2f speed_value, Math::Vector_2f reset_value = Math::Vector_2f(0.0f),
                              float column_width = 100.0f);
    static void display_property(const std::string &label, Math::Vector_3f &values, Math::Vector_3f min_value,
                                 Math::Vector_3f max_value,
                                 Math::Vector_3f speed_value, Math::Vector_3f reset_value = Math::Vector_3f(0.0f),
                                 float column_width = 100.0f);
    static void set_colors();
    [[maybe_unused]] [[nodiscard]] static uint32_t get_active_widget_id();
    public:
    static bool dockSpace_open, show_scene_panel, show_system_panel;
    static ImGuiWindowFlags window_flags;
    static ImGuiID dockSpace_id, viewport_panel_dock_id, scene_panel_dock_id, events_panel_dock_id, info_panel_dock_id;
    static ImGui_layer_data_s imgui_data;
  };
}
