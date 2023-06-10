//
// Created by nami on 23/04/23.
//

#pragma once

#include <Renderer/framebuffer.h>
#include <Renderer/vertex_array_buffer.h>

namespace Wave
{
  
  typedef struct Gl_framebuffer_viewport_data
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
    
    INTERFACE_SENDABLE
    INTERFACE_PRINTABLE
    
    [[nodiscard]] const Framebuffer_options &get_options() const override;
    [[nodiscard]] Texture *get_color_attachment() override;
    [[nodiscard]] Texture *get_depth_attachment() override;
    
    void resize(float width, float height, void *data) override;
    void on_resize_draw_data(void *data) override;
    void reset() override;
    void bind() override;
    void unbind() const override;
    public:
    Framebuffer_viewport_data data;
    private:
    uint32_t renderer_id = 0;
    Framebuffer_options options{0};
    Texture *color_attachment = nullptr;
    Texture *depth_attachment = nullptr;
  };
}
