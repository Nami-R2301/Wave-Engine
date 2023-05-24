//
// Created by nami on 2022-02-23.
//

#pragma once

#include <Math/matrix_4f.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <Renderer/color.h>

namespace Wave
{
  
  class Shader : public Printable
  {
    public:
    ~Shader() override = default;
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    
    virtual void source(uint32_t count, const char *source, int *length) const = 0;
    virtual void compile(int32_t shader_id) const = 0;
    virtual void attach(int32_t shader_id) const = 0;
    virtual void detach() const = 0;
    virtual void link() const = 0;
    virtual void validate() const = 0;
    
    static std::shared_ptr<Shader> create(const std::string &name, const char *vertex_source,
                                          const char *fragment_source);
    [[nodiscard]] virtual int32_t get_id() const = 0;
    
    // Uniform handling.
    [[nodiscard]] virtual int get_uniform_location(const char *uniform_name) const = 0;
    virtual void set_uniform(const char *uniform_name, const Color &color) const = 0;
    virtual void set_uniform(const char *uniform_name, const float *matrix_4f, bool transpose) const = 0;
    
    virtual void set_uniform(const char *uniform_name, bool bool_value) const = 0;
    virtual void set_uniform(const char *uniform_name, int32_t int_value) const = 0;
    virtual void set_uniform(const char *uniform_name, uint32_t uint_value) const = 0;
    virtual void set_uniform(const char *uniform_name, float float_value) const = 0;
    virtual void set_uniform(const char *uniform_name, glm::mat4x4 value) const = 0;
    [[maybe_unused]] virtual void set_uniform(const char *uniform_name, const Vector_3f &vector_3f) const = 0;
  };
}