//
// Created by nami on 11/05/23.
//

#pragma once

#include <Renderer/uniform_buffer.h>

namespace Wave
{
  
  class Gl_uniform_buffer : public Uniform_buffer
  {
    public:
    Gl_uniform_buffer(uint32_t size, uint32_t binding);
    ~Gl_uniform_buffer() override;
    
    void set_data(uint32_t offset, uint32_t size, const void *data) override;
    private:
    uint32_t renderer_id = 0;
  };
} // Wave
