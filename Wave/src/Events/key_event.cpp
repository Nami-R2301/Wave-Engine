//
// Created by nami on 11/8/22.
//

#include <Events/key_event.h>

namespace Wave
{
  
  /************************ ON ANY KEY EVENT ****************************/
  
  On_any_key_callback::On_any_key_callback() = default;
  
  std::string On_any_key_callback::to_string() const
  {
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer), "[On key input] :\n%50sEvent type --> %d\n",
                 DEFAULT, static_cast<int32_t>(this->event_type)) < 0)
    {
      return "Snprintf error when trying to display [On_key_input] event!";
    }
    return buffer;
  }
  
  /************************** KEY PRESS EVENT ***************************/
  
  On_key_press::On_key_press(int32_t key_code_)
  {
    this->current_key_code = key_code_;
  }
  
  std::string On_key_press::to_string() const
  {
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer), "[On key press] :\n%50sKey pressed --> %c\n",
                 DEFAULT, get_current_key_code()) < 0)
    {
      return "Snprintf error when trying to display [On_key_press] event!";
    }
    return buffer;
  }
  
  int On_key_press::get_current_key_code() const
  {
    return this->current_key_code;
  }
  
  /************************** KEY HOLD EVENT ***************************/
  
  On_key_hold::On_key_hold(int key_code_)
  {
    this->current_key_code = key_code_;
  }
  
  std::string On_key_hold::to_string() const
  {
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer),
                 "[On key hold] :\n%50sKey held --> %c\n", DEFAULT, get_current_key_code()) < 0)
    {
      return "Snprintf error when trying to display [On_key_hold] event!";
    }
    return buffer;
  }
  
  int On_key_hold::get_current_key_code() const
  {
    return this->current_key_code;
  }
  
  /************************** KEY RELEASE **************************/
  
  On_key_release::On_key_release(int key_code_)
  {
    this->current_key_code = key_code_;
    On_key_release::event_state = NOT_HANDLED;
  }
  
  std::string On_key_release::to_string() const
  {
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer), "[On key release] :\n%50sKey released --> %c\n",
                 DEFAULT, get_current_key_code()) < 0)
    {
      return "Snprintf error when trying to display [On_key_release] event!";
    }
    return buffer;
  }
  
  int On_key_release::get_current_key_code() const
  {
    return this->current_key_code;
  }
}
