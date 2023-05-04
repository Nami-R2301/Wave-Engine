//
// Created by nami on 23/04/23.
//

#pragma once

#include <Renderer/framebuffer.h>
#include <Renderer/vertex_array_buffer.h>

namespace Wave
{
  
  typedef struct Framebuffer_viewport_data
  {
    std::shared_ptr<Vertex_array_buffer> vao;
    uint32_t *ibo_data;
    float *vbo_data;
  } Framebuffer_viewport_data;
  
  class Gl_framebuffer : public Framebuffer
  {
  public:
    Gl_framebuffer() = default;
    explicit Gl_framebuffer(const Framebuffer_options &opt);
    ~Gl_framebuffer() override;
    
    [[nodiscard]] const Framebuffer_options &get_options() const override;
    [[nodiscard]] uint32_t get_color_attachment() const override;
    [[nodiscard]] uint32_t get_depth_attachment() const override;
    void resize(float width, float height, void *data) override;
    void on_resize_draw_data(void *data) override;
    void reset() override;
    void bind() override;
    void unbind() override;
    void remove() override;
  public:
    Framebuffer_viewport_data data;
  private:
    uint32_t renderer_id = 0;
    Framebuffer_options options{0};
    uint32_t color_attachment = 0;
    uint32_t depth_attachment = 0;
  };
}
