//
// Created by nami on 27/03/23.
//

#pragma once

#include <Layer/layer.h>

namespace Wave
{
  class Layer_stack
  {
    public:
    Layer_stack() = default;
    ~Layer_stack();
    
    void push_layer(Layer *layer);
    void pop_layer(Layer *layer);
    
    void push_overlay(Layer *overlay);
    void pop_overlay(Layer *overlay);
    
    std::vector<Layer *>::iterator begin();
    std::vector<Layer *>::iterator end();
    
    std::vector<Layer *>::reverse_iterator rbegin()
    { return this->layers.rbegin(); }
    
    std::vector<Layer *>::reverse_iterator rend()
    { return this->layers.rend(); }
    
    [[nodiscard]] std::vector<Layer *>::const_iterator begin() const
    { return this->layers.begin(); }
    
    [[nodiscard]] std::vector<Layer *>::const_iterator end() const
    { return this->layers.end(); }
    
    [[nodiscard]] std::vector<Layer *>::const_reverse_iterator rbegin() const
    { return this->layers.rbegin(); }
    
    [[nodiscard]] std::vector<Layer *>::const_reverse_iterator rend() const
    { return this->layers.rend(); }
    
    private:
    std::vector<Layer *> layers;
    unsigned int layer_insert_index = 0;
  };
}
