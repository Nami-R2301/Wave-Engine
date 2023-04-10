//
// Created by nami on 8/30/22.
//

#pragma once

#include <wave_pch.h>
#include <Core/core_macros.h>

namespace Wave
{
  
  enum Event_category
  {
    None = 0,
    EVENT_CATEGORY_APPLICATION = BIT(0),
    EVENT_CATEGORY_RENDERER = BIT(1),
    EVENT_CATEGORY_INPUT = BIT(2),
    EVENT_CATEGORY_KEYBOARD = BIT(3),
    EVENT_CATEGORY_MOUSE = BIT(4),
    EVENT_CATEGORY_MOUSE_BUTTON = BIT(5)
  };
  
  enum Event_state
  {
    NOT_HANDLED = 0,
    HANDLED
  };
  
  enum class Event_type
  {
    None = 0,
    On_window_close,
    On_window_resize,
    On_window_focus,
    On_window_lost_focus,
    On_window_movement,
    On_app_tick,
    On_app_update,
    On_app_render,
    On_renderer_error,
    On_key_event,
    On_key_press,
    On_key_release,
    On_key_hold,
    On_key_type,
    On_mouse_event,
    On_mouse_button_press,
    On_mouse_button_hold,
    On_mouse_button_release,
    On_mouse_wheel_scroll,
    On_mouse_movement
  };
  
  #define EVENT_CLASS_TYPE(type) static Event_type get_static_type() { return type; }\
                 Event_type get_event_type() const override { return get_static_type(); }
  
  #define EVENT_CLASS_CATEGORY(category) virtual int get_category_flags() const override { return category; }
  
  class Event : public Printable
  {
    friend class Event_dispatcher;
  
  public:
    ~Event() override = default;
    [[nodiscard]] virtual inline Event_type get_event_type() const
    {
      return this->event_type;
    };
    
    [[nodiscard]] virtual inline int get_category_flags() const = 0;
    
    template<class T>
    static inline Event_state get_event_state()
    {
      return T::event_state;
    }
    
    template<class T>
    static inline void set_event_state(Event_state state)
    {
      T::event_state = state;
    }
    
    /* Printable */
    [[nodiscard]] std::string to_string() const override
    {
      return {"[Generic event]\n"};
    };
    Event_state event_state = NOT_HANDLED;
  protected:
    Event_type event_type = Event_type::None;
  };
}
