//
// Created by nami on 12/05/23.
//

#include <Renderer/gl_texture.h>
#include <Renderer/gl_renderer.h>

#define STB_IMAGE_IMPLEMENTATION

#include <Dependencies/Stb/stb_image.h>

namespace Wave
{
  
  /********************* TEXTURE 2D ********************/
  
  Gl_texture_2D::Gl_texture_2D(const char *file_path, Texture_data_s data)
  {
    this->texture_type = data.type;
    this->samples = data.samples;
    this->texture_slot = data.slot;
    stbi_uc *image_buffer;
    if (file_path)
    {
      
      // Load file texture in memory.
      stbi_set_flip_vertically_on_load(1);  // Invert y-axis for opengl matrix coordinates.
      image_buffer = stbi_load(file_path, &this->width, &this->height, &this->bits_per_pixel,
                               4);  // 4 channels (RGBA).
      if (!image_buffer)
      {
        Wave::alert(WAVE_ERROR, "[Texture] : Could not load image from %s", file_path);
      }
    }
    
    this->internal_type = Gl_texture_2D::convert_to_api(data);
    // Initialize openGL texture buffers.
    CHECK_GL_CALL(glCreateTextures(this->internal_type, 1, &this->texture_id));
    
    this->set_data((const void **) (&image_buffer));
    
    // Assign texture image to texture buffer.
    // Deallocate file texture from CPU since it's loaded onto the GPU.
    if (image_buffer) stbi_image_free(image_buffer);
  }
  
