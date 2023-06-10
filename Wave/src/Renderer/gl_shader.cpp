//
// Created by nami on 10/05/23.
//

#include <Renderer/gl_renderer.h>
#include <Renderer/gl_shader.h>


namespace Wave
{
  Gl_shader::Gl_shader(const std::string &name_, const std::string &vertex_source_, const std::string &fragment_source_)
  {
    this->name = name_;
    this->vertex_source = vertex_source_;
    this->fragment_source = fragment_source_;
    this->uniform_cache = std::unordered_map<const char *, int32_t>();
  }

//  }

  Gl_shader::~Gl_shader()
  {
    Gl_shader::free_gpu();
  }
  
  void Gl_shader::send_gpu()
  {
    
    CHECK_GL_CALL(this->program_id = glCreateProgram());
    if (this->program_id == -1)
    {
      Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_OTHER,
                                                 "Either GLEW is not initialized properly or GPU messed up!",
                                                 "Gl_shader::Gl_shader(const std::string &name_, const char *vertex_source, const char *fragment_source)",
                                                 "shader.cpp", __LINE__ - 6);
    }
    add_shader(GL_VERTEX_SHADER, this->vertex_source.c_str());
    add_shader(GL_FRAGMENT_SHADER, this->fragment_source.c_str());
    Gl_shader::link();
    Gl_shader::validate();
    
    this->sent = true;
  }
  
  void Gl_shader::free_gpu()
  {
    if (this->is_sent())
    {
      this->detach();
      WAVE_LOG_INSTRUCTION("Shader", DEFAULT, "Deleting shader program",
                           CHECK_GL_CALL(glDeleteProgram(this->program_id)))
      if (!this->uniform_cache.empty()) this->uniform_cache.clear();
      this->sent = false;
    }
  }
  
  void Gl_shader::add_shader(int32_t type, const char *source)
  {
    if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) return;
    CHECK_GL_CALL(int32_t
                    shader_id = glCreateShader(type));
    if (!shader_id)
    {
      Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_OTHER,
                                                 "Unsupported shader type given or GPU messed up!",
                                                 "GL_shader::add_shader(int32_t type, const char *source)",
                                                 "shader.cpp", __LINE__ - 5);
    }
    Gl_shader::source(shader_id, source, nullptr);
    Gl_shader::compile(shader_id);
    attach(shader_id);
    CHECK_GL_CALL(glDeleteShader(shader_id));  // Don't leak the shader.
    if (type == GL_VERTEX_SHADER)
    { this->vertex_shader_id = shader_id; }
    else
    { this->fragment_shader_id = shader_id; }
  }

//Set the parse_shader code in program_id to the parse_shader code in the array of strings specified by string.
  void Gl_shader::source(uint32_t shader_, const char *source, int32_t *length) const
  {
    CHECK_GL_CALL(glShaderSource(shader_, 1, &source, length));
  }

// Compile the shaders into machine code to pass on to the GPU.
  void Gl_shader::compile(int32_t shader) const
  {
    CHECK_GL_CALL(glCompileShader(shader));
    
    GLint is_compiled_successfully = 0;
    CHECK_GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled_successfully));
    if (is_compiled_successfully == GL_FALSE)
    {
      GLint buffer_length = 0;
      CHECK_GL_CALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &buffer_length));
      
      // Buffer_length includes the NULL character
      std::vector<GLchar> error_log(buffer_length);
      CHECK_GL_CALL(glGetShaderInfoLog(shader, buffer_length, &buffer_length, &error_log[0]));
      
      Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_SHADER_COMPILER,
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
    CHECK_GL_CALL(glAttachShader(this->program_id, shader_id_));
  }

