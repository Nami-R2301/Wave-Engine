//
// Created by nami on 23/04/23.
//

#include <Renderer/gl_renderer.h>
#include <cassert>

namespace Wave
{
  
  static const uint32_t index_buffer_data[6] = {2,
                                                1,
                                                0,
                                                2,
                                                0,
                                                3};
  
  Gl_framebuffer::Gl_framebuffer(const Framebuffer_options &opt)
  {
    this->options = opt;
    
    // Creating the 2D texture of our viewport.
    this->color_attachment = new Gl_texture_2D(nullptr, {Texture::Texture_type_e::Texture_2D,
                                                         Texture::Texture_internal_format_e::Rgba8,
                                                         this->options.width,
                                                         this->options.height,
                                                         0,
                                                         2,
                                                         static_cast<int32_t>(this->options.samples),
                                                         nullptr});
    
    // Depth attachment.
    this->depth_attachment = new Gl_texture_2D(nullptr, {Texture::Texture_type_e::Texture_2D,
                                                         Texture::Texture_internal_format_e::Depth_stencil,
                                                         this->options.width,
                                                         this->options.height,
                                                         0,
                                                         3,
                                                         static_cast<int32_t>(this->options.samples),
                                                         nullptr});
  }
  
  Gl_framebuffer::~Gl_framebuffer()
  {
    Gl_framebuffer::free_gpu();
    delete this->color_attachment;
    delete this->depth_attachment;
    delete[] this->data.ibo_data;
    delete[] this->data.vbo_data;
  }
  
  void Gl_framebuffer::send_gpu()
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
    
    this->color_attachment->send_gpu();
    this->depth_attachment->send_gpu();
    
    int64_t texture_enum = this->options.samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    
    
    CHECK_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_enum,
                                         this->color_attachment->get_id(), 0));
    CHECK_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, texture_enum,
                                         this->depth_attachment->get_id(), 0));
    
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
  
  void Gl_framebuffer::free_gpu()
  {
    unbind();
    if (this->is_sent())
    {
      CHECK_GL_CALL(glDeleteFramebuffers(1, &this->renderer_id));
      if (this->color_attachment) this->color_attachment->free_gpu();
      if (this->depth_attachment) this->depth_attachment->free_gpu();
      
      this->sent = false;
    }
  }
  
  std::string Gl_framebuffer::to_string() const
  {
    char buffer[FILENAME_MAX * 4]{0};
    if (snprintf(buffer, sizeof(buffer), "[Gl framebuffer] :\n%55sID --> %d\n%55sWidth --> %.2f\n"
                                         "%55sHeight --> %.2f\n%55sSamples --> %d", DEFAULT, this->renderer_id,
                 DEFAULT, this->options.width, DEFAULT, this->options.height, DEFAULT, this->options.samples) < 0)
    {
      return "ERROR : Snprintf failed while trying to print [Gl framebuffer]!";
    }
    return buffer;
  }
  
  void Gl_framebuffer::reset()
  {
    if (this->renderer_id)
    {
      CHECK_GL_CALL(glDeleteFramebuffers(1, &this->renderer_id));
      this->color_attachment->free_gpu();
      this->depth_attachment->free_gpu();
      
      CHECK_GL_CALL(glCreateFramebuffers(1, &this->renderer_id));
      CHECK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
      
      // Creating the 2D texture of our viewport.
      this->color_attachment->set_width(this->options.width);
      this->color_attachment->set_height(this->options.height);
      
      this->depth_attachment->set_width(this->options.width);
      this->depth_attachment->set_height(this->options.height);
      
      this->color_attachment->send_gpu();
      this->depth_attachment->send_gpu();
      
      int64_t texture_enum = this->options.samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
      
      
      CHECK_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_enum,
                                           this->color_attachment->get_id(), 0));
      CHECK_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, texture_enum,
                                           this->depth_attachment->get_id(), 0));
      
      int64_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (status != GL_FRAMEBUFFER_COMPLETE)
      {
        Gl_renderer::gl_synchronous_error_callback(status,
                                                   "Cannot show framebuffer, framebuffer incomplete!",
                                                   "Gl_framebuffer()",
                                                   "gl_framebuffer.cpp",
                                                   __LINE__ - 5);
      }
      Gl_framebuffer::unbind();
    }
  }
  
  void Gl_framebuffer::bind()
  {
    if (!this->is_sent()) this->send_gpu();
    CHECK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
    Gl_renderer::set_viewport(this->options.width, this->options.height);
  }
  
  void Gl_framebuffer::resize(float width, float height, void *data_)
  {
    assert(width > 0 && height > 0);
    if (width != this->options.width || height != this->options.height)
    {
      alert(WAVE_LOG_INFO, "[GL Framebuffer] --> Resized framebuffer to (%.2f, %.2f)", width, height);
    }
    this->options.width = width;
    this->options.height = height;
    
    this->reset();
    this->on_resize_draw_data(data_);
  }
  
  void Gl_framebuffer::on_resize_draw_data(void *data_)
  {
    auto *viewport_rectangle = (Vector_4f *) data_;
    
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
  
  const Framebuffer_options &Gl_framebuffer::get_options() const
  {
    return this->options;
  }
  
  Texture *Gl_framebuffer::get_color_attachment()
  {
    return this->color_attachment;
  }
  
  Texture *Gl_framebuffer::get_depth_attachment()
  {
    return this->depth_attachment;
  }
}