//
// Created by nami on 23/04/23.
//

#include <Renderer/gl_framebuffer.h>
#include <Renderer/renderer.h>

namespace Wave
{
  
  Gl_framebuffer::Gl_framebuffer(const Framebuffer_options &opt)
  {
    if (this->renderer_id)
    {
      gl_call(glDeleteFramebuffers(1, &this->renderer_id));
      gl_call(glDeleteTextures(1, &this->color_attachment));
    }
    
    gl_call(glCreateFramebuffers(1, &this->renderer_id));
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
    
    // Creating the 2D texture of our viewport.
    gl_call(glCreateTextures(GL_TEXTURE_2D, 1, &this->color_attachment));
    gl_call(glBindTexture(GL_TEXTURE_2D, this->color_attachment));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
    gl_call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, opt.width, opt.height, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    
    // Depth attachment.
    gl_call(glCreateTextures(GL_TEXTURE_2D, 1, &this->depth_attachment));
    gl_call(glBindTexture(GL_TEXTURE_2D, this->depth_attachment));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    gl_call(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, opt.width, opt.height, 0,
                         GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
    
    // Bind textures to framebuffer.
    gl_call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->color_attachment, 0));
    gl_call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,this->depth_attachment, 0));
    
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
  }
  
  Gl_framebuffer::~Gl_framebuffer()
  {
    if (this->renderer_id)
    {
      gl_call(glDeleteFramebuffers(1, &this->renderer_id));
      gl_call(glDeleteTextures(1, &this->color_attachment));
    }
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
    
    gl_call(glDrawBuffer(GL_NONE));
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      alert(WAVE_ERROR, "[GL Framebuffer] --> Cannot show framebuffer, framebuffer incomplete!");
    }
    
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }
  
  void Gl_framebuffer::bind()
  {
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
    gl_call(glViewport(0, 0, this->options.width, this->options.height));
  }
  
  void Gl_framebuffer::resize(float width, float height)
  {
    if (width == 0 || height == 0)
    {
      alert(WAVE_WARN, "[GL Framebuffer] --> Attempted to resize framebuffer to %.2f, %.2f", width, height);
      return;
    }
    this->options.width = width;
    this->options.height = height;
    
    this->reset();
  }
  
  void Gl_framebuffer::unbind()
  {
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }
  
  void Gl_framebuffer::remove()
  {
    if (this->renderer_id)
    {
      gl_call(glDeleteFramebuffers(1, &this->renderer_id));
      gl_call(glDeleteTextures(1, &this->color_attachment));
    }
    
    gl_call(glCreateFramebuffers(1, &this->renderer_id));
    gl_call(glCreateTextures(GL_TEXTURE_2D, 1, &this->color_attachment));
    gl_call(glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id));
  }
  
  const Framebuffer_options &Gl_framebuffer::get_options() const
  {
    return this->options;
  }
  
  uint32_t Gl_framebuffer::get_color_attachment() const
  {
    return this->color_attachment;
  }
}