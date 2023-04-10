//
// Created by nami on 11/8/22.
//

#pragma once

#include <Window/window.h>
#include <Events/event_dispatcher.h>

namespace Wave
{
  
  class On_engine_tick : public Event
  {
  public:
    On_engine_tick() = default;
    
    EVENT_CLASS_TYPE(Event_type::On_app_tick)
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINT;
  };
  
  class On_engine_update : public Event
  {
  public:
    On_engine_update() = default;
    
    EVENT_CLASS_TYPE(Event_type::On_app_update)
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINT;
  };
  
  class On_engine_render : public Event
  {
  public:
    On_engine_render() = default;
    
    EVENT_CLASS_TYPE(Event_type::On_app_render)
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINT;
  };
  
  class On_window_close : public Event
  {
  public:
    On_window_close() = default;
    ~On_window_close() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_window_close)
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINT;
  };
  
  class On_window_resize : public Event
  {
  public:
    On_window_resize(uint32_t width, uint32_t height) : width(width), height(height)
    {};
    ~On_window_resize() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_window_resize);
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);
    
    INTERFACE_PRINT
    
    [[nodiscard]] uint32_t get_width() const;
    [[nodiscard]] uint32_t get_height() const;
  
  private:
    uint32_t width = 0;
    uint32_t height = 0;
  };

//
//  class  On_window_focus : public Event
//  {
//  public:
//    On_window_focus() = default;
//    explicit On_window_focus(std::function<void(Window *window)>);
//    ~On_window_focus() = default;
//  };
}
