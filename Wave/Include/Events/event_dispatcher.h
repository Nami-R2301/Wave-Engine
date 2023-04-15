//
// Created by nami on 11/6/22.
//

#pragma once

#include <Events/event.h>

namespace Wave
{
  
  class Event_dispatcher
  {
  public:
    inline explicit Event_dispatcher(Event &event_) : event(event_)
    {};
    ~Event_dispatcher() = default;
    
    template<class T, typename F>
    inline bool dispatch(const F &function)
    {
      if (!this->event.handled && T::get_static_type() == this->event.get_event_type())
      {
        this->event.handled |= function((T &) this->event);
        return true;
      }
      return false;
    }
  private:
    Event &event;
  };
}
