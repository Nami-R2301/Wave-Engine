//
// Created by nami on 23/04/23.
//

#include <Renderer/gl_framebuffer.h>
#include <Renderer/renderer.h>

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
    gl_call(glCreateFramebuffers(1, &this->renderer_id));
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
    
    // Determine max samples.
    GLint max_samples = 0;
    gl_call(glGetIntegerv(GL_MAX_SAMPLES, &max_samples));
    
    // Creating the 2D texture of our viewport.
    gl_call(glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &this->color_attachment));
    gl_call(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->color_attachment));
    gl_call(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, max_samples, GL_RGBA8, opt.width, opt.height,
                                    GL_FALSE));
    
    // Depth attachment.
//    gl_call(glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &this->depth_attachment));
//    gl_call(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->depth_attachment));
//    gl_call(
//        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, max_samples, GL_DEPTH24_STENCIL8, opt.width, opt.height,
//                                GL_FALSE));
    
    gl_call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE,
                                   this->color_attachment, 0));
//    gl_call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE,
//                                   this->depth_attachment, 0));
    
    uint64_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
      Gl_renderer::gl_error_callback(status,
                                     "Cannot show framebuffer, framebuffer incomplete!",
                                     "Gl_framebuffer()",
                                     "gl_framebuffer.cpp",
                                     __LINE__ - 5);
    }
    this->options = opt;
    Gl_framebuffer::unbind();
    
    auto vbo = Vertex_buffer::create(16 * sizeof(float));
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
  }
  
  Gl_framebuffer::~Gl_framebuffer()
  {
    if (this->renderer_id)
    {
      gl_call(glDeleteFramebuffers(1, &this->renderer_id));
      gl_call(glDeleteTextures(1, &this->color_attachment));
      gl_call(glDeleteTextures(1, &this->depth_attachment));
    }
    delete[] this->data.ibo_data;
    delete[] this->data.vbo_data;
  }
  
  void Gl_framebuffer::reset()
  {
    if (this->renderer_id)
    {
      gl_call(glDeleteFramebuffers(1, &this->renderer_id));
      gl_call(glDeleteTextures(1, &this->color_attachment));
      gl_call(glDeleteTextures(1, &this->depth_attachment));
    }
    
    gl_call(glCreateFramebuffers(1, &this->renderer_id));
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
    
    // Determine max samples.
    GLint max_samples = 0;
    gl_call(glGetIntegerv(GL_MAX_SAMPLES, &max_samples));
    
    // Creating the 2D texture of our viewport.
    gl_call(glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &this->color_attachment));
    gl_call(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->color_attachment));
    gl_call(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, max_samples, GL_RGBA8, this->options.width,
                                    this->options.height,
                                    GL_FALSE));
    
    // Depth attachment.
//    gl_call(glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &this->depth_attachment));
//    gl_call(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->depth_attachment));
//    gl_call(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, max_samples, GL_DEPTH24_STENCIL8, this->options.width,
//                                    this->options.height, GL_FALSE));
//
    gl_call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE,
                                   this->color_attachment, 0));
//    gl_call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE,
//                                   this->depth_attachment, 0));
    
    uint64_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
      Gl_renderer::gl_error_callback(status,
                                     "Cannot show framebuffer, framebuffer incomplete!",
                                     "Gl_framebuffer()",
                                     "gl_framebuffer.cpp",
                                     __LINE__ - 5);
    }
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      alert(WAVE_ERROR, "[GL Framebuffer] --> Cannot show framebuffer, framebuffer incomplete!");
    }

    gl_call(glDrawBuffer(GL_COLOR_ATTACHMENT0));
    
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }
  
  void Gl_framebuffer::bind()
  {
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
    gl_call(glViewport(0, 0, this->options.width, this->options.height));
  }
  
  void Gl_framebuffer::resize(float width, float height, void *data_)
  {
    assert(width != 0 && height != 0);
    if (width != this->options.width || height != this->options.height)
    {
      alert(WAVE_WARN, "[GL Framebuffer] --> Resized framebuffer to (%.2f, %.2f)", width, height);
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
//    alert(WAVE_WARN, "Vertex 0 : %.2f; Vertex 1 : %.2f; Vertex 5 : %.2f; Vertex 6 : %.2f; Vertex 10 %.2f;"
//                     " Vertex 11 %.2f; Vertex 15 %.2f; Vertex 16 %.2f",
//          vertex_buffer_data[0], vertex_buffer_data[1], vertex_buffer_data[5], vertex_buffer_data[6],
//          vertex_buffer_data[10],
//          vertex_buffer_data[11], vertex_buffer_data[15], vertex_buffer_data[16]);
    this->data.vao->unbind();
  }
  
  void Gl_framebuffer::unbind()
  {
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }
  
  void Gl_framebuffer::remove()
  {
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    if (this->renderer_id)
    {
      gl_call(glDeleteFramebuffers(1, &this->renderer_id));
      gl_call(glDeleteTextures(1, &this->color_attachment));
      gl_call(glDeleteTextures(1, &this->depth_attachment));
    }
  }
  
  const Framebuffer_options &Gl_framebuffer::get_options() const
  {
    return this->options;
  }
  
  uint32_t Gl_framebuffer::get_color_attachment() const
  {
    return this->color_attachment;
  }
  
  uint32_t Gl_framebuffer::get_depth_attachment() const
  {
    return this->depth_attachment;
  }
}