//
// Created by nami on 29/03/23.
//

#pragma once

#include <Renderer/buffer.h>

namespace Wave
{
  
  class Gl_vertex_buffer : public Vertex_buffer
  {
    public:
    explicit Gl_vertex_buffer(uint64_t size, uint64_t count);
    Gl_vertex_buffer(const void *data, uint64_t size, uint64_t count, Buffer_type buffer_type = STATIC_DRAW);
    ~Gl_vertex_buffer() override;
    
    [[nodiscard]] uint32_t get_count() const override;
    [[nodiscard]] uint32_t get_size() const override;
    [[nodiscard]] uint32_t get_id() const override;
    [[nodiscard]] bool is_bound() const override;
    
    void bind() override;
    void unbind() const override;
    void remove() override;
    
    void resize(uint64_t requested_vbo_size) override;
    void set_count(uint32_t count_) override;
    void set_data(const void *data, uint64_t size, uint64_t offset) override;
    
    [[nodiscard]] const Buffer_layout &get_layout() const override;
    void set_layout(const Buffer_layout &layout) override;
    private:
    uint64_t buffer_count = 0;
    uint64_t buffer_size = 0;
    uint32_t vbo_id = 0;
    Buffer_layout b_layout;
  };
  
  class Gl_index_buffer : public Index_buffer
  {
    public:
    Gl_index_buffer() = default;
    Gl_index_buffer(const void *data, uint32_t count_, uint32_t size = sizeof(uint32_t));
    ~Gl_index_buffer() override;
    
    [[nodiscard]] uint32_t get_id() const override;
    [[nodiscard]] bool is_bound() const override;
    void bind() override;
    void unbind() const override;
    void remove() override;
    
    void resize(uint64_t requested_ibo_size) override;
    void set_data(const void *data, uint64_t size, uint64_t offset) override;
    void set_count(uint32_t count_) override;
    
    [[nodiscard]] inline uint32_t get_count() const override
    {
      return this->buffer_count;
    };
    private:
    uint32_t index_buffer_id = 0;
    uint32_t buffer_count = 0;
    uint32_t buffer_size = 0;
  };
}
