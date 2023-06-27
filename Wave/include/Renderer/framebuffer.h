//
// Created by nami on 23/04/23.
//

#pragma once

#include <Renderer/texture.h>

namespace Wave
{
  
  enum class Framebuffer_target_e
  {
    Color_attachment = 0,
    Depth_attachment,
    Depth_stencil_attachment,
    Stencil_attachment,
    ID_attachment
  };
  
  typedef struct Framebuffer_attachment_s
  {
    Framebuffer_target_e buffer_target = Framebuffer_target_e::Color_attachment;
    int32_t attachment_index = 0, attachment_texture_slot = WAVE_VALUE_DONT_CARE;
    Texture *attachment_texture = nullptr;
  } Framebuffer_attachment_s;
  
  typedef struct Framebuffer_options_s
  {
    int width = 0, height = 0;
    std::vector<Framebuffer_attachment_s> attachments;
    int32_t samples = 1;
  } Framebuffer_options_s;
  
  class Framebuffer : public I_printable, public I_sendable
  {
    public:
    ~Framebuffer() override = default;
    
    static std::shared_ptr<Framebuffer> create(const Framebuffer_options_s &target);
    
    [[nodiscard]] virtual uint32_t get_id() const = 0;
    [[nodiscard]] virtual const Framebuffer_options_s &get_options() const = 0;
    [[nodiscard]] virtual const std::vector<Framebuffer_attachment_s> &get_color_attachments() = 0;
    [[nodiscard]] virtual const Framebuffer_attachment_s &get_depth_attachment() = 0;
    
    virtual void add_attachment(const Framebuffer_attachment_s &attachment) = 0;
    virtual void blit_color_attachments(int32_t framebuffer_id,
                                        const std::vector<Framebuffer_attachment_s> &color_attachments_) = 0;
    virtual void clear_attachment(uint32_t attachment_index, int32_t clear_value) = 0;
    virtual int32_t read_pixel(uint32_t attachment_index, int32_t position_x, int32_t position_y) = 0;
    
    virtual void bind() = 0;
    virtual void unbind() const = 0;
    virtual void resize(int32_t width, int32_t height, void *data) = 0;
    virtual void reset() = 0;
    virtual void on_resize_draw_data(void *data) = 0;
    protected:
    bool sent = false;
  };
}
