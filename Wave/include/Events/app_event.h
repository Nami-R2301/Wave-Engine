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
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_CONTEXT)
    
    INTERFACE_PRINT;
  };
  
  class On_window_resize : public Event
  {
    public:
    On_window_resize(float width, float height) : width(width), height(height)
    {};
    ~On_window_resize() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_window_resize);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_CONTEXT);
    
    INTERFACE_PRINT
    
    [[nodiscard]] float get_width() const;
    [[nodiscard]] float get_height() const;
    
    private:
    float width = 0.0f;
    float height = 0.0f;
  };
  
  class On_viewport_resize : public Event
  {
  };
  
  typedef struct Context_state
  {
    const char *type;
    const char *severity;
    const char *description;
    int32_t error_code;
  } Context_state;
  
  class On_context_error : public Event
  {
    public:
    On_context_error(Context_state state, Context_api api);
    ~On_context_error() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_window_error);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_CONTEXT);
    
    INTERFACE_PRINT
    
    [[nodiscard]] Context_state get_context_state() const;
    [[nodiscard]] Context_api get_context_api() const;
    private:
    Context_state state{nullptr,
                        nullptr,
                        nullptr,
                        0};
    Context_api api;
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
