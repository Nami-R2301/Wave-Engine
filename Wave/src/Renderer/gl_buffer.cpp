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
    CHECK_GL_CALL(glBufferData(GL_ARRAY_BUFFER, size * count, data,
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
  
  void Gl_vertex_buffer::resize(uint64_t requested_vbo_size)
  {
    if (requested_vbo_size <= 0 || requested_vbo_size >= INT_MAX)
    {
      alert(WAVE_LOG_ERROR, "[Gl vertex buffer] --> Attempted to resize vbo to %lld", requested_vbo_size);
      return;
    }
    
    if (requested_vbo_size <= this->buffer_count * this->buffer_size)
    {
      this->buffer_count = requested_vbo_size / this->buffer_size;
      return;
    }
    
    
    CHECK_GL_CALL(glBindBuffer(GL_COPY_READ_BUFFER, this->vbo_id));
    
    // Copy the current buffer contents to a new one.
    uint32_t new_buffer = 0;
    CHECK_GL_CALL(glCreateBuffers(1, &new_buffer));
    CHECK_GL_CALL(glBindBuffer(GL_COPY_WRITE_BUFFER, new_buffer));
    CHECK_GL_CALL(glBufferData(GL_COPY_WRITE_BUFFER, requested_vbo_size, nullptr, GL_STATIC_DRAW));
    
    CHECK_GL_CALL(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                                      this->buffer_count * this->buffer_size));
    
    Gl_vertex_buffer::remove();
    this->vbo_id = new_buffer;  // Substitute pointer.
    this->buffer_count = requested_vbo_size / this->buffer_size;  // Update size.
    
    CHECK_GL_CALL(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
    CHECK_GL_CALL(glBindBuffer(GL_COPY_READ_BUFFER, 0));
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
    this->bound = true;
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
  
  Gl_index_buffer::Gl_index_buffer(const void *data, uint32_t count_, uint32_t size) : buffer_count(count_),
                                                                                       buffer_size(size)
  {
    CHECK_GL_CALL(glGenBuffers(1, &this->index_buffer_id));
    Gl_index_buffer::bind();
    
    data ? CHECK_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * size, data, GL_STATIC_DRAW)) :
    CHECK_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * size, data, GL_DYNAMIC_DRAW));
  }
  
  Gl_index_buffer::~Gl_index_buffer()
  {
    if (Gl_index_buffer::is_bound())
    {
      Gl_index_buffer::unbind();
      glDeleteBuffers(1, &this->index_buffer_id);
    }
  }
  
  void Gl_index_buffer::resize(uint64_t requested_ibo_size)
  {
    if (requested_ibo_size <= 0 || requested_ibo_size >= INT_MAX)
    {
      alert(WAVE_LOG_ERROR, "[Gl vertex buffer] --> Attempted to resize ibo to %lld", requested_ibo_size);
      return;
    }
    
    if (requested_ibo_size <= this->buffer_count * this->buffer_size)
    {
      this->buffer_count = requested_ibo_size / this->buffer_size;
      return;
    }
    
    CHECK_GL_CALL(glBindBuffer(GL_COPY_READ_BUFFER, this->index_buffer_id));
    
    // Copy the current buffer contents to a new one.
    uint32_t new_buffer = 0;
    CHECK_GL_CALL(glCreateBuffers(1, &new_buffer));
    CHECK_GL_CALL(glBindBuffer(GL_COPY_WRITE_BUFFER, new_buffer));
    CHECK_GL_CALL(glBufferData(GL_COPY_WRITE_BUFFER, requested_ibo_size, nullptr, GL_STATIC_DRAW));
    
    CHECK_GL_CALL(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                                      this->buffer_count * this->buffer_size));
    
    Gl_index_buffer::remove();
    this->index_buffer_id = new_buffer;  // Substitute pointer.
    this->buffer_count = requested_ibo_size / this->buffer_size;  // Update size.
    
    CHECK_GL_CALL(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
    CHECK_GL_CALL(glBindBuffer(GL_COPY_READ_BUFFER, 0));
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
    this->buffer_count = count_;
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
