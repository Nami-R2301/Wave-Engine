//
// Created by nami on 29/03/23.
//

#pragma once

#include <Renderer/buffer.h>

namespace Wave
{
  class Vertex_array_buffer
  {
  public:
    virtual ~Vertex_array_buffer() = default;
    
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void remove() = 0;
    [[nodiscard]] virtual bool is_bound() const = 0;
    
    virtual void add_vertex_buffer(const std::shared_ptr<Vertex_buffer> &vertexBuffer) = 0;
    virtual void set_index_buffer(const std::shared_ptr<Index_buffer> &indexBuffer) = 0;
    
    [[nodiscard]] virtual uint32_t get_id() const = 0;
    [[nodiscard]] virtual const std::vector<std::shared_ptr<Vertex_buffer>> &get_vertex_buffers() const = 0;
    [[nodiscard]] virtual const std::shared_ptr<Index_buffer> &get_index_buffer() const = 0;
    
    static std::shared_ptr<Vertex_array_buffer> create();
  protected:
    bool bound = false;
  };
}
