//
// Created by nami on 27/03/23.
//


#include <Core/layer_stack.h>

namespace Wave
{
  
  Layer_stack::Layer_stack()
  {
    this->layer_insert = this->layers.begin();
  }
  
  Layer_stack::~Layer_stack()
  {
    for (Layer *layer: this->layers) delete layer;
  }
  
  void Layer_stack::push_layer(Layer *layer)
  {
    this->layer_insert = this->layers.emplace(this->layer_insert, layer);
  }
  
  void Layer_stack::pop_layer(Layer *layer)
  {
    auto it = std::find(this->layers.begin(), this->layers.end(), layer);
    if (it != this->layers.end())
    {
      this->layer_insert = this->layers.erase(it);;
    }
  }
  
  void Layer_stack::push_overlay(Layer *overlay)
  {
    this->layers.emplace_back(overlay);
  }
  
  void Layer_stack::pop_overlay(Layer *overlay)
  {
    auto it = std::find(this->layers.begin(), this->layers.end(), overlay);
    if (it != this->layers.end())
    {
      this->layers.erase(it);
    }
  }
  
  std::vector<Layer *>::iterator Layer_stack::begin()
  {
    return this->layers.begin();
  }
  
  std::vector<Layer *>::iterator Layer_stack::end()
  {
    return this->layers.end();
  }
}