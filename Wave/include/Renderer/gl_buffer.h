//
// Created by nami on 29/03/23.
//

#pragma once

#include <Renderer/buffer.h>

namespace Wave
{
  
  enum Buffer_type
  {
    STATIC_DRAW,
    DYNAMIC_DRAW,
    STREAM_DRAW [[maybe_unused]]
  };
  
  class Gl_vertex_buffer : public Vertex_buffer
  {
  public:
    explicit Gl_vertex_buffer(uint64_t size);
    Gl_vertex_buffer(const void *data, uint64_t size, Buffer_type buffer_type = STATIC_DRAW);
    ~Gl_vertex_buffer() override;
    
    [[nodiscard]] uint32_t get_id() const override;
    [[nodiscard]] bool is_bound() const override;
    void set_data(const void *data, uint64_t size, uint64_t offset) override;
    void bind() override;
    void unbind() const override;
    void remove() override;
    
    [[nodiscard]] const Buffer_layout &get_layout() const override;
    void set_layout(const Buffer_layout &layout) override;
  private:
    uint32_t vbo_id = 0;
    Buffer_layout b_layout;
  };
  
  constexpr uint32_t INDEX_SIZE = sizeof(uint32_t);
  
  class Gl_index_buffer : public Index_buffer
  {
  public:
    Gl_index_buffer() = default;
    Gl_index_buffer(const void *data, uint32_t count_);
    ~Gl_index_buffer() override;
    
    [[nodiscard]] uint32_t get_id() const override;
    [[nodiscard]] bool is_bound() const override;
    void bind() override;
    void unbind() const override;
    void remove() override;
    
    inline void set_count(uint32_t count_)
    {
      this->count = count_;
    }
    [[nodiscard]] inline uint32_t get_count() const override
    {
      return this->count;
    };
  private:
    uint32_t index_buffer_id = 0;
    uint32_t count = 0;
  };
}
