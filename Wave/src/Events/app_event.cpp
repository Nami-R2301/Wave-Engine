//
// Created by nami on 11/8/22.
//

#include "Window/window.h"
#include <Events/app_event.h>

namespace Wave
{
  
  std::string On_window_resize::to_string() const
  {
    
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer),
                 "[On window resize] : Window resized to --> (%.2f, %.2f)", this->get_width(), this->get_height()) < 0)
    {
      return "ERROR : Snprintf error when trying to display [On_window_resize] event!";
    }
    return buffer;
  }
  
  float On_window_resize::get_height() const
  {
    return this->height;
  }
  
  float On_window_resize::get_width() const
  {
    return this->width;
  }
  
  std::string On_window_close::to_string() const
  {
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer), "[On window close] : User request to close app window") < 0)
    {
      return "Snprintf error when trying to display [On_window_resize] event!";
    }
    return buffer;
  }
  
  On_context_error::On_context_error(Context_state state, Context_api api)
  {
    this->state = state;
    this->api = api;
  }
  
  std::string On_context_error::to_string() const
  {
    char *color = (char *) calloc(10, sizeof(char));
    
    if (strcmp(this->state.severity, "Warning") == 0) { memcpy(color, YELLOW, sizeof(YELLOW)); }
    else if (strcmp(this->state.severity, "Fatal") == 0) memcpy(color, RED, sizeof(RED));
    
    char buffer[FILENAME_MAX * 4] {0};
    if (snprintf(buffer, sizeof(buffer), "[On context error] :\n%50sType --> %s%s\n%50sSeverity --> %s%s\n"
                                         "%50sDescription --> %s%s\n%50sCode --> %s%d\n%50sApi --> %s", DEFAULT,
                 color, this->state.type, DEFAULT, color, this->state.severity, DEFAULT, color, this->state.description,
                 DEFAULT, color, static_cast<int32_t>(this->state.error_code), DEFAULT, color) < 0)
    {
      free(color);
      return "ERROR : Snprintf error when trying to display [On_window_resize] event!";
    }
    switch (this->api)
    {
      case Context_api::GLFW:
      {
        strncat(buffer, "GLFW", 5);
        break;
      }
      case Context_api::GLUT:
      {
        strncat(buffer, "GLUT", 5);
        break;
      }
      case Context_api::WIN32:
      {
        strncat(buffer, "WIN32", 6);
        break;
      }
      default:strncat(buffer, "None", 5);
        break;
    }
    
    free(color);
    return buffer;
  }
  
  Context_state On_context_error::get_context_state() const
  {
    return this->state;
  }
  
  Context_api On_context_error::get_context_api() const
  {
    return this->api;
  }
}