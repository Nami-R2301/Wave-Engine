//
// Created by nami on 11/8/22.
//

#pragma once

#include <Window/window.h>
#include <Math/camera.h>
#include <Input/key_codes.h>
#include <Events/event_dispatcher.h>

namespace Wave
{
  
  class On_any_key_callback : public Event
  {
  public:
    On_any_key_callback();
    ~On_any_key_callback() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_key_event);
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT);
    
    INTERFACE_PRINT
  };
  
  class On_key_press : public On_any_key_callback
  {
  public:
    explicit On_key_press(int key_code_);
    ~On_key_press() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_key_press);
    
    INTERFACE_PRINT
    
    [[nodiscard]] int get_current_key_code() const;
  private:
    int current_key_code = WAVE_KEY_UNKNOWN;
  };
  
  class On_key_hold : public On_any_key_callback
  {
  public:
    explicit On_key_hold(int key_code_);
    ~On_key_hold() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_key_hold);
    
    INTERFACE_PRINT
    
    [[nodiscard]] int get_current_key_code() const;
  private:
    int current_key_code = WAVE_KEY_UNKNOWN;
  };
  
  class On_key_release : public On_any_key_callback
  {
  public:
    explicit On_key_release(int key_code_);
    ~On_key_release() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_key_release);
    
    INTERFACE_PRINT
    
    [[nodiscard]] int get_current_key_code() const;
  private:
    int current_key_code = WAVE_KEY_UNKNOWN;
  };
}