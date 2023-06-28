//
// Created by nami on 11/8/22.
//

#pragma once

#include "Core/interfaces.h"
#include <Events/event.h>
#include <Input/key_codes.h>

namespace Wave::Event_system
{
  
  class On_any_key_event : public Event
  {
    public:
    On_any_key_event() = default;
    ~On_any_key_event() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_key_event);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT);
    
    void print([[maybe_unused]] Print_type type) const override
    {
    };
  };
  
  class On_key_press : public On_any_key_event
  {
    public:
    explicit On_key_press(int key_code_);
    ~On_key_press() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_key_press);
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] int get_current_key_code() const;
    private:
    int current_key_code = WAVE_KEY_UNKNOWN;
  };
  
  class On_key_hold : public On_any_key_event
  {
    public:
    explicit On_key_hold(int key_code_);
    ~On_key_hold() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_key_hold);
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] int get_current_key_code() const;
    private:
    int current_key_code = WAVE_KEY_UNKNOWN;
  };
  
  class On_key_release : public On_any_key_event
  {
    public:
    explicit On_key_release(int key_code_);
    ~On_key_release() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_key_release);
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] int get_current_key_code() const;
    private:
    int current_key_code = WAVE_KEY_UNKNOWN;
  };
}