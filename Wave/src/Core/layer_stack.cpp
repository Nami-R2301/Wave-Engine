//
// Created by nami on 27/03/23.
//


#include <Core/layer_stack.h>

namespace Wave
{
  
  Layer_stack::~Layer_stack()
  {
    for (Layer *layer: this->layers)
    {
      layer->on_detach();
      delete layer;
    }
  }
  
  void Layer_stack::push_layer(Layer *layer)
  {
    this->layers.emplace(this->layers.begin() + this->layer_insert_index, layer);
    this->layer_insert_index++;
  }
  
  void Layer_stack::pop_layer(Layer *layer)
  {
    auto it = std::find(this->layers.begin(), this->layers.begin() + this->layer_insert_index, layer);
    if (it != this->layers.begin() + this->layer_insert_index)
    {
      layer->on_detach();
      this->layers.erase(it);
      this->layer_insert_index--;
    }
  }
  
  void Layer_stack::push_overlay(Layer *overlay)
  {
    this->layers.emplace_back(overlay);
  }
  
  void Layer_stack::pop_overlay(Layer *overlay)
  {
    auto it = std::find(this->layers.begin() + this->layer_insert_index, this->layers.end(), overlay);
    if (it != this->layers.end())
    {
      overlay->on_detach();
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