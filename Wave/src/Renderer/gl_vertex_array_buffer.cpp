#include <Renderer/gl_vertex_array_buffer.h>
#include <Renderer/renderer.h>

namespace Wave
{
  static GLenum convert_buffer_data_type_to_gl(Buffer_data_type type)
  {
    switch (type)
    {
      case Buffer_data_type::Float:return GL_FLOAT;
      case Buffer_data_type::Vector_2f:return GL_FLOAT;
      case Buffer_data_type::Vector_3f:return GL_FLOAT;
      case Buffer_data_type::Vector_4f:return GL_FLOAT;
      case Buffer_data_type::Color_4f:return GL_FLOAT;
      case Buffer_data_type::Matrix_3f:return GL_FLOAT;
      case Buffer_data_type::Matrix_4f:return GL_FLOAT;
      case Buffer_data_type::Int:return GL_INT;
      case Buffer_data_type::Vector_2i:return GL_INT;
      case Buffer_data_type::Vector_3i:return GL_INT;
      case Buffer_data_type::Vector_4i:return GL_INT;
      case Buffer_data_type::Bool:return GL_BOOL;
      case Buffer_data_type::None:break;
    }
    
    alert(WAVE_ERROR, "[GL VAO BUFFER] --> Unknown Buffer data type (on line %d, in file %s) !",
          __LINE__, __FILE__);
    return 0;
  }
  
  Gl_vertex_array_buffer::Gl_vertex_array_buffer()
  {
    log_instruction("VAO", DEFAULT, "Creating vertex array", gl_call(glCreateVertexArrays(1, &this->vertex_array_id)))
  }
  
  Gl_vertex_array_buffer::~Gl_vertex_array_buffer()
  {
    if (this->is_bound()) glDeleteVertexArrays(1, &this->vertex_array_id);
  }
  
  void Gl_vertex_array_buffer::bind() const
  {
    gl_call(glBindVertexArray(this->vertex_array_id));
  }
  
  void Gl_vertex_array_buffer::unbind() const
  {
    gl_call(glBindVertexArray(0));
  }
  
  bool Gl_vertex_array_buffer::is_bound() const
  {
    return this->bound;
  }
  
  void Gl_vertex_array_buffer::remove()
  {
    log_instruction("VAO", DEFAULT, "Deleting vertex array", gl_call(glDeleteVertexArrays(1, &this->vertex_array_id)))
    log_instruction("VAO", DEFAULT, "Deleting vertex buffers",
                    for (const auto &vertex_buffer: this->vertex_buffers)
                    {
                      log_instruction("VAO", DEFAULT, "Deleting vertex buffer", vertex_buffer->remove())
                    })
    
    log_instruction("VAO", DEFAULT, "Deleting index buffer", this->index_buffer->remove())
    this->bound = false;
  }
  
  void Gl_vertex_array_buffer::add_vertex_buffer(const std::shared_ptr<Vertex_buffer> &vertex_buffer_)
  {
    if (vertex_buffer_->get_layout().get_elements().empty())
    {
      alert(WAVE_ERROR, "[GL VAO BUFFER] --> Vertex Buffer has no layout! (n line %d, in file %s)",
            __LINE__, __FILE__);
      return;
    }
    
    Gl_vertex_array_buffer::bind();
    vertex_buffer_->bind();
    
    const auto &layout = vertex_buffer_->get_layout();
    for (const auto &element: layout)
    {
      switch (element.type)
      {
        case Buffer_data_type::Float:
        case Buffer_data_type::Vector_2f:
        case Buffer_data_type::Vector_3f:
        case Buffer_data_type::Color_4f:
        case Buffer_data_type::Vector_4f:
        {
          glEnableVertexAttribArray(this->vertex_buffer_index);
          glVertexAttribPointer(this->vertex_buffer_index,
                                static_cast<GLint>(element.get_property_count()),
                                convert_buffer_data_type_to_gl(element.type),
                                element.normalized ? GL_TRUE : GL_FALSE,
                                static_cast<GLint>(layout.get_stride()),
                                (const void *) element.offset);
          this->vertex_buffer_index++;
          break;
        }
        case Buffer_data_type::Int:
        case Buffer_data_type::Vector_2i:
        case Buffer_data_type::Vector_3i:
        case Buffer_data_type::Vector_4i:
        case Buffer_data_type::Bool:
        {
          glEnableVertexAttribArray(this->vertex_buffer_index);
          glVertexAttribIPointer(this->vertex_buffer_index,
                                 static_cast<GLint>(element.get_property_count()),
                                 convert_buffer_data_type_to_gl(element.type),
                                 static_cast<GLint>(layout.get_stride()),
                                 (const void *) element.offset);
          this->vertex_buffer_index++;
          break;
        }
        case Buffer_data_type::Matrix_3f:
        case Buffer_data_type::Matrix_4f:
        {
          uint64_t count = element.get_property_count();
          for (uint64_t i = 0; i < count; i++)
          {
            glEnableVertexAttribArray(this->vertex_buffer_index);
            glVertexAttribPointer(this->vertex_buffer_index,
                                  static_cast<GLint>(count),
                                  convert_buffer_data_type_to_gl(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE,
                                  static_cast<GLint>(layout.get_stride()),
                                  (const void *) (element.offset + sizeof(float) * count * i));
            glVertexAttribDivisor(this->vertex_buffer_index, 1);
            this->vertex_buffer_index++;
          }
          break;
        }
        default:
          alert(WAVE_ERROR, "[GL VAO BUFFER] --> Unknown Buffer_data_type! (on line %d, in file %s)",
                __LINE__, __FILE__);
      }
    }
    
    this->vertex_buffers.push_back(vertex_buffer_);
  }

  uint32_t Gl_vertex_array_buffer::get_id() const
  {
      return this->vertex_array_id;
  }
  
  void Gl_vertex_array_buffer::set_index_buffer(const std::shared_ptr<Index_buffer> &index_buffer_)
  {
    Gl_vertex_array_buffer::bind();
    index_buffer_->bind();
    
    this->index_buffer = index_buffer_;
  }
  
  const std::vector<std::shared_ptr<Vertex_buffer>> &Gl_vertex_array_buffer::get_vertex_buffers() const
  {
    return this->vertex_buffers;
  }
  
  const std::shared_ptr<Index_buffer> &Gl_vertex_array_buffer::get_index_buffer() const
  {
    return this->index_buffer;
  }
}