  Gl_texture_2D::Gl_texture_2D(const char *file_path, FT_Face face)
  {
    // Generating font textures.
    if (strcmp(&file_path[strlen(file_path) - 3], "ttf") == 0)
    {
      CHECK_GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &this->texture_id));
      CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D, this->texture_id));
      
      // set texture options
      CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
      CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
      CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
      CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
      
      CHECK_GL_CALL(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        static_cast<int32_t>(face->glyph->bitmap.width),
        static_cast<int32_t>(face->glyph->bitmap.rows),
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer));
    }
  }
  
  Gl_texture_2D::~Gl_texture_2D()
  {
    Gl_texture_2D::remove();
  }
  
  int32_t Gl_texture_2D::convert_to_api(Texture_data_s data)
  {
    switch (data.type)
    {
      case Texture_type::Texture_2D:
      {
        if (data.samples > 1) return GL_TEXTURE_2D_MULTISAMPLE;
        return GL_TEXTURE_2D;
      }
      case Texture_type::Texture_3D: return GL_TEXTURE_3D;
      case Texture_type::Texture_2D_Ms_array: return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
      default:
        Gl_renderer::gl_synchronous_error_callback(GL_INVALID_ENUM, "[GL Texture] --> Invalid texture type given!",
                                                   __FUNCTION__, __FILE__, __LINE__ - 12);
    }
    return 0;
  }
  
  void Gl_texture_2D::bind(uint32_t slot_) const
  {
    if (*this)
    {
      CHECK_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot_));  // Set our active texture slot.
      CHECK_GL_CALL(glBindTexture(this->internal_type, this->texture_id));
    }
  }
  
  void Gl_texture_2D::unbind() const
  {
    if (*this)
    {
      CHECK_GL_CALL(glBindTexture(this->internal_type, 0));
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
  
  void Gl_texture_2D::set_data(const void **data) const
  {
    this->bind(this->texture_slot);
    /* Initialize flags for texture buffers. */
    
    // Min surface to cover.
    glTexParameteri(this->internal_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Max surface to cover and extend.
    glTexParameteri(this->internal_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Normalize x-axis of texture.
    glTexParameteri(this->internal_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // Normalize y-axis of texture.
    glTexParameteri(this->internal_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Normalize z-axis of texture.
    glTexParameteri(this->internal_type, GL_TEXTURE_WRAP_R, GL_REPEAT);
    
    // Supply texture data.
    if (!data)
    {
      alert(WAVE_WARN, "[GL texture] --> Incorrect or undefined data type supplied"
                       " in function %s, on line %d!", __FUNCTION__, __LINE__);
      return;
    }
    switch (this->texture_type)
    {
      case Texture_type::Texture_2D:
      {
        if (this->samples > 1)
        {
          CHECK_GL_CALL(glTexImage2DMultisample(this->internal_type, this->samples, GL_RGBA8, this->width, this->height,
                                                GL_FALSE));
        } else
        {
          CHECK_GL_CALL(glTexImage2D(this->internal_type, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA,
                                     GL_UNSIGNED_BYTE, data[0]));
        }
        break;
      }
      default: CHECK_GL_CALL(glTexImage2D(this->internal_type, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA,
                                          GL_UNSIGNED_BYTE, data[0]));
    }
  }
  
  uint32_t Gl_texture_2D::get_id() const
  {
    return this->texture_id;
  }
  
  int Gl_texture_2D::get_width() const
  {
    return this->width;
  }
  
  int Gl_texture_2D::get_height() const
  {
    return this->height;
  }
  
  int32_t Gl_texture_2D::get_depth() const
  {
    return 0;
  }
  
  uint32_t Gl_texture_2D::get_texture_slot() const
  {
    return this->texture_slot;
  }
  
  int Gl_texture_2D::get_bits_per_pixel() const
  {
    return this->bits_per_pixel;
  }
  
  void Gl_texture_2D::set_id(int id_texture)
  {
    this->texture_id = id_texture;
  }
  
  void Gl_texture_2D::set_width(int width_)
  {
    this->width = width_;
  }
  
  void Gl_texture_2D::set_height(int height_)
  {
    this->height = height_;
  }
  
  void Gl_texture_2D::set_depth([[maybe_unused]] int height_)
  {
  }
  
  void Gl_texture_2D::set_texture_slot(uint32_t texture_slot_)
  {
    this->texture_slot = texture_slot_;
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
    this->internal_type = other_texture.internal_type;
    this->width = other_texture.width;
    this->height = other_texture.height;
    this->texture_slot = other_texture.texture_slot;
    this->bits_per_pixel = other_texture.bits_per_pixel;
    this->samples = other_texture.samples;
    return *this;
  }
  
  Gl_texture_2D::operator bool() const
  {
    return texture_id != 255;
  }
  
  std::string Gl_texture_2D::to_string() const
  {
    char buffer[FILENAME_MAX * 4]{0};
    if (snprintf(buffer, sizeof(buffer), "[Texture 2D] :\n%55sID --> %d\n%55sHeight --> %d\n"
                                         "%55sWidth --> %d\n%55sBits per pixel (bbp) --> %u",
                 DEFAULT, this->texture_id, DEFAULT, this->height, DEFAULT, this->width, DEFAULT,
                 this->bits_per_pixel) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Texture]!";
    }
    return buffer;
  }
  
  /********************* TEXTURE 3D ********************/
  
  Gl_texture_3D::Gl_texture_3D(const char *file_path, Texture_data_s data)
  {
    this->texture_type = data.type;
    this->samples = data.samples;
    this->texture_slot = data.slot;
    stbi_uc *image_buffer;
    if (file_path)
    {
      
      // Load file texture in memory.
      stbi_set_flip_vertically_on_load(1);  // Invert y-axis for opengl matrix coordinates.
      image_buffer = stbi_load(file_path, &this->width, &this->height, &this->bits_per_pixel,
                               4);  // 4 channels (RGBA).
      if (!image_buffer)
      {
        Wave::alert(WAVE_ERROR, "[Texture] : Could not load image from %s", file_path);
      }
    }
    
    this->internal_type = Gl_texture_3D::convert_to_api(data);
    // Initialize openGL texture buffers.
    CHECK_GL_CALL(glCreateTextures(this->internal_type, 1, &this->texture_id));
    
    this->set_data((const void **) (&image_buffer));
    
    // Assign texture image to texture buffer.
    // Deallocate file texture from CPU since it's loaded onto the GPU.
    if (image_buffer) stbi_image_free(image_buffer);
  }
  
  Gl_texture_3D::~Gl_texture_3D()
  {
    Gl_texture_3D::remove();
  }
  
  int32_t Gl_texture_3D::convert_to_api(Texture_data_s data)
  {
    switch (data.type)
    {
      case Texture_type::Texture_3D:
      {
        if (data.samples > 1) return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
        return GL_TEXTURE_3D;
      }
      case Texture_type::Cube_map: return GL_TEXTURE_CUBE_MAP;
      case Texture_type::Cube_map_array: return GL_TEXTURE_CUBE_MAP_ARRAY;
      default:
        Gl_renderer::gl_synchronous_error_callback(GL_INVALID_ENUM, "[GL Texture] --> Invalid texture type given!",
                                                   __FUNCTION__, __FILE__, __LINE__ - 12);
    }
    return 0;
  }
  
  void Gl_texture_3D::bind(uint32_t slot_) const
  {
    if (*this)
    {
      CHECK_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot_));  // Set our active texture slot.
      CHECK_GL_CALL(glBindTexture(this->internal_type, this->texture_id));
    }
  }
  
  void Gl_texture_3D::unbind() const
  {
    if (*this)
    {
      CHECK_GL_CALL(glBindTexture(this->internal_type, 0));
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
  
  void Gl_texture_3D::set_data(const void **data) const
  {
    this->bind(this->texture_slot);
    /* Initialize flags for texture buffers. */
    
    // Min surface to cover.
    glTexParameteri(this->internal_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Max surface to cover and extend.
    glTexParameteri(this->internal_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Normalize x-axis of texture.
    glTexParameteri(this->internal_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // Normalize y-axis of texture.
    glTexParameteri(this->internal_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Normalize z-axis of texture.
    glTexParameteri(this->internal_type, GL_TEXTURE_WRAP_R, GL_REPEAT);
    
    // Supply texture data.
    if (!data)
    {
      alert(WAVE_WARN, "[GL texture] --> Incorrect or undefined data type supplied"
                       " in function %s, on line %d!", __FUNCTION__, __LINE__);
      return;
    }
    switch (this->texture_type)
    {
      case Texture_type::Texture_3D:
      {
        if (this->samples > 1)
        {
          CHECK_GL_CALL(glTexImage3DMultisample(this->internal_type, this->samples, GL_RGBA8, this->width, this->height,
                                                this->depth, GL_FALSE));
        }
        CHECK_GL_CALL(glTexImage3D(this->internal_type, 0, GL_RGBA8, this->width, this->height,
                                   this->depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[0]));
        break;
      }
      case Texture_type::Cube_map:
      {
        for (int i = 0; i < 6; ++i)  // For each face in a cube.
        {
          CHECK_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, this->width, this->height,
                                     0, GL_RGBA, GL_UNSIGNED_BYTE, &data[i]));
        }
        break;
      }
        //TODO Texture 2D array and Cube map array.
      default: break;
    }
  }
  
  uint32_t Gl_texture_3D::get_id() const
  {
    return this->texture_id;
  }
  
  int Gl_texture_3D::get_width() const
  {
    return this->width;
  }
  
  int Gl_texture_3D::get_height() const
  {
    return this->height;
  }
  
  int32_t Gl_texture_3D::get_depth() const
  {
    return this->depth;
  }
  
  uint32_t Gl_texture_3D::get_texture_slot() const
  {
    return this->texture_slot;
  }
  
  int Gl_texture_3D::get_bits_per_pixel() const
  {
    return this->bits_per_pixel;
  }
  
  void Gl_texture_3D::set_id(int id_texture)
  {
    this->texture_id = id_texture;
  }
  
  void Gl_texture_3D::set_width(int width_)
  {
    this->width = width_;
  }
  
  void Gl_texture_3D::set_height(int height_)
  {
    this->height = height_;
  }
  
  void Gl_texture_3D::set_depth(int depth_)
  {
    this->depth = depth_;
  }
  
  void Gl_texture_3D::set_texture_slot(uint32_t texture_slot_)
  {
    this->texture_slot = texture_slot_;
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
    this->width = other_texture.width;
    this->height = other_texture.height;
    this->texture_slot = other_texture.texture_slot;
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
    if (snprintf(buffer, sizeof(buffer), "[Texture 3D] :\n%55sID --> %d\n%55sHeight --> %d\n"
                                         "%55sWidth --> %d\n%55sBits per pixel (bbp) --> %u",
                 DEFAULT, this->texture_id, DEFAULT, this->height, DEFAULT, this->width, DEFAULT,
                 this->bits_per_pixel) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Texture]!";
    }
    return buffer;
  }
} // Wave