//
// Created by nami on 23/04/23.
//

#pragma once

#include <Renderer/framebuffer.h>

namespace Wave
{
  
  class Gl_framebuffer : public Framebuffer
  {
  public:
    Gl_framebuffer() = default;
    explicit Gl_framebuffer(const Framebuffer_options &opt);
    ~Gl_framebuffer() override;
    
    [[nodiscard]] const Framebuffer_options &get_options() const override;
    [[nodiscard]] uint32_t get_color_attachment() const override;
    void resize(float width, float height) override;
    void reset() override;
    void bind() override;
    void unbind() override;
    void remove() override;
  private:
    uint32_t renderer_id = 0;
    Framebuffer_options options{0};
    uint32_t color_attachment = 0;
    uint32_t depth_attachment = 0;
  };
}