// Links all parse_shader codes (shaders) in the program_id given as argument, creating the executable.
  void Gl_shader::link() const
  {
    CHECK_GL_CALL(glLinkProgram(this->program_id));
  }
  
  void Gl_shader::validate() const
  {
    CHECK_GL_CALL(glValidateProgram(this->program_id));
  }
  
  void Gl_shader::bind()
  {
    if (!this->sent) this->send_gpu();
    CHECK_GL_CALL(glUseProgram(this->program_id));
  }
  
  void Gl_shader::unbind() const
  {
    if (!this->sent)
    {
      char buffer[FILENAME_MAX]{0};
      if (snprintf(buffer, sizeof(buffer), "[Gl shader] --> Cannot unbind shader, shader not sent to the gpu!"
                                           "\n%55sDid you forget to send in your shader beforehand with send_gpu()"
                                           " or bind()?", DEFAULT) < 0)
      {
        alert(WAVE_LOG_ERROR, "[Gl shader] --> Internal error occurred (snprintf) on line %d, in file %s!",
              __LINE__, __FILE__);
      }
      Gl_renderer::gl_synchronous_error_callback(WAVE_GL_BUFFER_NOT_LOADED,
                                                 buffer,
                                                 __FUNCTION__, "gl_shader.cpp", __LINE__);
      return;
    }
    CHECK_GL_CALL(glUseProgram(0));
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

//TODO : Extract vertex shader and fragment shader in one source shader file.
//  Gl_shader::Gl_shader(const char *program_file_path)
//  {
  const std::unordered_map<const char *, int> &Gl_shader::get_uniforms() const
  {
    return this->uniform_cache;
  }
  
  int Gl_shader::get_uniform_location(const char *uniform_name) const
  {
    // Caching uniforms.
    auto cache = this->uniform_cache.find(uniform_name);
    if (cache != this->uniform_cache.end()) return cache->second;
    CHECK_GL_CALL(int
                    uniform_location = glGetUniformLocation(this->program_id, uniform_name));
    if (uniform_location == -1)
    {
      char buffer[FILENAME_MAX * 4]{0};
      snprintf(buffer, sizeof(buffer), "'%s' not found in current active program or program is not active",
               uniform_name);
      Gl_renderer::gl_synchronous_error_callback(WAVE_GL_DEBUG_SOURCE_INVALID_UNIFORM,
                                                 buffer,
                                                 "get_uniform_location(const char *uniform_name)",
                                                 "shader.cpp", __LINE__ - 7);
    }
    this->uniform_cache.emplace(uniform_name, uniform_location);
    return this->uniform_cache[uniform_name];
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, const float *matrix, bool transpose) const
  {
    CHECK_GL_CALL(glUniformMatrix4fv(get_uniform_location(uniform_name), 1, transpose, matrix));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, bool bool_value) const
  {
    CHECK_GL_CALL(glUniform1i(get_uniform_location(uniform_name), bool_value));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, int32_t int_value) const
  {
    CHECK_GL_CALL(glUniform1i(get_uniform_location(uniform_name), int_value));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, uint32_t unsigned_value) const
  {
    CHECK_GL_CALL(glUniform1ui(get_uniform_location(uniform_name), unsigned_value));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, float float_value) const
  {
    CHECK_GL_CALL(glUniform1f(get_uniform_location(uniform_name), float_value));
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, const Vector_3f &vector_3f) const
  {
    CHECK_GL_CALL(glUniform3f(get_uniform_location(uniform_name), vector_3f.get_x(),
                              vector_3f.get_y(),
                              vector_3f.get_z()));
  }
  
  void Gl_shader::set_uniform(const std::string &uniform_name, const Gpu_light_struct_s &light_struct) const
  {
    set_uniform((uniform_name + ".color").c_str(), light_struct.color);
    set_uniform((uniform_name + ".ambient_intensity").c_str(), light_struct.ambient_intensity);
    set_uniform((uniform_name + ".diffuse_intensity").c_str(), light_struct.diffuse_intensity);
  }
  
  void Gl_shader::set_uniform(const char *uniform_name, const Color &color) const
  {
    CHECK_GL_CALL(glUniform4f(get_uniform_location(uniform_name),
                              color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()));
  }
  
  void Gl_shader::detach() const
  {
    Gl_shader::unbind();
    CHECK_GL_CALL(glDetachShader(this->program_id, this->vertex_shader_id));
    CHECK_GL_CALL(glDetachShader(this->program_id, this->fragment_shader_id));
  }
  
  int32_t Gl_shader::get_id() const
  {
    return this->program_id;
  }
  
  bool Gl_shader::operator==(const Shader &other_shader) const
  {
    return this->program_id == other_shader.get_id();
  }
}