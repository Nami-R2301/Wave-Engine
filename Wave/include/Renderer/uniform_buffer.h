//
// Created by nami on 11/05/23.
//

#pragma once

#include <Core/core.h>

namespace Wave
{
  
  class Uniform_buffer
  {
    public:
    static std::shared_ptr<Uniform_buffer> create(uint32_t size, uint32_t binding);
    virtual ~Uniform_buffer() = default;
    
    virtual void set_data(uint32_t offset, uint32_t size, const void *data) = 0;
  };
}  // Wave
