//
// Created by nami on 2022-02-23.
//

#include <Renderer/renderer.h>
#include "Renderer/shader.h"

namespace Wave
{
  
  std::shared_ptr<Shader> Shader::create(const std::string &name, const char *vertex_source,
                                         const char *fragment_source)
  {
    switch (Gl_renderer::get_api())
    {
      case Renderer_api::Opengl:return create_shared_pointer<Gl_shader>(name, vertex_source, fragment_source);
      default:
        gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                      "shader.cpp",
                                      "Api not supported at the moment! Please select OpenGL instead.",
                                      "Shader::create(const std::string &name, const char *vertex_source, "
                                      "                                         const char *fragment_source)"
                                      "const std::string& fragment_source)", __LINE__ - 9);
    }
    return create_shared_pointer<Gl_shader>(name, vertex_source, fragment_source);
  }
  
  Gl_shader::Gl_shader(const std::string &name_, const char *vertex_source, const char *fragment_source)
  {
    this->name = name_;
    GL_CALL(this->program_id = glCreateProgram());
    if (this->program_id == -1)
    {
      gl_synchronous_error_callback(GL_DEBUG_SOURCE_OTHER,
                                    "Either GLEW is not initialized properly or GPU messed up!",
                                    "Gl_shader::Gl_shader(const std::string &name_, const char *vertex_source, const char *fragment_source)",
                                    "shader.cpp", __LINE__ - 6);
    }
    add_shader(GL_VERTEX_SHADER, vertex_source);
    add_shader(GL_FRAGMENT_SHADER, fragment_source);
    Gl_shader::link();
    Gl_shader::validate();
    this->uniform_cache = std::unordered_map<const char *, int32_t>();
  }
  
  //TODO : Extract vertex shader and fragment shader in one source shader file.
//  Gl_shader::Gl_shader(const char *program_file_path)
//  {
//  }
  
  Gl_shader::~Gl_shader()
  {
    this->detach();
    LOG_INSTRUCTION("SHADER", DEFAULT, "Deleting shader program", GL_CALL(glDeleteProgram(this->program_id)))
    this->uniform_cache.clear();
  }
  
  void Gl_shader::add_shader(int32_t type, const char *source)
  {
    if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) return;
    GL_CALL(int32_t shader_id = glCreateShader(type));
    if (!shader_id)
    {
      gl_synchronous_error_callback(GL_DEBUG_SOURCE_OTHER,
                                    "Unsupported shader type given or GPU messed up!",
                                    "GL_shader::add_shader(int32_t type, const char *source)",
                                    "shader.cpp", __LINE__ - 5);
    }
    Gl_shader::source(shader_id, source, nullptr);
    Gl_shader::compile(shader_id);
    attach(shader_id);
    GL_CALL(glDeleteShader(shader_id));  // Don't leak the shader.
    if (type == GL_VERTEX_SHADER)
    { this->vertex_shader_id = shader_id; }
    else
    { this->fragment_shader_id = shader_id; }
  }
  
  //Set the parse_shader code in program_id to the parse_shader code in the array of strings specified by string.
  void Gl_shader::source(uint32_t shader_, const char *source, int32_t *length) const
  {
    GL_CALL(glShaderSource(shader_, 1, &source, length));
  }
  
  // Compile the shaders into machine code to pass on to the GPU.
  void Gl_shader::compile(int32_t shader) const
  {
    GL_CALL(glCompileShader(shader));
    
    GLint is_compiled_successfully = 0;
    GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled_successfully));
    if (is_compiled_successfully == GL_FALSE)
    {
      GLint buffer_length = 0;
      GL_CALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &buffer_length));
      
      // Buffer_length includes the NULL character
      std::vector<GLchar> error_log(buffer_length);
      GL_CALL(glGetShaderInfoLog(shader, buffer_length, &buffer_length, &error_log[0]));
      
      gl_synchronous_error_callback(GL_DEBUG_SOURCE_SHADER_COMPILER,
                                    error_log.data(),
                                    "Gl_shader::compile(int32_t shader)",
                                    "shader.cpp",
                                    __LINE__ - 16);
      return;
    }
  }
  
  // Set our program_id to attach our created program_id to it (link parse_shader codes like '#include').
  void Gl_shader::attach(int32_t shader_id_) const
  {
    GL_CALL(glAttachShader(this->program_id, shader_id_));
  }
  
  // Links all parse_shader codes (shaders) in the program_id given as argument, creating the executable.
  void Gl_shader::link() const
  {
    GL_CALL(glLinkProgram(this->program_id));
  }
  
  void Gl_shader::validate() const
  {
    GL_CALL(glValidateProgram(this->program_id));
  }
  
  void Gl_shader::bind() const
  {
    GL_CALL(glUseProgram(this->program_id));
  }
  
  void Gl_shader::unbind() const
  {
    GL_CALL(glUseProgram(0));
  }
  
  std::string Gl_shader::to_string() const
  {
    char buffer[FILENAME_MAX * 4]{0};
    if (snprintf(buffer, sizeof(buffer), "[OpenGL shader] :\n%48sName --> %s", DEFAULT, this->name.c_str()) < 0)
    {
      return "ERROR : Snprintf failed while trying to print [Gl_shader]!";
    }
    return buffer;
  }
  
  int Gl_shader::get_uniform_location(const char *uniform_name) const
  {
    // Caching uniforms.
    auto cache = this->uniform_cache.find(uniform_name);
    if (cache != this->uniform_cache.end()) return cache->second;
    GL_CALL(int uniform_location = glGetUniformLocation(this->program_id, uniform_name));
    if (uniform_location == -1)
    {
      gl_synchronous_error_callback(GL_DEBUG_SOURCE_INVALID_UNIFORM,
                                    "Uniform not found in current active program or program is"
                                    " not active!",
                                    "get_uniform_location(const char *uniform_name)",
                                    "shader.cpp", __LINE__ - 7);
    }
    this->uniform_cache.emplace(uniform_name, uniform_location);
    return this->uniform_cache[uniform_name];
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, const float *matrix) const
  {
    GL_CALL(glUniformMatrix4fv(get_uniform_location(uniform_name), 1, GL_TRUE, matrix));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, bool bool_value) const
  {
    GL_CALL(glUniform1i(get_uniform_location(uniform_name), bool_value));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, int int_value) const
  {
    GL_CALL(glUniform1i(get_uniform_location(uniform_name), int_value));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, float float_value) const
  {
    GL_CALL(glUniform1f(get_uniform_location(uniform_name), float_value));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, glm::mat4x4 glm_matrix) const
  {
    GL_CALL(glUniformMatrix4fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(glm_matrix)));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, const Vector_3f &vector_3f) const
  {
    GL_CALL(glUniform3f(get_uniform_location(uniform_name), vector_3f.get_x(),
                        vector_3f.get_y(),
                        vector_3f.get_z()));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, const Color &color) const
  {
    GL_CALL(glUniform4f(get_uniform_location(uniform_name),
                        color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()));
  }
  
  void Gl_shader::detach() const
  {
    LOG_INSTRUCTION("SHADER", DEFAULT, "Detaching vertex shader id --> ",
                    GL_CALL(glDetachShader(this->program_id, this->vertex_shader_id)))
    LOG_INSTRUCTION("SHADER", DEFAULT, "Detaching fragment shader id --> ",
                    GL_CALL(glDetachShader(this->program_id, this->fragment_shader_id)))
  }
  
  int32_t Gl_shader::get_id() const
  {
    return this->program_id;
  }
}