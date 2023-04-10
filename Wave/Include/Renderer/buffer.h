//
// Created by nami on 29/03/23.
//

#pragma once

#include <wave_pch.h>
#include <Core/core.h>

#include <utility>

namespace Wave
{
  enum class Buffer_data_type
  {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
  };
  
  static uint32_t Buffer_data_type_size(Buffer_data_type type)
  {
    switch (type)
    {
      case Buffer_data_type::None:return 0;
      case Buffer_data_type::Float:return 4;
      case Buffer_data_type::Float2:return 4 * 2;
      case Buffer_data_type::Float3:return 4 * 3;
      case Buffer_data_type::Float4:return 4 * 4;
      case Buffer_data_type::Mat3:return 4 * 3 * 3;
      case Buffer_data_type::Mat4:return 4 * 4 * 4;
      case Buffer_data_type::Int:return 4;
      case Buffer_data_type::Int2:return 4 * 2;
      case Buffer_data_type::Int3:return 4 * 3;
      case Buffer_data_type::Int4:return 4 * 4;
      case Buffer_data_type::Bool:return 1;
    }
    
    alert(WAVE_ERROR, "[BUFFER] --> Unknown Shader_data_type on line %d in file %d!", __LINE__, __FILE__);
    return 0;
  }
  
  struct Buffer_element
  {
  public:
    Buffer_element() = default;
    
    Buffer_element(Buffer_data_type type, std::string name, bool normalized = false)
        : name(std::move(name)), type(type), size(Buffer_data_type_size(type)), offset(0), normalized(normalized)
    {
    }
    
    [[nodiscard]] uint64_t get_property_count() const
    {
      switch (type)
      {
        case Buffer_data_type::None:return 0;
        case Buffer_data_type::Float:return 1;
        case Buffer_data_type::Float2:return 2;
        case Buffer_data_type::Float3:return 3;
        case Buffer_data_type::Float4:return 4;
        case Buffer_data_type::Mat3:return 3; // 3* float3
        case Buffer_data_type::Mat4:return 4; // 4* float4
        case Buffer_data_type::Int:return 1;
        case Buffer_data_type::Int2:return 2;
        case Buffer_data_type::Int3:return 3;
        case Buffer_data_type::Int4:return 4;
        case Buffer_data_type::Bool:return 1;
      }
      
      alert(WAVE_ERROR, "[BUFFER] --> Unknown Shader_data_type on line %d in file %d!", __LINE__, __FILE__);
      return 0;
    }
    
    std::string name;
    Buffer_data_type type = Buffer_data_type::None;
    uint64_t size = 0;
    size_t offset = 0;
    bool normalized = false;
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
    { return stride; }
    [[nodiscard]] const std::vector<Buffer_element> &get_elements() const
    { return elements; }
    
    std::vector<Buffer_element>::iterator begin()
    { return elements.begin(); }
    std::vector<Buffer_element>::iterator end()
    { return elements.end(); }
    [[nodiscard]] std::vector<Buffer_element>::const_iterator begin() const
    { return elements.begin(); }
    [[nodiscard]] std::vector<Buffer_element>::const_iterator end() const
    { return elements.end(); }
  private:
    void calculate_offsets_and_stride()
    {
      size_t offset = 0;
      stride = 0;
      for (auto &element: elements)
      {
        element.offset = offset;
        offset += element.size;
        stride += element.size;
      }
    }
  private:
    std::vector<Buffer_element> elements;
    uint64_t stride = 0;
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
    
    [[nodiscard]] virtual const Buffer_layout &get_layout() const = 0;
    virtual void set_layout(const Buffer_layout &layout) = 0;
    
    static std::shared_ptr<Vertex_buffer> Create(uint64_t size);
    static std::shared_ptr<Vertex_buffer> Create(const float *vertices, uint64_t size);
  protected:
    bool bound = false;
  };
  
  class Index_buffer
  {
  public:
    virtual ~Index_buffer() = default;
    
    [[nodiscard]] virtual bool is_bound() const = 0;
    virtual void bind() = 0;
    virtual void unbind() const = 0;
    virtual void remove() = 0;
    
    [[nodiscard]] virtual uint32_t get_count() const = 0;
    
    static std::shared_ptr<Index_buffer> Create(uint32_t *indices, uint32_t count);
  protected:
    bool bound = false;
  };
}
