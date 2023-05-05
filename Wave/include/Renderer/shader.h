//
// Created by nami on 2022-02-23.
//

#pragma once

#include <wave_pch.h>
#include <Math/matrix_4f.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

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
    virtual void set_uniform(const char *uniform_name, const float *matrix_4f) const = 0;
    
    virtual void set_uniform(const char *uniform_name, bool value) const = 0;
    virtual void set_uniform(const char *uniform_name, int value) const = 0;
    virtual void set_uniform(const char *uniform_name, float value) const = 0;
    virtual void set_uniform(const char *uniform_name, glm::mat4x4 value) const = 0;
    [[maybe_unused]] virtual void set_uniform(const char *uniform_name, const Vector_3f &vector_3f) const = 0;
  };
  
  class Gl_shader : public Shader
  {
    public:
    Gl_shader() = default;
//    explicit Gl_shader(const char *program_file_path);  // In case the vertex and fragment shaders are written in a single file.
    Gl_shader(const std::string &name, const char *vertex_source, const char *fragment_source);
    ~Gl_shader() override;
    
    void add_shader(int32_t type, const char *source);
    void bind() const override;
    void unbind() const override;
    [[nodiscard]] int32_t get_id() const override;
    
    INTERFACE_PRINT
    
    void source(uint32_t count, const char *source, int *length) const override;
    void compile(int32_t shader_id) const override;
    void attach(int32_t shader_id) const override;
    void detach() const override;
    void link() const override;
    void validate() const override;
    
    // Uniform handling.
    [[nodiscard]] int get_uniform_location(const char *uniform_name) const override;
    void set_uniform(const char *uniform_name, const Color &color) const override;
    void set_uniform(const char *uniform_name, const float *matrix_4f) const override;
    
    void set_uniform(const char *uniform_name, bool value) const override;
    void set_uniform(const char *uniform_name, int value) const override;
    void set_uniform(const char *uniform_name, float value) const override;
    void set_uniform(const char *uniform_name, glm::mat4x4 value) const override;
    [[maybe_unused]] void set_uniform(const char *uniform_name, const Vector_3f &vector_3f) const override;
    private:
    std::string name = "No name";
    int32_t program_id = 0;
    int32_t vertex_shader_id = 0;
    int32_t fragment_shader_id = 0;
    mutable std::unordered_map<const char *, int> uniform_cache;
  };
}