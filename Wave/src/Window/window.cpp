//
// Created by nami on 9/5/22.
//

#include <Window/glfw/glfw_window.h>

namespace Wave
{
  
  std::unique_ptr<Window> Window::create(Context_api api_chosen)
  {
    switch (api_chosen)
    {
      case Context_api::Glfw:return std::make_unique<Glfw_window>();
      case Context_api::Glut:
      {
        Wave::alert(WAVE_WARN, "[Window] --> Glut is not supported yet! Glfw used instead.");
        break;
      }
      case Context_api::Win32:
      {
        Wave::alert(WAVE_WARN, "[Window] --> Win32 is not supported yet! Glfw used instead.");
        break;
      }
      default:return std::make_unique<Glfw_window>();
    }
    return std::make_unique<Glfw_window>();
  }
  
  std::function<void(Event &event)> Window::event_callback_function;
  
  const std::function<void(Event &event)> &Window::get_event_callback_function()
  {
    return Window::event_callback_function;
  }
}