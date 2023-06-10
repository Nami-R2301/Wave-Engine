//
// Created by nami on 10/05/23.
//

#pragma once

#include <Renderer/shader.h>

namespace Wave
{
  
  class Gl_shader : public Shader
  {
    public:
    Gl_shader() = default;
//    explicit Gl_shader(const char *program_file_path);  // In case the vertex and fragment shaders are written in a single file.
    Gl_shader(const std::string &name, const std::string &vertex_source, const std::string &fragment_source);
    ~Gl_shader() override;
    
    void add_shader(int32_t type, const char *source);
    void bind() override;
    void unbind() const override;
    [[nodiscard]] int32_t get_id() const override;
    
    INTERFACE_PRINTABLE
    INTERFACE_SENDABLE
    
    
    void source(uint32_t count, const char *source, int *length) const override;
    void compile(int32_t shader_id) const override;
    void attach(int32_t shader_id) const override;
    void detach() const override;
    void link() const override;
    void validate() const override;
    
    // Uniform handling.
    [[nodiscard]] const std::unordered_map<const char *, int> &get_uniforms() const override;
    [[nodiscard]] int get_uniform_location(const char *uniform_name) const override;
    void set_uniform(const char *uniform_name, const Color &color) const override;
    void set_uniform(const char *uniform_name, const float *matrix_4f, bool transpose) const override;
    
    void set_uniform(const char *uniform_name, bool value) const override;
    void set_uniform(const char *uniform_name, int32_t value) const override;
    void set_uniform(const char *uniform_name, uint32_t value) const override;
    void set_uniform(const char *uniform_name, float value) const override;
    void set_uniform(const std::string &uniform_name, const Gpu_light_struct_s &light_struct) const override;
    
    [[maybe_unused]] void set_uniform(const char *uniform_name, const Vector_3f &vector_3f) const override;
    
    bool operator==(const Shader &other_shader) const override;
    private:
    std::string name = "No name";
    std::string vertex_source = "Empty", fragment_source = "Empty";
    int32_t program_id = 0;
    int32_t vertex_shader_id = 0;
    int32_t fragment_shader_id = 0;
    mutable std::unordered_map<const char *, int> uniform_cache;
  };
}
