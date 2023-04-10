//
// Created by nami on 27/03/23.
//

#pragma once

#include <Core/layer.h>

namespace Wave
{
  class Layer_stack
  {
  public:
    Layer_stack();
    ~Layer_stack();
    
    void push_layer(Layer *layer);
    void pop_layer(Layer *layer);
    
    void push_overlay(Layer *overlay);
    void pop_overlay(Layer *overlay);
    
    std::vector<Layer *>::iterator begin();
    std::vector<Layer *>::iterator end();
  private:
    std::vector<Layer *> layers;
    std::vector<Layer *>::iterator layer_insert;
  };
}
