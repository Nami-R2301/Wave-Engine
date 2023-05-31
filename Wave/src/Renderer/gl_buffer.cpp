//
// Created by nami on 29/03/23.
//

#include <Renderer/gl_renderer.h>
#include <Renderer/gl_buffer.h>


namespace Wave
{
  
  /*************************** Vertex buffer ***************************/
  
  Gl_vertex_buffer::Gl_vertex_buffer(uint64_t size, uint64_t count)
  {
    CHECK_GL_CALL(glGenBuffers(1, &this->vbo_id)); // Create empty buffer for our vertex_source data.
    CHECK_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id));
    CHECK_GL_CALL(glBufferData(GL_ARRAY_BUFFER, size * count, nullptr, GL_DYNAMIC_DRAW));
    
    this->buffer_size = size;
    this->buffer_count = count;
  }
  
  Gl_vertex_buffer::Gl_vertex_buffer(const void *data, uint64_t size, uint64_t count, Buffer_type buffer_type)
  {
    this->buffer_size = size;
    this->buffer_count = count;
    
    CHECK_GL_CALL(glGenBuffers(1, &this->vbo_id)); // Create empty buffer for our vertex_source data.
    CHECK_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id));
    CHECK_GL_CALL(glBufferData(GL_ARRAY_BUFFER, this->buffer_size, data,
                               buffer_type == STATIC_DRAW ? GL_STATIC_DRAW :
                               buffer_type == DYNAMIC_DRAW ? GL_DYNAMIC_DRAW : GL_STREAM_DRAW));
  }
  
  Gl_vertex_buffer::~Gl_vertex_buffer()
  {
    if (this->is_bound())
    {
      Gl_vertex_buffer::unbind();
      glDeleteBuffers(1, &this->vbo_id);
    }
  }
  
  uint32_t Gl_vertex_buffer::get_count() const
  {
    return this->buffer_count;
  }
  
  void Gl_vertex_buffer::set_count(uint32_t count_)
  {
    this->buffer_count = count_;
  }
  
  uint32_t Gl_vertex_buffer::get_size() const
  {
    return this->buffer_size;
  }
  
  bool Gl_vertex_buffer::is_bound() const
  {
    return this->bound;
  }
  
  uint32_t Gl_vertex_buffer::get_id() const
  {
    return this->vbo_id;
  }
  
  void Gl_vertex_buffer::set_data(const void *data_, uint64_t size, uint64_t offset)
  {
    Gl_vertex_buffer::bind();
    CHECK_GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data_));
  }
  
  void Gl_vertex_buffer::bind()
  {
    CHECK_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id));
  }
  
  void Gl_vertex_buffer::unbind() const
  {
    if (this->is_bound())
    {
      CHECK_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
  }
  
  void Gl_vertex_buffer::remove()
  {
    Gl_vertex_buffer::unbind();
    CHECK_GL_CALL(glDeleteBuffers(1, &this->vbo_id));
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
    CHECK_GL_CALL(glGenBuffers(1, &this->index_buffer_id));
    Gl_index_buffer::bind();
    if (data)
    {
      CHECK_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count_ * INDEX_SIZE), data,
                                 GL_STATIC_DRAW));
    } else
    {
      CHECK_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count_ * INDEX_SIZE), data,
                                 GL_DYNAMIC_DRAW));
    }
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
  
  void Gl_index_buffer::set_count(uint32_t count_)
  {
    this->count = count_;
  }
  
  void Gl_index_buffer::bind()
  {
    CHECK_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id));
    Gl_index_buffer::bound = true;
  }
  
  void Wave::Gl_index_buffer::set_data(const void *data, uint64_t size, uint64_t offset)
  {
    Gl_index_buffer::bind();
    CHECK_GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
  }
  
  void Gl_index_buffer::unbind() const
  {
    if (is_bound())
    {
      CHECK_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
  }
  
  void Gl_index_buffer::remove()
  {
    Gl_index_buffer::unbind();
    CHECK_GL_CALL(glDeleteBuffers(1, &this->index_buffer_id));
    this->bound = false;
  }
}
