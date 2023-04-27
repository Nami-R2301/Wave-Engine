//
// Created by nami on 12/04/23.
//

#pragma once

#include <Core/layer.h>

namespace Wave
{
  
  class ImGui_layer : public Layer
  {
  public:
    ImGui_layer();
    ~ImGui_layer() override = default;
    
    void on_attach() override;
    void on_detach() override;
    void on_update(float time_step) override;
    void on_ui_render(float time_step) override;
    void on_event(Event &e) override;
    
    static void begin();
    static void end();
    
    static void set_colors();
    [[maybe_unused]] [[nodiscard]] static uint32_t get_active_widget_id();
  };
}
