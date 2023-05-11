//
// Created by nami on 5/26/22.
//

#pragma once

#include <wave_pch.h>
#include <Core/interfaces.h>
#include <Utilities/logger.h>
#include <freetype/freetype.h>

namespace Wave
{
  
  class Texture : public Printable
  {
    public:
    Texture() = default;
    explicit Texture(const char *file_path);
    Texture(const char *file_path, FT_Face face);
    ~Texture() override = default;
    
    void bind_to_context();
    
    INTERFACE_PRINT
    
    [[nodiscard]] uint32_t get_id() const;
    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;
    [[nodiscard]] int get_bits_per_pixel() const;
    [[nodiscard]] unsigned char *get_local_buffer() const;
    
    void set_id(int id_texture);
    void set_local_buffer(unsigned char *local_buffer);
    void set_width(int width);
    void set_height(int height);
    void set_bits_per_pixel(int bits_per_pixel);
    
    void bind(uint32_t slot_) const;
    void unbind() const;
    void remove() const;
    
    Texture &operator=(const Texture &other_texture);
    private:
    uint32_t texture_id = 255;
    unsigned char *local_buffer = nullptr;
    int width = 0;
    int height = 0;
    int bits_per_pixel = 0;
  };
}