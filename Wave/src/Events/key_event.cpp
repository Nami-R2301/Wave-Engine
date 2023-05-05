//
// Created by nami on 11/8/22.
//

#include <Events/key_event.h>

namespace Wave
{
  
  /************************** KEY PRESS EVENT ***************************/
  
  On_key_press::On_key_press(int32_t key_code_)
  {
    this->current_key_code = key_code_;
  }
  
  std::string On_key_press::to_string() const
  {
    char buffer[FILENAME_MAX]{0};
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
    char buffer[FILENAME_MAX]{0};
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
  }
  
  std::string On_key_release::to_string() const
  {
    char buffer[FILENAME_MAX]{0};
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
