//
// Created by nami on 11/05/23.
//

#include <Renderer/gl_uniform_buffer.h>
#include <Renderer/gl_renderer.h>

namespace Wave
{
  
  Gl_uniform_buffer::Gl_uniform_buffer(uint32_t size, uint32_t binding)
  {
    CHECK_GL_CALL(glCreateBuffers(1, &this->renderer_id));
    CHECK_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, this->renderer_id));
    CHECK_GL_CALL(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    CHECK_GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, this->renderer_id));
  }
  
  Gl_uniform_buffer::~Gl_uniform_buffer()
  {
    glDeleteBuffers(1, &this->renderer_id);
  }
  
  void Gl_uniform_buffer::set_data(uint32_t offset, uint32_t size, const void *data)
  {
    CHECK_GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
  }
} // Wave