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
    
    if (this->state.severity[0] == 'W')
    { memcpy(color, YELLOW, sizeof(YELLOW)); }
    else if (this->state.severity[0] == 'F') memcpy(color, RED, sizeof(RED));
    
    char api_buffer[8]{0};
    
    switch (this->api)
    {
      case Renderer_api::Opengl:
      {
        snprintf(api_buffer, 7, "OpenGL");
        break;
      }
      case Renderer_api::Vulkan:
      {
        snprintf(api_buffer, 7, "Vulkan");
        break;
      }
      case Renderer_api::Directx:
      {
        snprintf(api_buffer, 8, "DirectX");
        break;
      }
      default:
      {
        snprintf(api_buffer, 5, "None");
        break;
      }
    }
    
    char buffer[FILENAME_MAX * 4]{0};
    if (snprintf(buffer, sizeof(buffer), "[On renderer error] :"
                                         "\n%50sApi --> %s%s"
                                         "\n%50sType --> %s%s"
                                         "\n%50sSeverity --> %s%s"
                                         "\n%50sDescription --> %s%s"
                                         "\n%50sCode --> %s0x%X",  // Print the error code in hex
                 DEFAULT, color, api_buffer, DEFAULT, color,
                 this->state.type, DEFAULT, color, this->state.severity,
                 DEFAULT, color, this->state.description, DEFAULT, color,
                 this->state.code) < 0)
    {
      free(color);
      return "ERROR : Snprintf error when trying to display [On_window_resize] event!";
    }
    
    free(color);
    return buffer;
  }
}