#pragma once

#include <Renderer/vertex_array_buffer.h>

namespace Wave
{
  class Gl_vertex_array_buffer : public Vertex_array_buffer
  {
    public:
    Gl_vertex_array_buffer();
    ~Gl_vertex_array_buffer() override;
    
    void bind() const override;
    void unbind() const override;
    void remove() override;
    [[nodiscard]] bool is_bound() const override;
    
    void add_vertex_buffer(const std::shared_ptr<Vertex_buffer> &vertexBuffer) override;
    void set_index_buffer(const std::shared_ptr<Index_buffer> &indexBuffer) override;
    
    [[nodiscard]] uint32_t get_id() const override;
    [[nodiscard]] const std::vector<std::shared_ptr<Vertex_buffer>> &get_vertex_buffers() const override;
    [[nodiscard]] const std::shared_ptr<Index_buffer> &get_index_buffer() const override;
    private:
    uint32_t vertex_array_id = 0;
    uint32_t vertex_buffer_index = 0;
    std::vector<std::shared_ptr<Vertex_buffer>> vertex_buffers;
    std::shared_ptr<Index_buffer> index_buffer;
  };
}