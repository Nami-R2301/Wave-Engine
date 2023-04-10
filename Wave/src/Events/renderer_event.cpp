//
// Created by nami on 30/03/23.
//


#include <Events/renderer_event.h>

namespace Wave
{
  
  On_renderer_error::On_renderer_error(Renderer_state state_, Renderer_api api_)
  {
    this->state = state_;
    this->api = api_;
  }
  
  Renderer_state On_renderer_error::get_renderer_state() const
  {
    return this->state;
  }
  
  Renderer_api On_renderer_error::get_renderer_api() const
  {
    return this->api;
  }
  
  std::string On_renderer_error::to_string() const
  {
    char *color = (char *) calloc(10, sizeof(char));
    
    if (strcmp(this->state.severity, "Warning") == 0) { memcpy(color, YELLOW, sizeof(YELLOW)); }
    else if (strcmp(this->state.severity, "Fatal") == 0) memcpy(color, RED, sizeof(RED));
    
    char buffer[FILENAME_MAX * 4] {0};
    if (snprintf(buffer, sizeof(buffer), "[On renderer error] :\n%50sType --> %s%s\n%50sSeverity --> %s%s\n"
                                         "%50sDescription --> %s%s\n%50sCode --> %s%d\n%50sApi --> %s", DEFAULT,
                 color, this->state.type, DEFAULT, color, this->state.severity, DEFAULT, color, this->state.description,
                 DEFAULT, color, static_cast<int32_t>(this->state.code), DEFAULT, color) < 0)
    {
      free(color);
      return "ERROR : Snprintf error when trying to display [On_window_resize] event!";
    }
    switch (this->api)
    {
      case Renderer_api::Opengl:
      {
        strncat(buffer, "OpenGL", 7);
        break;
      }
      case Renderer_api::Vulkan:
      {
        strncat(buffer, "Vulkan", 7);
        break;
      }
      case Renderer_api::Directx:
      {
        strncat(buffer, "DirectX", 8);
        break;
      }
      default:strncat(buffer, "None", 5);
        break;
    }
    
    free(color);
    return buffer;
  }
}