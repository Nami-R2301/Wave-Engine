//
// Created by nami on 11/8/22.
//

#include <Events/mouse_event.h>

namespace Wave
{
  
  std::string On_any_mouse_event::to_string() const
  {
    std::string output("[On mouse button input] :\n");
    
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer), "%50sEvent type --> %d\n",
                 DEFAULT, static_cast<int32_t>(this->event_type)) < 0)
    {
      output += "Snprintf error when trying to display [On_mouse_button_input] event!";
    }
    else { output += buffer; }
    
    return output;
  }
  
  /*************************** MOUSE PRESS ***************************/
  
  On_mouse_button_press::On_mouse_button_press(int32_t mouse_button_)
  {
    this->mouse_button_pressed = mouse_button_;
  }
  
  int32_t On_mouse_button_press::get_current_mouse_button() const
  {
    return this->mouse_button_pressed;
  }
  
  std::string On_mouse_button_press::to_string() const
  {
    std::string output("[On mouse button press] :\n");
    
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer), "%50sMouse button pressed --> %c\n",
                 DEFAULT, get_current_mouse_button()) < 0)
    {
      output += "Snprintf error when trying to display [On_mouse_button_press] event!";
    }
    else { output += buffer; }
    
    return output;
  }
  
  /***************************** MOUSE HOLD ****************************/
  
  On_mouse_button_hold::On_mouse_button_hold(int mouse_button)
  {
    this->mouse_button_held = mouse_button;
  }
  
  int On_mouse_button_hold::get_current_mouse_button() const
  {
    return this->mouse_button_held;
  }
  
  std::string On_mouse_button_hold::to_string() const
  {
    std::string output("[On mouse button hold] :\n");
    
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer), "%50sMouse button held --> %c\n",
                 DEFAULT, get_current_mouse_button()) < 0)
    {
      output += "Snprintf error when trying to display [On_mouse_button_hold] event!";
    }
    else { output += buffer; }
    
    return output;
  }
  
  /********************* MOUSE RELEASE EVENT **********************/
  
  On_mouse_button_release::On_mouse_button_release(int mouse_button_)
  {
    this->mouse_button_released = mouse_button_;
  }
  
  std::string On_mouse_button_release::to_string() const
  {
    std::string output("[On mouse button release] :\n");
    
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer), "%50sMouse button release --> %c\n",
                 DEFAULT, get_current_mouse_button()) < 0)
    {
      output += "Snprintf error when trying to display [On_mouse_button_release] event!";
    }
    else { output += buffer; }
    
    return output;
  }
  
  int On_mouse_button_release::get_current_mouse_button() const
  {
    return this->mouse_button_released;
  }
  
  /********************* MOUSE CURSOR EVENT ***********************/
  
  On_mouse_movement::On_mouse_movement(const Vector_2f &position)
  {
    this->mouse_cursor_position = position;
  }
  
  const Vector_2f &On_mouse_movement::get_mouse_cursor_position() const
  {
    return this->mouse_cursor_position;
  }
  
  On_mouse_wheel_scroll::On_mouse_wheel_scroll(const Vector_2f &mouse_scroll_offset_)
  {
    this->mouse_wheel_offset = mouse_scroll_offset_;
  }
  
  std::string On_mouse_movement::to_string() const
  {
    std::string output("[On mouse cursor movement] :\n");
    Vector_2f position = get_mouse_cursor_position();
    
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer),
                 "%50sCurrent mouse position --> (%.2f, %.2f)\n",
                 DEFAULT, position.get_x(), position.get_y()) < 0)
    {
      output += "Snprintf error when trying to display [On_mouse_cursor_movement] event!";
    }
    else { output += buffer; }
    
    return output;
  }
  
  /************************** MOUSE WHEEL *********************************/
  
  const Vector_2f &On_mouse_wheel_scroll::get_mouse_wheel_offset() const
  {
    return this->mouse_wheel_offset;
  }
  
  std::string On_mouse_wheel_scroll::to_string() const
  {
    std::string output("[On mouse wheel input] :\n");
    Vector_2f offset = get_mouse_wheel_offset();
    
    char buffer[FILENAME_MAX] {0};
    if (snprintf(buffer, sizeof(buffer),
                 "%50sMouse wheel offset --> (%.2f, %.2f)\n",
                 DEFAULT, offset.get_x(), offset.get_y()) < 0)
    {
      output += "Snprintf error when trying to display [On_mouse_wheel_input] event!";
    }
    else { output += buffer; }
    
    return output;
  }
}