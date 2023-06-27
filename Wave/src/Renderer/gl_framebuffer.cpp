//
// Created by nami on 23/04/23.
//

#include <Renderer/gl_renderer.h>
#include <cassert>
#include <Renderer/gl_framebuffer.h>


namespace Wave
{
  
  static const uint32_t index_buffer_data[6] = {2,
                                                1,
                                                0,
                                                2,
                                                0,
                                                3};
  
  static int32_t convert_to_gl_attachment(Framebuffer_target_e target, uint32_t attachment_index_)
  {
    switch (target)
    {
      case Framebuffer_target_e::ID_attachment:
      case Framebuffer_target_e::Color_attachment: return GL_COLOR_ATTACHMENT0 + attachment_index_;
      case Framebuffer_target_e::Depth_attachment: return GL_DEPTH_ATTACHMENT;
      case Framebuffer_target_e::Stencil_attachment: return GL_STENCIL_ATTACHMENT;
      case Framebuffer_target_e::Depth_stencil_attachment: return GL_DEPTH_STENCIL_ATTACHMENT;
    }
    return GL_COLOR_ATTACHMENT0 + attachment_index_;
  }
  
  static Texture::Texture_internal_format_e attachment_to_texture_internal_format(Framebuffer_target_e target)
  {
    switch (target)
    {
      case Framebuffer_target_e::ID_attachment: return Texture::Texture_internal_format_e::ID_attachment;
      case Framebuffer_target_e::Color_attachment: return Texture::Texture_internal_format_e::Color_attachment;
      case Framebuffer_target_e::Depth_attachment: return Texture::Texture_internal_format_e::Depth_attachment;
      case Framebuffer_target_e::Stencil_attachment: return Texture::Texture_internal_format_e::Stencil_attachment;
      case Framebuffer_target_e::Depth_stencil_attachment: return Texture::Texture_internal_format_e::Depth_stencil_attachment;
    }
    return Texture::Texture_internal_format_e::Color_attachment;
  }
  
  static std::string buffer_type_to_string(Framebuffer_target_e target)
  {
    switch (target)
    {
      case Framebuffer_target_e::ID_attachment: return "ID attachment";
      case Framebuffer_target_e::Color_attachment: return "Color attachment";
      case Framebuffer_target_e::Depth_attachment: return "Depth attachment";
      case Framebuffer_target_e::Stencil_attachment: return "Stencil attachment";
      case Framebuffer_target_e::Depth_stencil_attachment: return "Depth stencil attachment";
    }
    return "Undefined target attachment!";
  }
  
  Gl_framebuffer::Gl_framebuffer(const Framebuffer_options_s &options_)
  {
    this->options = options_;
    
    for (const auto &attachment: options_.attachments)
    {
      Texture::Texture_internal_format_e texture_internal_format = attachment_to_texture_internal_format(
        attachment.buffer_target);
      
      // Creating the 2D texture of our viewport.
      Framebuffer_attachment_s new_attachment = {attachment.buffer_target, attachment.attachment_index,
                                                 attachment.attachment_texture_slot};
      
      new_attachment.attachment_texture = new Gl_texture_2D(nullptr, {Texture::Texture_type_e::Texture_2D,
                                                                      texture_internal_format,
                                                                      this->options.width,
                                                                      this->options.height,
                                                                      0,
                                                                      attachment.attachment_texture_slot,
                                                                      this->options.samples,
                                                                      nullptr});
      
      add_attachment(new_attachment);
    }
  }
  
  Gl_framebuffer::~Gl_framebuffer()
  {
    Gl_framebuffer::free_gpu(1);
    for (const auto &attachment: this->color_attachments) delete attachment.attachment_texture;
    this->color_attachments.clear();
    delete this->depth_attachment.attachment_texture;
    
    delete[] this->data.ibo_data;
    delete[] this->data.vbo_data;
  }
  
  void Gl_framebuffer::add_attachment(const Framebuffer_attachment_s &attachment)
  {
    attachment.buffer_target == Framebuffer_target_e::Depth_attachment ||
    attachment.buffer_target == Framebuffer_target_e::Depth_stencil_attachment ? this->depth_attachment = attachment :
    this->color_attachments.emplace_back(attachment);
  }
  
