//
// Created by nami on 12/05/23.
//

#pragma once

#include <Renderer/texture.h>

namespace Wave
{
  /********************* TEXTURE 2D ********************/
  
  class Gl_texture_2D : public Texture_2D
  {
    public:
    Gl_texture_2D(const char *file_path, Texture_data_s data);
    explicit Gl_texture_2D(const char *file_path, FT_Face face);
    ~Gl_texture_2D() override;
    
    [[nodiscard]] int32_t convert_to_api(Texture_data_s data) override;
    [[nodiscard]] uint32_t get_id() const override;
    [[nodiscard]] int32_t get_width() const override;
    [[nodiscard]] int32_t get_height() const override;
    [[nodiscard]] int32_t get_depth() const override;
    [[nodiscard]] uint32_t get_texture_slot() const override;
    [[nodiscard]] int32_t get_bits_per_pixel() const override;
    
    INTERFACE_PRINT
    
    void set_id(int id_texture) override;
    void set_width(int width_) override;
    void set_height(int height_) override;
    void set_depth(int height_) override;
    void set_texture_slot(uint32_t texture_slot_) override;
    void set_bits_per_pixel(int bits_per_pixel) override;
    
    void bind(uint32_t slot_) const override;
    void unbind() const override;
    void remove() override;
    void set_data(const void **data) const override;
    
    
    explicit operator bool() const override;
    Gl_texture_2D &operator=(const Gl_texture_2D &other_texture);
    private:
    uint32_t texture_id = 255;
    Texture_type texture_type = Texture_type::Texture_2D;
    int32_t internal_type = 0;
    int32_t width = 0;
    int32_t height = 0;
    uint32_t texture_slot = 0;
    int32_t bits_per_pixel = 0;
    int32_t samples = 1;
  };
  
  /********************* TEXTURE 3D ********************/
  
  class Gl_texture_3D : public Texture_3D
  {
    public:
    Gl_texture_3D(const char *file_path, Texture_data_s data);
    ~Gl_texture_3D() override;
    
    [[nodiscard]] int32_t convert_to_api(Texture_data_s data) override;
    [[nodiscard]] uint32_t get_id() const override;
    [[nodiscard]] int32_t get_width() const override;
    [[nodiscard]] int32_t get_height() const override;
    [[nodiscard]] int32_t get_depth() const override;
    [[nodiscard]] uint32_t get_texture_slot() const override;
    [[nodiscard]] int32_t get_bits_per_pixel() const override;
    
    INTERFACE_PRINT
    
    void set_id(int id_texture) override;
    void set_width(int width_) override;
    void set_height(int height_) override;
    void set_depth(int depth_) override;
    void set_texture_slot(uint32_t texture_slot_) override;
    void set_bits_per_pixel(int bits_per_pixel) override;
    
    void bind(uint32_t slot_) const override;
    void unbind() const override;
    void remove() override;
    void set_data(const void **data) const override;
    
    
    explicit operator bool() const override;
    Gl_texture_3D &operator=(const Gl_texture_3D &other_texture);
    private:
    uint32_t texture_id = 255;
    Texture_type texture_type = Texture_type::Texture_3D;
    int32_t internal_type = 0;
    int32_t width = 0;
    int32_t height = 0;
    int32_t depth = 0;
    uint32_t texture_slot = 0;
    int32_t bits_per_pixel = 0;
    int32_t samples = 1;
  };
} // Wave
