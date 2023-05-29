//
// Created by nami on 12/05/23.
//

#include <Renderer/gl_texture.h>
#include <Renderer/gl_renderer.h>

#define STB_IMAGE_IMPLEMENTATION

#include <Dependencies/Stb/stb_image.h>

namespace Wave
{
  
  int32_t convert_format_to_gl_format(Texture::Texture_internal_format_e format)
  {
    switch (format)
    {
      case Texture::Texture_internal_format_e::Red: return GL_RED;
      case Texture::Texture_internal_format_e::Rgba8:
      case Texture::Texture_internal_format_e::Rgba16:
      case Texture::Texture_internal_format_e::Rgba32: return GL_RGBA;
      case Texture::Texture_internal_format_e::Depth_stencil: return GL_DEPTH_STENCIL;
      default:
        Gl_renderer::gl_synchronous_error_callback(GL_INVALID_ENUM,
                                                   "[GL Texture] --> Invalid texture format given!",
                                                   __FUNCTION__, __FILE__, __LINE__ - 12);
        return 0;
    }
  }
  
  int32_t convert_internal_format_to_gl_internal_format(Texture::Texture_internal_format_e format)
  {
    switch (format)
    {
      case Texture::Texture_internal_format_e::Red: return GL_RED;
      case Texture::Texture_internal_format_e::Rgba8: return GL_RGBA8;
      case Texture::Texture_internal_format_e::Rgba16: return GL_RGBA16;
      case Texture::Texture_internal_format_e::Rgba32: return GL_RGBA32F;
      case Texture::Texture_internal_format_e::Depth_stencil: return GL_DEPTH24_STENCIL8;
      default:
        Gl_renderer::gl_synchronous_error_callback(GL_INVALID_ENUM,
                                                   "[GL Texture] --> Invalid texture format given!",
                                                   __FUNCTION__, __FILE__, __LINE__ - 12);
        return 0;
    }
  }
  
  int32_t convert_buffer_type_to_gl_buffer_type(Texture::Texture_internal_format_e buffer_type)
  {
    switch (buffer_type)
    {
      case Texture::Texture_internal_format_e::Red:
      case Texture::Texture_internal_format_e::Rgba8: return GL_UNSIGNED_BYTE;
      case Texture::Texture_internal_format_e::Rgba16: return GL_2_BYTES;
      case Texture::Texture_internal_format_e::Rgba32: return GL_FLOAT;
      case Texture::Texture_internal_format_e::Depth_stencil: return GL_UNSIGNED_INT_24_8;
      default:
        Gl_renderer::gl_synchronous_error_callback(GL_INVALID_ENUM,
                                                   "[GL Texture] --> Invalid texture format given!",
                                                   __FUNCTION__, __FILE__, __LINE__ - 12);
        return 0;
    }
  }
  
  /********************* TEXTURE 2D ********************/
  
