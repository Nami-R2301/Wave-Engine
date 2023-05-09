//
// Created by nami on 27/03/23.
//

#include <Core/layer.h>

namespace Wave
{
  
  Layer::Layer(const std::string &name)
  {
    this->layer_name = name;
  }
  
  Layer::~Layer() = default;
  
  void Layer::on_attach()
  {
  }
  
  void Layer::on_detach()
  {
  }
  
  void Layer::on_update([[maybe_unused]] float time_step)
  {
  }
  
  void Layer::on_event([[maybe_unused]] Event &event)
  {
  }
  
  void Layer::on_ui_render(float time_step)
  {
  }
  
  void Layer::on_ui_render(float time_step)
  {
  
  }
}