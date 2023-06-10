//
// Created by nami on 07/06/23.
//

#pragma once

#include <Utilities/logger.h>
#include <Core/interfaces.h>
#include <Core/core_macros.h>
#include <Renderer/color.h>
#include <Renderer/shader.h>

namespace Wave
{
  
  class Light : public Printable
  {
    public:
    enum class Light_type_e
    {
      None = 0,
      Point_light,
      Directional_light
    };
    Light();
    Light(const Color &color_, float ambient_intensity_, float diffuse_intensity_);
    ~Light() override = default;
    
    [[nodiscard]] virtual const Light_type_e &get_type() const = 0;
    [[nodiscard]] const Color &get_color() const;
    [[nodiscard]] float get_ambient_intensity() const;
    [[nodiscard]] float get_diffuse_intensity() const;
    
    void set_color(const Color &color_);
    void set_ambient_intensity(float ambient_intensity_);
    void set_diffuse_intensity(float diffuse_intensity_);
    protected:
    Color color;
    float ambient_intensity;
    float diffuse_intensity;
  };
  
  class Point_light : public Light
  {
    public:
    Point_light();
    Point_light(const Color &color_, float ambient_intensity_, float diffuse_intensity_, const Vector_3f &position_,
                float constant_, float linear_, float exponent_);
    ~Point_light() override = default;
    
    INTERFACE_PRINTABLE
    
    [[nodiscard]] const Light_type_e &get_type() const override;
    [[nodiscard]] int64_t get_count() const;
    [[nodiscard]] const Vector_3f &get_position() const;
    [[nodiscard]] float get_constant() const;
    [[nodiscard]] float get_linear() const;
    [[nodiscard]] float get_exponent() const;
    
    private:
    Light_type_e light_type = Light_type_e::Point_light;
    int64_t count = 0;
    Vector_3f position;
    float constant;
    float linear;
    float exponent;
  };
}
