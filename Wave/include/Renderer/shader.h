//
// Created by nami on 2022-02-23.
//

#pragma once

#include <Math/matrix_4f.h>
#include <Renderer/color.h>

namespace Wave
{
  
  typedef struct Gpu_light_struct_s
  {
    Color color;
    float ambient_intensity;
    float diffuse_intensity;
  } Gpu_light_struct_s;
  
  class Shader : public I_printable, public I_sendable
  {
    public:
    ~Shader() override = default;
    static std::shared_ptr<Shader> create(const std::string &name, const std::string &vertex_source,
                                          const std::string &fragment_source);
    
    virtual void bind() = 0;
    virtual void unbind() const = 0;
    
    virtual void source(uint32_t count, const char *source, int *length) const = 0;
    virtual void compile(int32_t shader_id) const = 0;
    virtual void attach(int32_t shader_id) const = 0;
    virtual void detach() const = 0;
    virtual void link() const = 0;
    virtual void validate() const = 0;
    
    
    [[nodiscard]] virtual int32_t get_id() const = 0;
    [[nodiscard]] virtual const std::unordered_map<const char *, int> &get_uniforms() const = 0;
    [[nodiscard]] virtual int get_uniform_location(const char *uniform_name) const = 0;
    
    virtual void bind_texture_units() = 0;
    
    virtual void set_uniform(const char *uniform_name, const Color &color) const = 0;
    virtual void set_uniform(const char *uniform_name, const float *matrix_4f, bool transpose) const = 0;
    virtual void set_uniform(const char *uniform_name, bool bool_value) const = 0;
    virtual void set_uniform(const char *uniform_name, int32_t int_value) const = 0;
    virtual void set_uniform(const char *uniform_name, uint32_t uint_value) const = 0;
    virtual void set_uniform(const char *uniform_name, float float_value) const = 0;
    virtual void set_uniform(const std::string &uniform_name, const Gpu_light_struct_s &light_struct) const = 0;
    [[maybe_unused]] virtual void set_uniform(const char *uniform_name, const Math::Vector_3f &vector_3f) const = 0;
    
    virtual bool operator==(const Shader &other_shader) const = 0;
    bool operator!=(const Shader &other_shader) const;
    protected:
    bool sent = false;
  };
}