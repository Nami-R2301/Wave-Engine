//
// Created by nami on 11/8/22.
//

#pragma once

#include <Window/window.h>
#include <Events/event_dispatcher.h>

namespace Wave::Event_system
{
  
  class On_app_tick : public Event
  {
    public:
    On_app_tick() = default;
    ~On_app_tick() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_app_tick)
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINTABLE
  };
  
  class On_app_update : public Event
  {
    public:
    On_app_update() = default;
    ~On_app_update() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_app_update)
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINTABLE
  };
  
  class On_app_render : public Event
  {
    public:
    On_app_render() = default;
    ~On_app_render() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_app_render)
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINTABLE
  };
  
  class On_app_framerate_change : public Event
  {
    public:
    explicit On_app_framerate_change(float new_framerate);
    ~On_app_framerate_change() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_app_framerate_change);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] float get_framerate() const;
    private:
    float framerate = 0.0f;
  };
  
  class On_window_close : public Event
  {
    public:
    On_window_close() = default;
    ~On_window_close() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_window_close)
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_CONTEXT)
    
    INTERFACE_PRINTABLE
  };
  
  class On_window_resize : public Event
  {
    public:
    On_window_resize(float width, float height) : width(width), height(height)
    {};
    ~On_window_resize() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_window_resize);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_CONTEXT);
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] float get_width() const;
    [[nodiscard]] float get_height() const;
    
    private:
    float width = 0.0f;
    float height = 0.0f;
  };
  
  class On_framebuffer_resize : public Event
  {
    public:
    On_framebuffer_resize(float position_x_, float position_y_, float width, float height) :
      position_x(position_x_), position_y(position_y_), width(width), height(height)
    {}
    
    ~On_framebuffer_resize() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_framebuffer_resize);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] float get_width() const
    {
      return this->width;
    }
    
    [[nodiscard]] float get_height() const
    {
      return this->height;
    }
    
    [[nodiscard]] float get_position_x() const
    {
      return this->position_x;
    }
    
    [[nodiscard]] float get_position_y() const
    {
      return this->position_y;
    }
    
    private:
    float position_x = 0.0f;
    float position_y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
  };
  
  typedef struct Context_state_s
  {
    const char *type;
    const char *severity;
    const char *description;
    int32_t error_code;
  } Context_state_s;
  
  class On_context_error : public Event
  {
    public:
    On_context_error(Context_state_s state, Context_api_e api);
    ~On_context_error() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_window_error);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_CONTEXT);
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] Context_state_s get_context_state() const;
    [[nodiscard]] Context_api_e get_context_api() const;
    private:
    Context_state_s state{nullptr,
                          nullptr,
                          nullptr,
                          0};
    Context_api_e api;
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