  void Gl_framebuffer::blit_color_attachments(int32_t framebuffer_id,
                                              const std::vector<Framebuffer_attachment_s> &color_attachments_)
  {
    CHECK_GL_CALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, this->renderer_id));
    CHECK_GL_CALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_id));
    
    for (const auto &color_attachment: color_attachments_)
    {
      CHECK_GL_CALL(glReadBuffer(GL_COLOR_ATTACHMENT0 + color_attachment.attachment_index));
      CHECK_GL_CALL(glDrawBuffer(GL_COLOR_ATTACHMENT0 + color_attachment.attachment_index));
      CHECK_GL_CALL(glBlitFramebuffer(0, 0, this->options.width, this->options.height, 0, 0, this->options.width,
                                      this->options.height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
    }
  }
  
  int32_t Gl_framebuffer::read_pixel(uint32_t attachment_index, int32_t position_x, int32_t position_y)
  {
    CHECK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
    CHECK_GL_CALL(glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index));
    int pixelData;
    CHECK_GL_CALL(glReadPixels(position_x, position_y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData));
    
    return pixelData;
  }
  
  void Gl_framebuffer::clear_attachment([[maybe_unused]] uint32_t attachment_index,
                                        [[maybe_unused]] int32_t clear_value)
  {
    if (this->color_attachments.empty() && !this->depth_attachment.attachment_texture) return;
    if (this->color_attachments.size() <= attachment_index) return;
    
    Framebuffer_attachment_s &attachment = this->color_attachments[attachment_index];
    CHECK_GL_CALL(glClearTexImage(attachment.attachment_texture->get_id(), 0, GL_RED_INTEGER, GL_INT, &clear_value));
  }
  
  void Gl_framebuffer::send_gpu([[maybe_unused]] uint64_t instance_count)
  {
    CHECK_GL_CALL(glCreateFramebuffers(1, &this->renderer_id));
    CHECK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
    
    auto vbo = Vertex_buffer::create(sizeof(float), 16);
    std::vector<Buffer_element> b_elements;
    b_elements.emplace_back(Buffer_data_type::Vector_2f, "Position", false);
    b_elements.emplace_back(Buffer_data_type::Vector_2f, "Texture coords", false);
    Buffer_layout vbo_layout(b_elements);
    vbo->set_layout(vbo_layout);
    
    this->data.vao = Vertex_array_buffer::create();
    this->data.vbo_data = new float[16];
    this->data.ibo_data = new uint32_t[6];
    memcpy(this->data.ibo_data, index_buffer_data, sizeof(index_buffer_data));
    auto ibo = Index_buffer::create(this->data.ibo_data, 6);
    this->data.vao->add_vertex_buffer(vbo);
    this->data.vao->set_index_buffer(ibo);
    
    for (Framebuffer_attachment_s &attachment: this->color_attachments)
    {
      attachment.attachment_texture->send_gpu(1);
      int64_t texture_enum = this->options.samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
      int32_t gl_attachment = convert_to_gl_attachment(attachment.buffer_target,
                                                       attachment.attachment_index);
      
      CHECK_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment, texture_enum,
                                           attachment.attachment_texture->get_id(), 0));
    }
    
    if (this->depth_attachment.attachment_texture)
    {
      this->depth_attachment.attachment_texture->send_gpu(1);
      
      int64_t texture_enum = this->options.samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
      int32_t gl_attachment = convert_to_gl_attachment(this->depth_attachment.buffer_target,
                                                       this->depth_attachment.attachment_index);
      
      CHECK_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment, texture_enum,
                                           this->depth_attachment.attachment_texture->get_id(), 0));
    }
    
    int64_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
      Gl_renderer::gl_synchronous_error_callback(status,
                                                 "Cannot show framebuffer, framebuffer incomplete!",
                                                 "Gl_framebuffer()",
                                                 "gl_framebuffer.cpp",
                                                 __LINE__ - 5);
    }
    this->sent = true;
    Gl_framebuffer::unbind();
  }
  
  void Gl_framebuffer::free_gpu(uint64_t instance_count)
  {
    unbind();
    if (this->is_sent())
    {
      CHECK_GL_CALL(glDeleteFramebuffers(1, &this->renderer_id));
      for (const auto &attachment: this->color_attachments) attachment.attachment_texture->free_gpu(instance_count);
      if (this->depth_attachment.attachment_texture) this->depth_attachment.attachment_texture->free_gpu(1);
      
      this->sent = false;
    }
  }
  
  std::string Gl_framebuffer::to_string() const
  {
    std::string buffer("[Gl framebuffer] :\n");
    
    std::vector<Framebuffer_attachment_s> all_attachments = this->color_attachments;
    all_attachments.emplace_back(this->depth_attachment);
    
    for (const auto &attachment: all_attachments)
    {
      std::string type = buffer_type_to_string(attachment.buffer_target);
      char buffer_c[LINE_MAX]{0};
      if (snprintf(buffer_c, sizeof(buffer_c), "%55sType --> %s\n%55sID --> %d\n%55sWidth --> %d\n"
                                               "%55sHeight --> %d\n%55sSamples --> %d",
                   DEFAULT, type.c_str(), DEFAULT, this->renderer_id,
                   DEFAULT, this->options.width, DEFAULT, this->options.height, DEFAULT, this->options.samples) < 0)
      {
        return "ERROR : Snprintf failed while trying to print [Gl framebuffer]!";
      }
    }
    return buffer;
  }
  
  void Gl_framebuffer::reset()
  {
    if (this->renderer_id)
    {
      CHECK_GL_CALL(glDeleteFramebuffers(1, &this->renderer_id));
      
      CHECK_GL_CALL(glCreateFramebuffers(1, &this->renderer_id));
      CHECK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
      
      
      for (Framebuffer_attachment_s &attachment: this->color_attachments)
      {
        attachment.attachment_texture->free_gpu(1);
        
        // Creating the 2D texture of our viewport.
        attachment.attachment_texture->set_width(this->options.width);
        attachment.attachment_texture->set_height(this->options.height);
        
        attachment.attachment_texture->send_gpu(1);
        
        int64_t texture_enum = this->options.samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        int32_t gl_attachment = convert_to_gl_attachment(attachment.buffer_target,
                                                         attachment.attachment_index);
        
        CHECK_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment, texture_enum,
                                             attachment.attachment_texture->get_id(), 0));
      }
      
      if (this->depth_attachment.attachment_texture)
      {
        this->depth_attachment.attachment_texture->free_gpu(1);
        
        this->depth_attachment.attachment_texture->set_width(this->options.width);
        this->depth_attachment.attachment_texture->set_height(this->options.height);
        
        this->depth_attachment.attachment_texture->send_gpu(1);
        
        int64_t texture_enum = this->options.samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        int32_t gl_attachment = convert_to_gl_attachment(this->depth_attachment.buffer_target,
                                                         this->depth_attachment.attachment_index);
        
        CHECK_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment, texture_enum,
                                             this->depth_attachment.attachment_texture->get_id(), 0));
      }
      
      int64_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (status != GL_FRAMEBUFFER_COMPLETE)
      {
        Gl_renderer::gl_synchronous_error_callback(status,
                                                   "Cannot show framebuffer, framebuffer incomplete!",
                                                   "Gl_framebuffer()",
                                                   "gl_framebuffer.cpp",
                                                   __LINE__ - 5);
      }
      
      if (this->color_attachments.size() > 1)
      {
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        CHECK_GL_CALL(glDrawBuffers((int) this->color_attachments.size(), buffers));
      } else if (this->color_attachments.empty())
      {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
      }
      
      Gl_framebuffer::unbind();
    }
  }
  
  void Gl_framebuffer::bind()
  {
    if (!this->is_sent()) this->send_gpu(1);
    CHECK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
    
    Gl_renderer::set_viewport((float) this->options.width, (float) this->options.height);
  }
  
  void Gl_framebuffer::resize(int32_t width, int32_t height, void *data_)
  {
    assert(width > 0 && height > 0);
    if (width != this->options.width || height != this->options.height)
    {
      alert(WAVE_LOG_INFO, "[GL Framebuffer] --> Resized framebuffer to (%d, %d)", width, height);
    }
    this->options.width = width;
    this->options.height = height;
    
    this->reset();
    if (data_) this->on_resize_draw_data(data_);
  }
  
  void Gl_framebuffer::on_resize_draw_data(void *data_)
  {
    auto *viewport_rectangle = (Math::Vector_4f *) data_;
    
    // defines the vertices of the viewport quad
    const float vertex_buffer_data[16]
      {
        // Positions
        viewport_rectangle->get_x() + viewport_rectangle->get_z(),
        viewport_rectangle->get_y() + viewport_rectangle->get_w(),
        // Tex_coords
        1.0f,
        0.0f,
        // Positions
        viewport_rectangle->get_x() + viewport_rectangle->get_z(),
        viewport_rectangle->get_y(),
        // Tex_coords
        1.0f,
        1.0f,
        // Positions
        viewport_rectangle->get_x(),
        viewport_rectangle->get_y(),
        // Tex_coords
        0.0f,
        1.0f,
        // Positions
        viewport_rectangle->get_x(),
        viewport_rectangle->get_y() + viewport_rectangle->get_w(),
        // Tex_coords
        0.0f,
        0.0f
      };
    
    memcpy(this->data.vbo_data, vertex_buffer_data, sizeof(vertex_buffer_data));
    this->data.vao->get_vertex_buffers().back()->set_data(this->data.vbo_data, sizeof(vertex_buffer_data), 0);
    this->data.vao->unbind();
  }
  
  void Gl_framebuffer::unbind() const
  {
    if (!this->sent)
    {
      char buffer[FILENAME_MAX]{0};
      if (snprintf(buffer, sizeof(buffer),
                   "[Gl framebuffer] --> Cannot unbind framebuffer, framebuffer not sent to the gpu!"
                   "\n%55sDid you forget to send in your shader beforehand with send_gpu()"
                   " or bind()?", DEFAULT) < 0)
      {
        alert(WAVE_LOG_ERROR, "[Gl framebuffer] --> Internal error occurred (snprintf) on line %d, in file %s!",
              __LINE__, __FILE__);
      }
      Gl_renderer::gl_synchronous_error_callback(WAVE_GL_BUFFER_NOT_LOADED,
                                                 buffer,
                                                 __FUNCTION__, "gl_framebuffer.cpp", __LINE__);
      return;
    }
    CHECK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }
  
  uint32_t Gl_framebuffer::get_id() const
  {
    return this->renderer_id;
  }
  
  const Framebuffer_options_s &Gl_framebuffer::get_options() const
  {
    return this->options;
  }
  
  const std::vector<Framebuffer_attachment_s> &Gl_framebuffer::get_color_attachments()
  {
    return this->color_attachments;
  }
  
  const Framebuffer_attachment_s &Gl_framebuffer::get_depth_attachment()
  {
    return this->depth_attachment;
  }
}