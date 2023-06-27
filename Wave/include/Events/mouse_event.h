//
// Created by nami on 11/8/22.
//

#pragma once

#include <Events/event.h>
#include <Input/mouse_codes.h>

namespace Wave
{
  
  class On_mouse_button_press : public Event
  {
    public:
    explicit On_mouse_button_press(int mouse_button_);
    ~On_mouse_button_press() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_mouse_button_press);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_MOUSE_BUTTON | EVENT_CATEGORY_INPUT)
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] int get_current_mouse_button() const;
    private:
    int mouse_button_pressed = WAVE_MOUSE_BUTTON_UNKNOWN;
  };
  
  class On_mouse_button_hold : public Event
  {
    public:
    explicit On_mouse_button_hold(int mouse_button_);
    ~On_mouse_button_hold() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_mouse_button_hold);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_MOUSE_BUTTON | EVENT_CATEGORY_INPUT)
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] int get_current_mouse_button() const;
    private:
    int mouse_button_held = WAVE_MOUSE_BUTTON_UNKNOWN;
  };
  
  class On_mouse_button_release : public Event
  {
    public:
    explicit On_mouse_button_release(int mouse_button_);
    ~On_mouse_button_release() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_mouse_button_release);
    
    EVENT_CLASS_CATEGORY(None)
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] int get_current_mouse_button() const;
    private:
    int mouse_button_released = WAVE_MOUSE_BUTTON_UNKNOWN;
  };
  
  class On_mouse_movement : public Event
  {
    public:
    explicit On_mouse_movement(const Math::Vector_2f &position);
    ~On_mouse_movement() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_mouse_movement);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)
    
    INTERFACE_PRINTABLE
    
    // MOUSE MOVEMENT INSTANCE FUNCTIONS.
    [[nodiscard]] const Math::Vector_2f &get_mouse_cursor_position() const;
    private:
    Math::Vector_2f mouse_cursor_position = Math::Vector_2f(0);
  };
  
  class On_mouse_wheel_scroll : public Event
  {
    public:
    explicit On_mouse_wheel_scroll(const Math::Vector_2f &mouse_scroll_offset_);
    ~On_mouse_wheel_scroll() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_mouse_wheel_scroll);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);
    
    INTERFACE_PRINTABLE
    
    // MOUSE MOVEMENT INSTANCE FUNCTIONS.
    [[nodiscard]] const Math::Vector_2f &get_mouse_wheel_offset() const;
    private:
    Math::Vector_2f mouse_wheel_offset = Math::Vector_2f(0);
  };
}
