//
// Created by nami on 8/30/22.
//

#include <Core/settings.h>

namespace Wave
{
  void Display_settings::set_vsync(Window *window, bool state)
  {
    window->set_vsync(state);
    Wave::alert(WAVE_LOG_INFO, "[SETTING] --> Set vsync %s", state ? "ON" : "OFF");
  }
  
  void Display_settings::toggle_fullscreen(Window *window)
  {
    window->toggle_fullscreen();
    Wave::alert(WAVE_LOG_INFO, "[SETTING] --> Set fullscreen %s", window->is_fullscreen() ? "ON" : "OFF");
  }
  
  void Display_settings::set_refresh_rate(Window *window, int32_t refresh_rate)
  {
    window->set_refresh_rate(refresh_rate);
    Wave::alert(WAVE_LOG_INFO, "[SETTING] --> Set on_render rate to %d", refresh_rate);
  }
}