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
    ushort *ibo_data;
    float *vbo_data;
  } Framebuffer_viewport_data;
  
  class Gl_framebuffer : public Framebuffer
  {
    public:
    Gl_framebuffer() = default;
    explicit Gl_framebuffer(const Framebuffer_options_s &opt);
    ~Gl_framebuffer() override;
    
    INTERFACE_SENDABLE;
    INTERFACE_PRINTABLE;
    
    [[nodiscard]] uint32_t get_id() const override;
    [[nodiscard]] const Framebuffer_options_s &get_options() const override;
    [[nodiscard]] const std::vector<Framebuffer_attachment_s> &get_color_attachments() override;
    [[nodiscard]] const Framebuffer_attachment_s &get_depth_attachment() override;
    
    void add_attachment(const Framebuffer_attachment_s &attachment) override;
    void blit_color_attachments(int32_t framebuffer_id,
                                const std::vector<Framebuffer_attachment_s> &color_attachments_) override;
    void clear_attachment(uint32_t attachment_index, int32_t clear_value) override;
    int32_t read_pixel(uint32_t attachment_index, int32_t position_x, int32_t position_y) override;
    
    void resize(int32_t width, int32_t height, void *data) override;
    void on_resize_draw_data(void *data) override;
    void reset() override;
    void bind() override;
    void unbind() const override;
    public:
    Framebuffer_viewport_data data;
    private:
    uint32_t renderer_id = 0;
    Framebuffer_options_s options;
    std::vector<Framebuffer_attachment_s> color_attachments;
    Framebuffer_attachment_s depth_attachment;
  };
}
