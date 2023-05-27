//
// Created by nami on 5/26/22.
//

#pragma once

#include <Core/interfaces.h>
#include <Utilities/logger.h>
#include <freetype/freetype.h>

namespace Wave
{
  
  enum class Texture_type
  {
    None,
    Texture_2D,
    Texture_2D_Array,
    Texture_2D_Ms_array,
    Texture_3D,
    Cube_map,
    Cube_map_array
  };
  
  typedef struct Texture_data_s
  {
    Texture_type type = Texture_type::None;
    int32_t width = 0;
    int32_t height = 0;
    int32_t depth = 0;
    int32_t slot = 0;
    int32_t samples = 1;
    void *data = nullptr;
  } Texture_data_s;
  
  class Texture : public Printable
  {
    public:
    static std::shared_ptr<Texture> create(const char *file_path, Texture_data_s texture_data);
    ~Texture() override = default;
    
    [[nodiscard]] virtual int32_t convert_to_api(Texture_data_s data) = 0;
    [[nodiscard]] virtual uint32_t &get_id() = 0;
    [[nodiscard]] virtual int32_t get_width() const = 0;
    [[nodiscard]] virtual int32_t get_height() const = 0;
    [[nodiscard]] virtual int32_t get_depth() const = 0;
    [[nodiscard]] virtual int32_t get_texture_slot() const = 0;
    [[nodiscard]] virtual int32_t get_bits_per_pixel() const = 0;
    [[nodiscard]] virtual void *get_data() const = 0;
    
    virtual void set_id(int32_t id_texture) = 0;
    virtual void set_width(int32_t width_) = 0;
    virtual void set_height(int32_t height_) = 0;
    virtual void set_depth(int32_t height_) = 0;
    virtual void set_texture_slot(int32_t texture_slot_) = 0;
    virtual void set_bits_per_pixel(int32_t bits_per_pixel) = 0;
    
    virtual void bind(int32_t slot_) const = 0;
    virtual void unbind() const = 0;
    virtual void remove() = 0;
    virtual void set_data(const void **data_array) const = 0;
    
    virtual explicit operator bool() const = 0;
  };
  
  /********************* TEXTURE 2D ********************/
  
  class Texture_2D : public Texture
  {
    public:
    static std::shared_ptr<Texture_2D> create(const char *file_path, Texture_data_s texture_data);
    ~Texture_2D() override = default;
  };
  
  /********************* TEXTURE 3D ********************/
  
  class Texture_3D : public Texture
  {
    public:
    static std::shared_ptr<Texture_3D> create(const char *file_path, Texture_data_s texture_data);
    ~Texture_3D() override = default;
  };
}