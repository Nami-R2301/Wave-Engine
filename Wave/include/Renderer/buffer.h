//
// Created by nami on 29/03/23.
//

#pragma once

#include <wave_pch.h>
#include <Renderer/color.h>

#include <utility>

namespace Wave
{
  enum class Buffer_data_type
  {
    None = 0,
    Float,
    Vector_2f,
    Vector_3f,
    Vector_4f,
    Color_4f,
    Matrix_3f,
    Matrix_4f,
    Int,
    Vector_2i,
    Vector_3i,
    Vector_4i,
    Bool
  };
  
  static uint32_t Buffer_data_type_size(Buffer_data_type type)
  {
    switch (type)
    {
      case Buffer_data_type::None:return 0;
      case Buffer_data_type::Float:return 4;
      case Buffer_data_type::Vector_2f:return 4 * 2;
      case Buffer_data_type::Vector_3f:return 4 * 3;
      case Buffer_data_type::Color_4f:
      case Buffer_data_type::Vector_4f:return 4 * 4;
      case Buffer_data_type::Matrix_3f:return 4 * 3 * 3;
      case Buffer_data_type::Matrix_4f:return 4 * 4 * 4;
      case Buffer_data_type::Int:return 4;
      case Buffer_data_type::Vector_2i:return 4 * 2;
      case Buffer_data_type::Vector_3i:return 4 * 3;
      case Buffer_data_type::Vector_4i:return 4 * 4;
      case Buffer_data_type::Bool:return 1;
    }
    
    alert(WAVE_LOG_ERROR, "[BUFFER] --> Unknown Shader_data_type on line %d in file %d!", __LINE__, __FILE__);
    return 0;
  }
  
  struct Buffer_element
  {
    public:
    Buffer_element() = default;
    
    Buffer_element(Buffer_data_type type, std::string name, bool normalized = false, bool flat = false)
      : name(std::move(name)), type(type), size(Buffer_data_type_size(type)), offset(0), normalized(normalized),
        flat(flat)
    {
    }
    
    [[nodiscard]] uint64_t get_property_count() const
    {
      switch (type)
      {
        case Buffer_data_type::None:return 0;
        case Buffer_data_type::Float:return 1;
        case Buffer_data_type::Vector_2f:return 2;
        case Buffer_data_type::Vector_3f:return 3;
        case Buffer_data_type::Color_4f:
        case Buffer_data_type::Vector_4f:return 4;
        case Buffer_data_type::Matrix_3f:return 3;
        case Buffer_data_type::Matrix_4f:return 4;
        case Buffer_data_type::Int:return 1;
        case Buffer_data_type::Vector_2i:return 2;
        case Buffer_data_type::Vector_3i:return 3;
        case Buffer_data_type::Vector_4i:return 4;
        case Buffer_data_type::Bool:return 1;
      }
      
      alert(WAVE_LOG_ERROR, "[BUFFER] --> Unknown Shader_data_type on line %d in file %d!", __LINE__, __FILE__);
      return 0;
    }
    
    std::string name;
    Buffer_data_type type = Buffer_data_type::None;
    uint64_t size = 0;
    size_t offset = 0;
    bool normalized = false;
    bool flat = false;
  };
  
  class Buffer_layout
  {
    public:
    Buffer_layout() = default;
    
    explicit Buffer_layout(const std::vector<Buffer_element> &elements)
      : elements(elements)
    {
      calculate_offsets_and_stride();
    }
    
    [[nodiscard]] uint64_t get_stride() const
    { return this->stride; }
    
    [[nodiscard]] const std::vector<Buffer_element> &get_elements() const
    { return this->elements; }
    
    std::vector<Buffer_element>::iterator begin()
    { return this->elements.begin(); }
    
    std::vector<Buffer_element>::iterator end()
    { return this->elements.end(); }
    
    [[nodiscard]] std::vector<Buffer_element>::const_iterator begin() const
    { return this->elements.begin(); }
    
    [[nodiscard]] std::vector<Buffer_element>::const_iterator end() const
    { return this->elements.end(); }
    
    private:
    void calculate_offsets_and_stride()
    {
      size_t offset = 0;
      this->stride = 0;
      for (auto &element: this->elements)
      {
        element.offset = offset;
        offset += element.size;
        this->stride += element.size;
      }
    }
    
    private:
    std::vector<Buffer_element> elements;
    uint64_t stride = 0;
  };
  
  enum Buffer_type
  {
    STATIC_DRAW,
    DYNAMIC_DRAW,
    STREAM_DRAW [[maybe_unused]]
  };
  
  class Vertex_buffer
  {
    public:
    virtual ~Vertex_buffer() = default;
    
    [[nodiscard]] virtual bool is_bound() const = 0;
    virtual void bind() = 0;
    virtual void unbind() const = 0;
    virtual void remove() = 0;
    
    virtual void set_data(const void *data, uint64_t size, uint64_t offset) = 0;
    virtual void set_count(uint32_t count_) = 0;
    
    [[nodiscard]] virtual uint32_t get_count() const = 0;
    [[nodiscard]] virtual uint32_t get_size() const = 0;
    [[nodiscard]] virtual uint32_t get_id() const = 0;
    [[nodiscard]] virtual const Buffer_layout &get_layout() const = 0;
    virtual void set_layout(const Buffer_layout &layout) = 0;
    
    static std::shared_ptr<Vertex_buffer> create(uint64_t size, uint64_t count);
    static std::shared_ptr<Vertex_buffer> create(const void *vertices, uint64_t size, uint64_t count,
                                                 Buffer_type buffer_type);
    protected:
    bool bound = false;
  };
  
  class Index_buffer
  {
    public:
    virtual ~Index_buffer() = default;
    
    [[nodiscard]] virtual uint32_t get_id() const = 0;
    [[nodiscard]] virtual bool is_bound() const = 0;
    virtual void bind() = 0;
    virtual void unbind() const = 0;
    virtual void remove() = 0;
    
    virtual void set_data(const void *data, uint64_t size, uint64_t offset) = 0;
    virtual void set_count(uint32_t count_) = 0;
    
    [[nodiscard]] virtual uint32_t get_count() const = 0;
    
    static std::shared_ptr<Index_buffer> create(const uint32_t *indices, uint32_t size);
    protected:
    bool bound = false;
  };
}
