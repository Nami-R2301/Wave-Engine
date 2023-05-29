//
// Created by nami on 27/03/23.
//

#pragma once

#include <wave_pch.h>
#include <Events/event.h>

namespace Wave
{
  class Layer
  {
    public:
    explicit Layer(const std::string &name = "Layer");
    virtual ~Layer();
    
    virtual void on_attach();
    virtual void on_detach();
    virtual void on_update(float time_step);
    virtual void on_event(Event &event);
    virtual void on_render();
    
    [[nodiscard]] inline const std::string &get_name() const
    { return this->layer_name; };
    protected:
    std::string layer_name;
    private:
    bool activated = true;
  };
}