//
// Created by nami on 11/8/22.
//

#include <Events/app_event.h>

namespace Wave
{
  
  std::string On_window_resize::to_string() const
  {
    uint32_t width_ = get_width();
    uint32_t height_ = get_height();
    
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer),
                 "[On window resize] :\n%50sWindow resized to --> (%d, %d)", DEFAULT, width_, height_) < 0)
    {
      return "ERROR : Snprintf error when trying to display [On_window_resize] event!";
    }
    return buffer;
  }
  
  uint32_t On_window_resize::get_height() const
  {
    return this->height;
  }
  
  uint32_t On_window_resize::get_width() const
  {
    return this->width;
  }
  
  std::string On_window_close::to_string() const
  {
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer),
                 "[On window close] :\n%50sUser request to close app window", DEFAULT) < 0)
    {
      return "Snprintf error when trying to display [On_window_resize] event!";
    }
    return buffer;
  }
}