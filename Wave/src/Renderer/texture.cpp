//
// Created by nami on 5/26/22.
//


#include <Renderer/gl_renderer.h>

#define STB_IMAGE_IMPLEMENTATION

#include <Dependencies/Stb/stb_image.h>
#include "Renderer/texture.h"


namespace Wave
{
  
  Texture::Texture(const char *file_path) : texture_id(0), width(0), height(0), bits_per_pixel(0)
  {
    // Load texture in memory.
    stbi_set_flip_vertically_on_load(1);  // Invert y-axis for opengl matrix coordinates.
    this->local_buffer = stbi_load(file_path, &this->width, &this->height, &this->bits_per_pixel,
                                   4);  // 4 channels (RGBA).
    if (!this->local_buffer)
    {
      Wave::alert(WAVE_ERROR, "[TEXTURE ERROR] : Could not load image from %s", file_path);
    }
    
    bind_to_context();
    
    // Deallocate texture from CPU since it's loaded onto the GPU.
    if (this->local_buffer) stbi_image_free(this->local_buffer);
  }
  
  Texture::Texture(const char *file_path, FT_Face face)
  {
    // Generating font textures.
    if (strcmp(&file_path[strlen(file_path) - 3], "ttf") == 0)
    {
      glGenTextures(1, &this->texture_id);
      glBindTexture(GL_TEXTURE_2D, this->texture_id);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        static_cast<int32_t>(face->glyph->bitmap.width),
        static_cast<int32_t>(face->glyph->bitmap.rows),
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer);
      
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
  }
  
  Texture::~Texture()
  {
    remove();
  }
  
  void Texture::bind_to_context()
  {
    // Initialize openGL texture buffers.
    CHECK_GL_CALL(glGenTextures(1, &this->texture_id));
    
    // Initialize flags for texture buffers.
    CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D, this->texture_id));
    CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));  // Min surface to cover.
    CHECK_GL_CALL(
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));  // Max surface to cover and extend.
    CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));  // Normalize x-axis of texture.
    CHECK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));  // Normalize y-axis of texture.
    
    // Assign texture image to texture buffer.
    CHECK_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0,
                               GL_RGBA, GL_UNSIGNED_BYTE, this->local_buffer));
  }
  
  void Texture::bind(uint32_t slot_) const
  {
    if (this->texture_id != 255)
    {
      CHECK_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot_));  // Set our active texture slot.
      CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D, this->texture_id));
    }
  }
  
  void Texture::unbind() const
  {
    if (this->texture_id != 255)
    {
      CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }
  }
  
  void Texture::remove() const
  {
    unbind();
    glDeleteTextures(1, &this->texture_id);
  }
  
  uint32_t Texture::get_id() const
  {
    return this->texture_id;
  }
  
  uint8_t *Texture::get_local_buffer() const
  {
    return this->local_buffer;
  }
  
  int Texture::get_width() const
  {
    return this->width;
  }
  
  int Texture::get_height() const
  {
    return this->height;
  }
  
  int Texture::get_bits_per_pixel() const
  {
    return this->bits_per_pixel;
  }
  
  void Texture::set_id(int id_texture)
  {
    this->texture_id = id_texture;
  }
  
  void Texture::set_local_buffer(uint8_t *local_buffer_)
  {
    this->local_buffer = local_buffer_;
  }
  
  void Texture::set_width(int width_)
  {
    this->width = width_;
  }
  
  void Texture::set_height(int height_)
  {
    this->height = height_;
  }
  
  void Texture::set_bits_per_pixel(int bits_per_pixel_)
  {
    this->bits_per_pixel = bits_per_pixel_;
  }
  
  Texture &Texture::operator=(const Texture &other_texture)
  {
    if (this == &other_texture) return *this;
    if (other_texture.texture_id == 255) return *this;
    this->texture_id = other_texture.texture_id;
    this->local_buffer = other_texture.local_buffer;
    this->width = other_texture.width;
    this->height = other_texture.height;
    this->bits_per_pixel = other_texture.bits_per_pixel;
    return *this;
  }
  
  std::string Texture::to_string() const
  {
    char buffer[FILENAME_MAX * 4]{0};
    if (snprintf(buffer, sizeof(buffer), "[TEXTURE] :\n%55sID --> %d\n%55sLocal buffer --> %hhu\n"
                                         "%55sHeight --> %d\n%55sWidth --> %d\n%55sBits per pixel (bbp) --> %u",
                 DEFAULT, this->texture_id, DEFAULT, *this->local_buffer,
                 DEFAULT, this->height, DEFAULT, this->width, DEFAULT, this->bits_per_pixel) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Texture]!";
    }
    return buffer;
  }
}