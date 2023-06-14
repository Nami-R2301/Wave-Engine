//
// Created by nami on 5/26/22.
//

#pragma once

#include <Core/interfaces.h>
#include <Utilities/logger.h>
#include <freetype/freetype.h>

namespace Wave
{
  
  class Texture : public Printable, public Sendable
  {
    public:
    enum class Texture_type_e
    {
      None,
      Texture_2D,
      Texture_2D_Array,
      Texture_2D_Ms_array,
      Texture_3D,
      Cube_map,
      Cube_map_array
    };
    
    enum class Texture_internal_format_e
    {
      None,
      Red,
      Rgba8,
      Rgba16,
      Rgba32,
      
      // Framebuffers.
      Color_attachment,
      Depth_attachment,
      Stencil_attachment,
      Depth_stencil_attachment
    };
    
    typedef struct Texture_data_s
    {
      Texture_type_e type = Texture_type_e::None;
      Texture_internal_format_e internal_format = Texture_internal_format_e::None;
      float desired_width = 0;
      float desired_height = 0;
      float desired_depth = 0;
      int32_t desired_slot = 0;
      int32_t desired_samples = 1;
      void *data = nullptr;
      int32_t framebuffer_attachment_index = 0;
    } Texture_data_s;
    public:
    static std::shared_ptr<Texture> create(const char *file_path_);
    static std::shared_ptr<Texture> create(const char *file_path_, Texture_data_s texture_data_);
    ~Texture() override = default;
    
    [[nodiscard]] virtual int32_t convert_type_to_api(Texture_data_s texture_data_) = 0;
    [[nodiscard]] virtual uint32_t &get_id() = 0;
    [[nodiscard]] virtual Texture_type_e get_type() = 0;
    [[nodiscard]] virtual float get_width() const = 0;
    [[nodiscard]] virtual float get_height() const = 0;
    [[nodiscard]] virtual float get_depth() const = 0;
    [[nodiscard]] virtual int32_t get_texture_slot() const = 0;
    [[nodiscard]] virtual int32_t get_bits_per_pixel() const = 0;
    [[nodiscard]] virtual int32_t get_samples() const = 0;
    [[nodiscard]] virtual void *get_data() const = 0;
    
    virtual void set_id(int32_t id_texture) = 0;
    virtual void set_width(float width_) = 0;
    virtual void set_height(float height_) = 0;
    virtual void set_depth(float height_) = 0;
    virtual void set_texture_slot(int32_t texture_slot_) = 0;
    virtual void set_bits_per_pixel(int32_t bits_per_pixel) = 0;
    
    virtual void bind(int32_t slot_) = 0;
    virtual void unbind() const = 0;
    virtual void set_data(const Texture_data_s *data_array, uint32_t *offset_array) const = 0;
    
    virtual explicit operator bool() const = 0;
    protected:
    bool sent = false;
  };
  
  /********************* TEXTURE 2D ********************/
  
  class Texture_2D : public Texture
  {
    public:
    static std::shared_ptr<Texture_2D> create(const char *file_path_);
    static std::shared_ptr<Texture_2D> create(const char *file_path_, Texture_data_s texture_data_);
    ~Texture_2D() override = default;
  };
  
  /********************* TEXTURE 3D ********************/
  
  class Texture_3D : public Texture
  {
    public:
    static std::shared_ptr<Texture_3D> create(const char *file_path_);
    static std::shared_ptr<Texture_3D> create(const char *file_path_, Texture_data_s texture_data_);
    ~Texture_3D() override = default;
  };
}