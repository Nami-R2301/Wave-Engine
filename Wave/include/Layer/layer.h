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
    virtual ~Layer() = default;
    
    virtual void on_attach() = 0;
    virtual void on_detach() = 0;
    virtual void on_update(float time_step) = 0;
    virtual void on_event(Event &event) = 0;
    virtual void on_render() = 0;
    virtual void on_ui_render(float time_step) = 0;
    
    [[nodiscard]] inline const std::string &get_name() const
    { return this->layer_name; };
    protected:
    std::string layer_name;
    private:
    bool activated = true;
  };
}