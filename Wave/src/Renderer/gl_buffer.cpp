//
// Created by nami on 29/03/23.
//

#include <Renderer/gl_buffer.h>
#include <Renderer/renderer.h>

namespace Wave
{
  
  /*************************** Vertex buffer ***************************/
  
  Gl_vertex_buffer::Gl_vertex_buffer(uint64_t size)
  {
    GL_CALL(glGenBuffers(1, &this->vbo_id)); // Create empty buffer for our vertex_source data.
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
  }
  
  Gl_vertex_buffer::Gl_vertex_buffer(const void *data, size_t size, Buffer_type buffer_type)
  {
    GL_CALL(glGenBuffers(1, &this->vbo_id)); // Create empty buffer for our vertex_source data.
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data,
                         buffer_type == STATIC_DRAW ? GL_STATIC_DRAW : buffer_type == DYNAMIC_DRAW ?
                                                                       GL_DYNAMIC_DRAW : GL_STREAM_DRAW));
  }
  
  Gl_vertex_buffer::~Gl_vertex_buffer()
  {
    if (this->is_bound())
    {
      Gl_vertex_buffer::unbind();
      glDeleteBuffers(1, &this->vbo_id);
    }
  }
  
  uint32_t Gl_vertex_buffer::get_id() const
  {
    return this->vbo_id;
  }
  
  void Gl_vertex_buffer::set_data(const void *data, uint64_t size, uint64_t offset)
  {
    Gl_vertex_buffer::bind();
    GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
  }
  
  bool Gl_vertex_buffer::is_bound() const
  {
    return this->bound;
  }
  
  void Gl_vertex_buffer::bind()
  {
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id));
    Gl_vertex_buffer::bound = true;
  }
  
  void Gl_vertex_buffer::unbind() const
  {
    if (this->is_bound())
    {
      GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
  }
  
  void Gl_vertex_buffer::remove()
  {
    if (this->is_bound()) Gl_vertex_buffer::unbind();
    GL_CALL(glDeleteBuffers(1, &this->vbo_id));
    this->bound = false;
  }
  
  const Buffer_layout &Gl_vertex_buffer::get_layout() const
  {
    return this->b_layout;
  }
  
  void Gl_vertex_buffer::set_layout(const Buffer_layout &layout)
  {
    this->b_layout = layout;
  }
  
  /*************************** Index buffer ***************************/
  
  Gl_index_buffer::Gl_index_buffer(const void *data, uint32_t count_) : count(count_)
  {
    GL_CALL(glGenBuffers(1, &this->index_buffer_id));
    LOG_INSTRUCTION("IBO", DEFAULT, "Binding index buffer",
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);)
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count_ * INDEX_SIZE), data, GL_STATIC_DRAW));
  }
  
  Gl_index_buffer::~Gl_index_buffer()
  {
    if (Gl_index_buffer::is_bound())
    {
      Gl_index_buffer::unbind();
      glDeleteBuffers(1, &this->index_buffer_id);
    }
  }
  
  uint32_t Gl_index_buffer::get_id() const
  {
    return this->index_buffer_id;
  }
  
  bool Gl_index_buffer::is_bound() const
  {
    return this->bound;
  }
  
  void Gl_index_buffer::bind()
  {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id));
    Gl_index_buffer::bound = true;
  }
  
  void Gl_index_buffer::unbind() const
  {
    if (is_bound())
    {
      GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
  }
  
  void Gl_index_buffer::remove()
  {
    Gl_index_buffer::unbind();
    GL_CALL(glDeleteBuffers(1, &this->index_buffer_id));
    this->bound = false;
  }
}