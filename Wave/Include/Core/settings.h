#pragma once

#include <Window/window.h>

namespace Wave
{
  
  class Display_settings
  {
  public:
    static void set_vsync(Window *window, bool state);
    static void toggle_fullscreen(Window *window);
    static void set_refresh_rate(Window *window, int refresh_rate);
  };
}