  Gl_texture_2D::Gl_texture_2D(const char *file_path)
  {
    this->texture_data.type = Texture_type_e::Texture_2D;
    this->texture_data.internal_format = Texture_internal_format_e::Rgba8;
    this->texture_data.desired_samples = 1;
    this->texture_data.desired_slot = 1;
    this->texture_data.data = nullptr;
    this->texture_target = Gl_texture_2D::convert_type_to_api({this->texture_data.type,
                                                               this->texture_data.internal_format,
                                                               this->texture_data.desired_width,
                                                               this->texture_data.desired_height,
                                                               0,
                                                               this->texture_data.desired_slot,
                                                               this->texture_data.desired_samples,
                                                               this->texture_data.data});
    this->gl_format = convert_format_to_gl_format(this->texture_data.internal_format);
    this->gl_internal_format = convert_internal_format_to_gl_internal_format(this->texture_data.internal_format);
    this->gl_buffer_type = convert_buffer_type_to_gl_buffer_type(this->texture_data.internal_format);
    
    // Default text glyph texture slot given for renderer.
    // Initialize openGL texture buffers.
    CHECK_GL_CALL(glCreateTextures(this->texture_target, 1, &this->texture_id));
    
    if (file_path && strcmp(&file_path[strlen(file_path) - 3], "ttf") == 0)  // Generating font textures.
    {
      this->texture_data.desired_slot = 0;
      this->bind(this->texture_data.desired_slot);
      // set texture options
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    } else if (file_path)  // Generating file texture.
    {
      this->bind(this->texture_data.desired_slot);
      
      // Load file texture in memory.
      stbi_set_flip_vertically_on_load(1);  // Invert y-axis for opengl matrix coordinates.
      this->texture_data.data = (stbi_uc *) stbi_load(file_path,
                                                      &this->texture_data.desired_width,
                                                      &this->texture_data.desired_height,
                                                      &this->bits_per_pixel,
                                                      4);  // 4 channels (RGBA).
      if (!this->texture_data.data)
        Wave::alert(WAVE_LOG_ERROR, "[Texture] : Could not load image from %s", file_path);
      
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_R, GL_REPEAT));
    } else  // Generating empty buffer texture.
    {
      // Initialize openGL texture buffers.
      CHECK_GL_CALL(glCreateTextures(this->texture_target, 1, &this->texture_id));
      this->bind(this->texture_data.desired_slot);
      
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_R, GL_REPEAT));
    }
    
    this->set_data(&this->texture_data, nullptr);
    
    // Assign texture image to texture buffer.
    // Deallocate file texture from CPU since it's loaded onto the GPU.
    if (file_path && this->texture_data.data) stbi_image_free((stbi_uc *) this->texture_data.data);
  }
  
  Gl_texture_2D::Gl_texture_2D(const char *file_path, Texture_data_s data)
  {
    this->texture_data.type = data.type;
    this->texture_data.desired_width = data.desired_width;
    this->texture_data.desired_height = data.desired_height;
    this->texture_data.desired_samples = data.desired_samples;
    this->texture_data.desired_slot = data.desired_slot;
    this->texture_data.internal_format = data.internal_format;
    this->texture_data.data = data.data;
    this->texture_target = Gl_texture_2D::convert_type_to_api(data);
    this->gl_format = convert_format_to_gl_format(this->texture_data.internal_format);
    this->gl_internal_format = convert_internal_format_to_gl_internal_format(this->texture_data.internal_format);
    this->gl_buffer_type = convert_buffer_type_to_gl_buffer_type(this->texture_data.internal_format);
    
    CHECK_GL_CALL(glCreateTextures(this->texture_target, 1, &this->texture_id));
    this->bind(data.desired_slot);
    
    // Generating font textures.
    if (file_path && strcmp(&file_path[strlen(file_path) - 3], "ttf") == 0)
    {
      // Set texture options
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      
      if (data.type == Texture_type_e::Texture_2D_Array)
      {
        CHECK_GL_CALL(glTexImage3D(
          this->texture_target,
          0,
          this->gl_internal_format,
          data.desired_width == WAVE_VALUE_DONT_CARE ? 0 : data.desired_width,
          data.desired_height == WAVE_VALUE_DONT_CARE ? 0 : data.desired_height,
          data.desired_depth == WAVE_VALUE_DONT_CARE ? 0 : data.desired_depth,
          0,
          this->gl_format,
          this->gl_buffer_type,
          nullptr));
      } else
      {
        CHECK_GL_CALL(glTexImage2D(
          this->texture_target,
          0,
          this->gl_internal_format,
          data.desired_width == WAVE_VALUE_DONT_CARE ? 0 : data.desired_width,
          data.desired_height == WAVE_VALUE_DONT_CARE ? 0 : data.desired_height,
          0,
          this->gl_format,
          this->gl_buffer_type,
          data.data));
      }
    } else if (file_path)
    {
      // Load file texture in memory.
      stbi_set_flip_vertically_on_load(1);  // Invert y-axis for opengl matrix coordinates.
      data.data = (stbi_uc *) stbi_load(file_path, &this->texture_data.desired_width,
                                        &this->texture_data.desired_height, &this->bits_per_pixel,
                                        4);  // 4 channels (RGBA).
      this->texture_data.data = data.data;
      
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT));
      CHECK_GL_CALL(glTexParameteri(this->texture_target, GL_TEXTURE_WRAP_R, GL_REPEAT));
      
      
      if (!data.data)
      {
        Wave::alert(WAVE_LOG_ERROR, "[Texture] : Could not load image from %s", file_path);
      }
    } else
    {
      CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
      CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
      CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT));
    }
    
    this->set_data(&data, nullptr);
    
    // Assign texture image to texture buffer.
    // Deallocate file texture from CPU since it's loaded onto the GPU.
    if (file_path && data.data) stbi_image_free((stbi_uc *) this->texture_data.data);
  }
  
  Gl_texture_2D::~Gl_texture_2D()
  {
    Gl_texture_2D::remove();
  }
  
  int32_t Gl_texture_2D::convert_type_to_api(Texture_data_s data_)
  {
    switch (data_.type)
    {
      case Texture_type_e::Texture_2D:
      {
        if (data_.desired_samples > 1) return GL_TEXTURE_2D_MULTISAMPLE;
        return GL_TEXTURE_2D;
      }
      case Texture_type_e::Texture_2D_Array: return GL_TEXTURE_2D_ARRAY;
      case Texture_type_e::Texture_2D_Ms_array: return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
      default:
        Gl_renderer::gl_synchronous_error_callback(GL_INVALID_ENUM,
                                                   "[GL Texture 2D] --> Invalid texture type given for 2D texture!",
                                                   __FUNCTION__, __FILE__, __LINE__ - 12);
    }
    return 0;
  }
  
  void Gl_texture_2D::bind(int32_t slot_) const
  {
    if (*this)
    {
      CHECK_GL_CALL(glActiveTexture(
        slot_ == WAVE_VALUE_DONT_CARE ? GL_TEXTURE0 : GL_TEXTURE0 + slot_));  // Set our active texture slot.
      CHECK_GL_CALL(glBindTexture(this->texture_target, this->texture_id));
    }
  }
  
  void Gl_texture_2D::unbind() const
  {
    if (*this)
    {
      CHECK_GL_CALL(glActiveTexture(GL_TEXTURE0 + this->texture_data.desired_slot));
      CHECK_GL_CALL(glBindTexture(this->texture_target, 0));
    }
  }
  
  void Gl_texture_2D::remove()
  {
    if (*this)
    {
      unbind();
      glDeleteTextures(1, &this->texture_id);
    }
    this->texture_id = 255;
  }
  
  void Gl_texture_2D::set_data(const Texture_data_s *data_array, uint32_t offset_array[2]) const
  {
    this->bind(this->texture_data.desired_slot);
    
    // Supply texture data.
    switch (this->texture_data.type)
    {
      case Texture_type_e::Texture_2D:
      {
        if (data_array && data_array->desired_samples > 1)
        {
          CHECK_GL_CALL(glTexImage2DMultisample(this->texture_target,
                                                data_array->desired_samples == WAVE_VALUE_DONT_CARE ? 1 :
                                                data_array->desired_samples,
                                                this->gl_internal_format,
                                                data_array->desired_width == WAVE_VALUE_DONT_CARE ? 0 :
                                                data_array->desired_width,
                                                data_array->desired_height == WAVE_VALUE_DONT_CARE ? 0 :
                                                data_array->desired_height,
                                                GL_FALSE));
        } else if (data_array && offset_array)
        {
          CHECK_GL_CALL(glTexSubImage2D(this->texture_target,
                                        0,
                                        offset_array[0],
                                        offset_array[1],
                                        data_array->desired_width == WAVE_VALUE_DONT_CARE ? 0 :
                                        data_array->desired_width,
                                        data_array->desired_height == WAVE_VALUE_DONT_CARE ? 0 :
                                        data_array->desired_height,
                                        this->gl_format,
                                        this->gl_buffer_type,
                                        data_array->data));
        } else if (data_array)
        {
          CHECK_GL_CALL(glTexImage2D(this->texture_target,
                                     0,
                                     this->gl_internal_format,
                                     data_array->desired_width == WAVE_VALUE_DONT_CARE ? 0 :
                                     data_array->desired_width,
                                     data_array->desired_height == WAVE_VALUE_DONT_CARE ? 0 :
                                     data_array->desired_height,
                                     0,
                                     this->gl_format,
                                     this->gl_buffer_type,
                                     data_array->data));
        }
        break;
      }
      default:
      {
        alert(WAVE_LOG_WARN, "[GL texture] --> Incorrect or undefined data type supplied"
                             " in function %s, on line %d!", __FUNCTION__, __LINE__);
        
        return;
      }
    }
  }
  
  Texture::Texture_type_e Gl_texture_2D::get_type()
  {
    return this->texture_data.type;
  }
  
  uint32_t &Gl_texture_2D::get_id()
  {
    return this->texture_id;
  }
  
  int Gl_texture_2D::get_width() const
  {
    return this->texture_data.desired_width;
  }
  
  int Gl_texture_2D::get_height() const
  {
    return this->texture_data.desired_height;
  }
  
  int32_t Gl_texture_2D::get_depth() const
  {
    return 0;
  }
  
  int32_t Gl_texture_2D::get_texture_slot() const
  {
    return this->texture_data.desired_slot;
  }
  
  int Gl_texture_2D::get_bits_per_pixel() const
  {
    return this->bits_per_pixel;
  }
  
  int32_t Gl_texture_2D::get_samples() const
  {
    return this->texture_data.desired_samples;
  }
  
  void *Gl_texture_2D::get_data() const
  {
    return this->texture_data.data;
  }
  
  void Gl_texture_2D::set_id(int id_texture)
  {
    this->texture_id = id_texture;
  }
  
  void Gl_texture_2D::set_width(int width_)
  {
    this->texture_data.desired_width = width_;
  }
  
  void Gl_texture_2D::set_height(int height_)
  {
    this->texture_data.desired_height = height_;
  }
  
  void Gl_texture_2D::set_depth([[maybe_unused]] int height_)
  {
  }
  
  void Gl_texture_2D::set_texture_slot(int32_t texture_slot_)
  {
    this->texture_data.desired_slot = texture_slot_;
  }
  
  void Gl_texture_2D::set_bits_per_pixel(int bits_per_pixel_)
  {
    this->bits_per_pixel = bits_per_pixel_;
  }
  
  Gl_texture_2D &Gl_texture_2D::operator=(const Gl_texture_2D &other_texture)
  {
    if (this == &other_texture) return *this;
    if (other_texture.texture_id) return *this;
    this->texture_id = other_texture.texture_id;
    this->texture_data.internal_format = other_texture.texture_data.internal_format;
    this->gl_format = other_texture.gl_format;
    this->gl_internal_format = other_texture.gl_internal_format;
    this->gl_buffer_type = other_texture.gl_buffer_type;
    this->texture_data.desired_width = other_texture.texture_data.desired_width;
    this->texture_data.desired_height = other_texture.texture_data.desired_height;
    this->texture_data.desired_slot = other_texture.texture_data.desired_slot;
    this->bits_per_pixel = other_texture.bits_per_pixel;
    this->texture_data.desired_samples = other_texture.texture_data.desired_samples;
    this->texture_data.data = other_texture.texture_data.data;
    return *this;
  }
  
  Gl_texture_2D::operator bool() const
  {
    return texture_id != 255;
  }
  
  std::string Gl_texture_2D::to_string() const
  {
    char buffer[FILENAME_MAX * 4]{0};
    std::string opengl_internal_format, opengl_format, opengl_buffer_type;
    switch (this->texture_data.internal_format)
    {
      case Texture::Texture_internal_format_e::Red:
      {
        opengl_internal_format = "GL_RED";
        opengl_format = "GL_RED";
        opengl_buffer_type = "GL_UNSIGNED_BYTE";
        break;
      }
      case Texture::Texture_internal_format_e::Rgba8:
      {
        opengl_internal_format = "GL_RGBA8";
        opengl_format = "GL_RGBA";
        opengl_buffer_type = "GL_UNSIGNED_BYTE";
        break;
      }
      case Texture::Texture_internal_format_e::Rgba16:
      {
        opengl_internal_format = "GL_RGBA16";
        opengl_format = "GL_RGBA";
        opengl_buffer_type = "GL_2_BYTES";
        break;
      }
      case Texture::Texture_internal_format_e::Rgba32:
      {
        opengl_internal_format = "GL_RGBA32F";
        opengl_format = "GL_RGBA";
        opengl_buffer_type = "GL_FLOAT";
        break;
      }
      case Texture::Texture_internal_format_e::Depth_stencil:
      {
        opengl_internal_format = "GL_DEPTH24_STENCIL8";
        opengl_format = "GL_DEPTH_STENCIL";
        opengl_buffer_type = "GL_UNSIGNED_INT_24_8";
        break;
      }
      default:
      {
        opengl_internal_format = "Unknown";
        opengl_format = "Unknown";
        opengl_buffer_type = "Unknown";
        break;
      }
    }
    
    if (snprintf(buffer, sizeof(buffer), "[Texture 2D] :\n%55sID --> %d\n%55sHeight --> %d\n"
                                         "%55sWidth --> %d\n%55sOpenGL internal format --> %s\n"
                                         "%55sOpenGL format --> %s\n%55sOpenGL buffer type --> %s"
                                         "\n%55sSlot --> %d\n%55sSamples --> %d",
                 DEFAULT, this->texture_id, DEFAULT, this->texture_data.desired_height, DEFAULT,
                 this->texture_data.desired_width, DEFAULT,
                 opengl_internal_format.c_str(), DEFAULT, opengl_format.c_str(), DEFAULT,
                 opengl_buffer_type.c_str(), DEFAULT, this->texture_data.desired_slot, DEFAULT,
                 this->texture_data.desired_samples) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Gl texture 2D]!";
    }
    return buffer;
  }
  
  /********************* TEXTURE 3D ********************/
  
  Gl_texture_3D::Gl_texture_3D(const char *file_path)
  {
    this->texture_data.type = Texture_type_e::Texture_3D;
    this->texture_data.internal_format = Texture_internal_format_e::Rgba8;
    this->texture_data.desired_samples = 1;
    this->texture_data.desired_slot = 0;
    this->gl_format = convert_format_to_gl_format(this->texture_data.internal_format);
    this->gl_internal_format = convert_internal_format_to_gl_internal_format(this->texture_data.internal_format);
    this->gl_buffer_type = convert_buffer_type_to_gl_buffer_type(this->texture_data.internal_format);
    stbi_uc *image_buffer{};
    
    if (file_path)
    {
      
      // Load file texture in memory.
      stbi_set_flip_vertically_on_load(1);  // Invert y-axis for opengl matrix coordinates.
      image_buffer = stbi_load(file_path, &this->texture_data.desired_width, &this->texture_data.desired_height,
                               &this->bits_per_pixel,
                               4);  // 4 channels (RGBA).
      if (!image_buffer)
      {
        Wave::alert(WAVE_LOG_ERROR, "[Texture] : Could not load image from %s", file_path);
      }
    }
    
    this->texture_target = Gl_texture_3D::convert_type_to_api({this->texture_data.type,
                                                               this->texture_data.internal_format,
                                                               this->texture_data.desired_width,
                                                               this->texture_data.desired_height,
                                                               this->texture_data.desired_depth,
                                                               0,
                                                               1,
                                                               this->texture_data.data});
    // Initialize openGL texture buffers.
    CHECK_GL_CALL(glCreateTextures(this->texture_target, 1, &this->texture_id));
    
    this->set_data(&this->texture_data, nullptr);
    
    // Assign texture image to texture buffer.
    // Deallocate file texture from CPU since it's loaded onto the GPU.
    if (image_buffer) stbi_image_free(image_buffer);
  }
  
  Gl_texture_3D::Gl_texture_3D(const char *file_path, Texture_data_s data)
  {
    this->texture_data.type = data.type;
    this->texture_data.internal_format = data.internal_format;
    this->texture_data.desired_samples = data.desired_samples;
    this->texture_data.desired_slot = data.desired_slot;
    this->gl_format = convert_format_to_gl_format(this->texture_data.internal_format);
    this->gl_internal_format = convert_internal_format_to_gl_internal_format(this->texture_data.internal_format);
    this->gl_buffer_type = convert_buffer_type_to_gl_buffer_type(this->texture_data.internal_format);
    stbi_uc *image_buffer{};
    if (file_path)
    {
      
      // Load file texture in memory.
      stbi_set_flip_vertically_on_load(1);  // Invert y-axis for opengl matrix coordinates.
      image_buffer = stbi_load(file_path, &this->texture_data.desired_width, &this->texture_data.desired_height,
                               &this->bits_per_pixel,
                               4);  // 4 channels (RGBA).
      if (!image_buffer)
      {
        Wave::alert(WAVE_LOG_ERROR, "[Texture] : Could not load image from %s", file_path);
      }
    }
    
    this->texture_target = Gl_texture_3D::convert_type_to_api(data);
    // Initialize openGL texture buffers.
    CHECK_GL_CALL(glCreateTextures(this->texture_target, 1, &this->texture_id));
    
    this->set_data(&data, nullptr);
    
    // Assign texture image to texture buffer.
    // Deallocate file texture from CPU since it's loaded onto the GPU.
    if (image_buffer) stbi_image_free(image_buffer);
  }
  
  Gl_texture_3D::~Gl_texture_3D()
  {
    Gl_texture_3D::remove();
  }
  
  int32_t Gl_texture_3D::convert_type_to_api(Texture_data_s data_)
  {
    switch (data_.type)
    {
      case Texture_type_e::Texture_3D:
      {
        if (data_.desired_samples > 1) return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
        return GL_TEXTURE_3D;
      }
      case Texture_type_e::Cube_map: return GL_TEXTURE_CUBE_MAP;
      case Texture_type_e::Cube_map_array: return GL_TEXTURE_CUBE_MAP_ARRAY;
      default:
        Gl_renderer::gl_synchronous_error_callback(GL_INVALID_ENUM, "[GL Texture] --> Invalid texture type given!",
                                                   __FUNCTION__, __FILE__, __LINE__ - 12);
    }
    return 0;
  }
  
  void Gl_texture_3D::bind(int32_t slot_) const
  {
    if (*this)
    {
      CHECK_GL_CALL(glActiveTexture(
        slot_ == WAVE_VALUE_DONT_CARE ? GL_TEXTURE0 : GL_TEXTURE0 + slot_));  // Set our active texture slot.
      CHECK_GL_CALL(glBindTexture(this->texture_target, this->texture_id));
    }
  }
  
  void Gl_texture_3D::unbind() const
  {
    if (*this)
    {
      CHECK_GL_CALL(glActiveTexture(GL_TEXTURE0 + this->texture_data.desired_slot));
      CHECK_GL_CALL(glBindTexture(this->texture_target, 0));
    }
  }
  
  void Gl_texture_3D::remove()
  {
    if (*this)
    {
      unbind();
      glDeleteTextures(1, &this->texture_id);
    }
    this->texture_id = 255;
  }
  
  void Gl_texture_3D::set_data(const Texture_data_s data_array[6], uint32_t offset_array[3]) const
  {
    this->bind(this->texture_data.desired_slot);
    
    // Supply texture data.
    switch (this->texture_data.type)
    {
      case Texture_type_e::Texture_3D:
      {
        if (data_array && data_array->desired_samples > 1)
        {
          CHECK_GL_CALL(glTexImage3DMultisample(this->texture_target,
                                                data_array->desired_samples,
                                                this->gl_internal_format,
                                                data_array[0].desired_width,
                                                data_array[0].desired_height,
                                                data_array[0].desired_depth,
                                                GL_FALSE));
        } else if (data_array && offset_array)
        {
          CHECK_GL_CALL(glTexSubImage3D(this->texture_target,
                                        0,
                                        offset_array[0],
                                        offset_array[1],
                                        offset_array[2],
                                        data_array[0].desired_width,
                                        data_array[0].desired_height,
                                        data_array[0].desired_depth,
                                        this->gl_format,
                                        this->gl_buffer_type,
                                        data_array[0].data));
        } else if (data_array)
        {
          CHECK_GL_CALL(glTexImage3D(this->texture_target,
                                     0,
                                     this->gl_internal_format,
                                     data_array[0].desired_width,
                                     data_array[0].desired_height,
                                     data_array[0].desired_depth,
                                     0,
                                     this->gl_format,
                                     this->gl_buffer_type,
                                     data_array[0].data));
        }
        break;
      }
      case Texture_type_e::Cube_map:
      {
        for (int i = 0; i < 6; ++i)  // For each face in a cube.
        {
          if (data_array && offset_array)
          {
            CHECK_GL_CALL(glTexSubImage2D(this->texture_target,
                                          0,
                                          offset_array[0],
                                          offset_array[1],
                                          data_array[i].desired_width == WAVE_VALUE_DONT_CARE ? 0 :
                                          data_array[i].desired_width,
                                          data_array[i].desired_height == WAVE_VALUE_DONT_CARE ? 0 :
                                          data_array[i].desired_height,
                                          this->gl_format,
                                          this->gl_buffer_type,
                                          data_array[i].data));
          } else if (data_array)
          {
            CHECK_GL_CALL(glTexImage2D(this->texture_target,
                                       0,
                                       this->gl_internal_format,
                                       data_array[i].desired_width == WAVE_VALUE_DONT_CARE ? 0 :
                                       data_array[i].desired_width,
                                       data_array[i].desired_height == WAVE_VALUE_DONT_CARE ? 0 :
                                       data_array[i].desired_height,
                                       0,
                                       this->gl_format,
                                       this->gl_buffer_type,
                                       data_array[i].data));
          }
        }
        break;
      }
        //TODO Texture 2D array and Cube map array.
      default:
      {
        alert(WAVE_LOG_WARN, "[GL texture] --> Incorrect or undefined data type supplied"
                             " in function %s, on line %d!", __FUNCTION__, __LINE__);
        return;
      }
    }
  }
  
  Texture::Texture_type_e Gl_texture_3D::get_type()
  {
    return this->texture_data.type;
  }
  
  uint32_t &Gl_texture_3D::get_id()
  {
    return this->texture_id;
  }
  
  int Gl_texture_3D::get_width() const
  {
    return this->texture_data.desired_width;
  }
  
  int Gl_texture_3D::get_height() const
  {
    return this->texture_data.desired_height;
  }
  
  int32_t Gl_texture_3D::get_depth() const
  {
    return this->texture_data.desired_depth;
  }
  
  int32_t Gl_texture_3D::get_texture_slot() const
  {
    return this->texture_data.desired_slot;
  }
  
  int Gl_texture_3D::get_bits_per_pixel() const
  {
    return this->bits_per_pixel;
  }
  
  int32_t Gl_texture_3D::get_samples() const
  {
    return this->texture_data.desired_samples;
  }
  
  void *Gl_texture_3D::get_data() const
  {
    return this->texture_data.data;
  }
  
  void Gl_texture_3D::set_id(int id_texture)
  {
    this->texture_id = id_texture;
  }
  
  void Gl_texture_3D::set_width(int width_)
  {
    this->texture_data.desired_width = width_;
  }
  
  void Gl_texture_3D::set_height(int height_)
  {
    this->texture_data.desired_height = height_;
  }
  
  void Gl_texture_3D::set_depth(int depth_)
  {
    this->texture_data.desired_depth = depth_;
  }
  
  void Gl_texture_3D::set_texture_slot(int32_t texture_slot_)
  {
    this->texture_data.desired_slot = texture_slot_;
  }
  
  void Gl_texture_3D::set_bits_per_pixel(int bits_per_pixel_)
  {
    this->bits_per_pixel = bits_per_pixel_;
  }
  
  Gl_texture_3D &Gl_texture_3D::operator=(const Gl_texture_3D &other_texture)
  {
    if (this == &other_texture) return *this;
    if (other_texture.texture_id) return *this;
    this->texture_id = other_texture.texture_id;
    this->texture_data.internal_format = other_texture.texture_data.internal_format;
    this->gl_format = other_texture.gl_format;
    this->gl_internal_format = other_texture.gl_internal_format;
    this->gl_buffer_type = other_texture.gl_buffer_type;
    this->texture_data.desired_width = other_texture.texture_data.desired_width;
    this->texture_data.desired_height = other_texture.texture_data.desired_height;
    this->texture_data.desired_slot = other_texture.texture_data.desired_slot;
    this->bits_per_pixel = other_texture.bits_per_pixel;
    return *this;
  }
  
  Gl_texture_3D::operator bool() const
  {
    return texture_id != 255;
  }
  
  std::string Gl_texture_3D::to_string() const
  {
    char buffer[FILENAME_MAX * 4]{0};
    std::string opengl_internal_format, opengl_format, opengl_buffer_type;
    switch (this->texture_data.internal_format)
    {
      case Texture::Texture_internal_format_e::Red:
      {
        opengl_internal_format = "GL_RED";
        opengl_format = "GL_RED";
        opengl_buffer_type = "GL_UNSIGNED_BYTE";
        break;
      }
      case Texture::Texture_internal_format_e::Rgba8:
      {
        opengl_internal_format = "GL_RGBA8";
        opengl_format = "GL_RGBA";
        opengl_buffer_type = "GL_UNSIGNED_BYTE";
        break;
      }
      case Texture::Texture_internal_format_e::Rgba16:
      {
        opengl_internal_format = "GL_RGBA16";
        opengl_format = "GL_RGBA";
        opengl_buffer_type = "GL_2_BYTES";
        break;
      }
      case Texture::Texture_internal_format_e::Rgba32:
      {
        opengl_internal_format = "GL_RGBA32F";
        opengl_format = "GL_RGBA";
        opengl_buffer_type = "GL_FLOAT";
        break;
      }
      case Texture::Texture_internal_format_e::Depth_stencil:
      {
        opengl_internal_format = "GL_DEPTH24_STENCIL8";
        opengl_format = "GL_DEPTH_STENCIL";
        opengl_buffer_type = "GL_UNSIGNED_INT_24_8";
        break;
      }
      default:
      {
        opengl_internal_format = "Unknown";
        opengl_format = "Unknown";
        opengl_buffer_type = "Unknown";
        break;
      }
    }
    
    if (snprintf(buffer, sizeof(buffer), "[Texture 3D] :\n%55sID --> %d\n%55sHeight --> %d\n"
                                         "%55sWidth --> %d\n%55sOpenGL internal format --> %s\n"
                                         "%55sOpenGL format --> %s\n%55sOpenGL buffer type --> %s"
                                         "\n%55sSlot --> %d\n%55sSamples --> %d",
                 DEFAULT, this->texture_id, DEFAULT, this->texture_data.desired_height, DEFAULT,
                 this->texture_data.desired_width, DEFAULT,
                 opengl_internal_format.c_str(), DEFAULT, opengl_format.c_str(), DEFAULT,
                 opengl_buffer_type.c_str(), DEFAULT, this->texture_data.desired_slot, DEFAULT,
                 this->texture_data.desired_samples) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Gl texture 3D]!";
    }
    return buffer;
  }
} // Wave