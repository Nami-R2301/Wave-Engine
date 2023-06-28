//
// Created by nami on 8/30/22.
//

#pragma once

#include <wave_pch.h>
#include <wave_pch.h>

namespace Wave::Event_system
{
  
  enum Event_category
  {
    None = 0,
    EVENT_CATEGORY_APPLICATION = BIT(0),
    EVENT_CATEGORY_CONTEXT = BIT(1),
    EVENT_CATEGORY_RENDERER = BIT(2),
    EVENT_CATEGORY_INPUT = BIT(3),
    EVENT_CATEGORY_KEYBOARD = BIT(4),
    EVENT_CATEGORY_MOUSE = BIT(5),
    EVENT_CATEGORY_MOUSE_BUTTON = BIT(6)
  };
  
  enum class Event_type
  {
    None = 0,
    On_window_close,
    On_window_resize,
    On_window_focus,
    On_window_lost_focus,
    On_window_movement,
    On_framebuffer_resize,
    On_framebuffer_focus,
    On_framebuffer_lost_focus,
    On_app_tick,
    On_app_framerate_change,
    On_app_update,
    On_app_render,
    On_scene_load,
    On_entity_add,
    On_entity_delete,
    On_entity_duplicate,
    On_window_error,
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

#define EVENT_CLASS_CATEGORY(category) virtual int get_category_flags() const override { return (int) (category); }
  
  class Event : public I_printable
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
    static inline bool get_event_state()
    {
      return T::event_state;
    }
    
    [[nodiscard]] bool is_in_category(Event_category category) const
    {
      return get_category_flags() & category;
    }
    
    template<class T>
    static inline void set_event_state(bool state)
    {
      T::event_state = state;
    }
    
    /* Printable */
    [[nodiscard]] std::string to_string() const override
    {
      return {"[Generic event]\n"};
    };
    bool handled = false;
    protected:
    Event_type event_type = Event_type::None;
  };
}
