//
// Created by nami on 23/04/23.
//

#pragma once

#include <wave_pch.h>

namespace Wave
{
  
  typedef struct Framebuffer_options
  {
    float width = 0, height = 0;
    uint32_t samples = 1;
  } Framebuffer_options;
  
  class Framebuffer
  {
    public:
    virtual ~Framebuffer() = default;
    
    static std::shared_ptr<Framebuffer> create(const Framebuffer_options &opt);
    
    [[nodiscard]] virtual const Framebuffer_options &get_options() const = 0;
    [[nodiscard]] virtual uint32_t get_color_attachment() const = 0;
    [[nodiscard]] virtual uint32_t get_depth_attachment() const = 0;
    
    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual void resize(float width, float height, void *data) = 0;
    virtual void reset() = 0;
    virtual void remove() = 0;
    virtual void on_resize_draw_data(void *data) = 0;
  };
}
