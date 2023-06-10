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
    Gl_texture_2D() = default;
    explicit Gl_texture_2D(const char *file_path_);
    Gl_texture_2D(const char *file_path_, Texture_data_s texture_data_);
    
    ~Gl_texture_2D() override;
    [[nodiscard]] int32_t convert_type_to_api(Texture_data_s texture_data_) override;
    [[nodiscard]] uint32_t &get_id() override;
    [[nodiscard]] Texture_type_e get_type() override;
    [[nodiscard]] float get_width() const override;
    [[nodiscard]] float get_height() const override;
    [[nodiscard]] float get_depth() const override;
    [[nodiscard]] int32_t get_texture_slot() const override;
    [[nodiscard]] int32_t get_bits_per_pixel() const override;
    [[nodiscard]] int32_t get_samples() const override;
    
    INTERFACE_SENDABLE
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] void *get_data() const override;
    void set_id(int id_texture) override;
    void set_width(float width_) override;
    void set_height(float height_) override;
    void set_depth(float height_) override;
    void set_texture_slot(int32_t texture_slot_) override;
    void set_bits_per_pixel(int bits_per_pixel) override;
    
    void set_data(const Texture_data_s *data_array, uint32_t offset_array[2]) const override;
    void bind(int32_t slot_) override;
    void unbind() const override;
    
    
    explicit operator bool() const override;
    Gl_texture_2D &operator=(const Gl_texture_2D &other_texture);
    private:
    const char *file_path = nullptr;
    uint32_t texture_id = 0;
    Texture_data_s texture_data{Texture_type_e::Texture_2D,
                                Texture_internal_format_e::Rgba8,
                                0, 0, 0, 0, 1,
                                nullptr};
    int32_t texture_target = 0;
    int32_t gl_format = 0;
    int32_t gl_internal_format = 0;
    int32_t gl_buffer_type = 0;
    int32_t bits_per_pixel = 0;
  };
  
  /********************* TEXTURE 3D ********************/
  
  class Gl_texture_3D : public Texture_3D
  {
    public:
    Gl_texture_3D() = default;
    explicit Gl_texture_3D(const char *file_path);
    Gl_texture_3D(const char *file_path, Texture_data_s data);
    ~Gl_texture_3D() override;
    
    [[nodiscard]] int32_t convert_type_to_api(Texture_data_s data) override;
    [[nodiscard]] uint32_t &get_id() override;
    [[nodiscard]] Texture_type_e get_type() override;
    [[nodiscard]] float get_width() const override;
    [[nodiscard]] float get_height() const override;
    [[nodiscard]] float get_depth() const override;
    [[nodiscard]] int32_t get_texture_slot() const override;
    [[nodiscard]] int32_t get_bits_per_pixel() const override;
    [[nodiscard]] int32_t get_samples() const override;
    [[nodiscard]] void *get_data() const override;
    
    INTERFACE_SENDABLE
    
    INTERFACE_PRINTABLE
    
    void set_id(int id_texture) override;
    void set_width(float width_) override;
    void set_height(float height_) override;
    void set_depth(float depth_) override;
    void set_texture_slot(int32_t texture_slot_) override;
    void set_bits_per_pixel(int bits_per_pixel) override;
    
    void bind(int32_t slot_) override;
    void unbind() const override;
    void set_data(const Texture_data_s data_array[6], uint32_t offset_array[3]) const override;
    
    
    explicit operator bool() const override;
    Gl_texture_3D &operator=(const Gl_texture_3D &other_texture);
    private:
    const char *file_path = nullptr;
    uint32_t texture_id = 0;
    Texture_data_s texture_data{Texture_type_e::Texture_3D,
                                Texture_internal_format_e::Rgba8,
                                0, 0, 0, 0, 1,
                                nullptr};
    int32_t texture_target = 0;
    int32_t gl_format = 0;
    int32_t gl_internal_format = 0;
    int32_t gl_buffer_type = 0;
    int32_t bits_per_pixel = 0;
  };
  
  [[nodiscard]] int32_t convert_format_to_gl_format(Texture::Texture_internal_format_e format);
  [[nodiscard]] int32_t convert_internal_format_to_gl_internal_format(Texture::Texture_internal_format_e format);
  [[nodiscard]] int32_t convert_buffer_type_to_gl_buffer_type(Texture::Texture_internal_format_e format);
} // Wave
