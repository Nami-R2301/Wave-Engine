//
// Created by nami on 9/5/22.
//

#include <Window/glfw/glfw_window.h>

namespace Wave
{
  
  std::unique_ptr<Window> Window::create(Context_api_e api_chosen, Window_properties_s options)
  {
    switch (api_chosen)
    {
      case Context_api_e::Glfw:return std::make_unique<Glfw_window>(options);
      case Context_api_e::Glut:
      {
        Wave::alert(WAVE_WARN, "[Window] --> Glut is not supported yet! Glfw used instead.");
        break;
      }
      case Context_api_e::Win32:
      {
        Wave::alert(WAVE_WARN, "[Window] --> Win32 is not supported yet! Glfw used instead.");
        break;
      }
      default:return std::make_unique<Glfw_window>(options);
    }
    return std::make_unique<Glfw_window>(options);
  }
  
  const std::function<void(Event &event)> &Window::get_event_callback_function()
  {
    if (glfwGetPlatform() != GLFW_NOT_INITIALIZED) return Glfw_window::get_event_callback_function();
    else
    {
      alert(WAVE_ERROR, "[Window] --> Window context api not initialized, cannot get event callback!");
      return Glfw_window::get_event_callback_function();
    }
    //TODO WIN32
  }
  
  void Window::set_event_callback_function(const std::function<void(Event &)> &callback_function)
  {
    if (glfwGetPlatform() != GLFW_NOT_INITIALIZED) Glfw_window::set_event_callback_function(callback_function);
    else
    {
      alert(WAVE_ERROR, "[Window] --> Window context api not initialized, cannot set event callback!");
    }
    //TODO WIN32
  }
}