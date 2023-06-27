//
// Created by nami on 07/06/23.
//

#include <Lighting/light.h>

namespace Wave
{
  
  
  Light::Light()
  {
    this->color = Color(1.0f, 1.0f, true);
    this->ambient_intensity = 1.0f;
    this->diffuse_intensity = 0.0f;
  }
  
  Light::Light(const Color &color_, float ambient_intensity_, float diffuse_intensity_)
  {
    this->color = color_;
    this->ambient_intensity = ambient_intensity_;
    this->diffuse_intensity = diffuse_intensity_;
  }
  
  const Color &Light::get_color() const
  {
    return this->color;
  }
  
  float Light::get_ambient_intensity() const
  {
    return this->ambient_intensity;
  }
  
  float Light::get_diffuse_intensity() const
  {
    return this->diffuse_intensity;
  }
  
  void Light::set_color(const Color &color_)
  {
    this->color = color_;
  }
  
  void Light::set_ambient_intensity(float ambient_intensity_)
  {
    this->ambient_intensity = ambient_intensity_;
  }
  
  void Light::set_diffuse_intensity(float diffuse_intensity_)
  {
    this->diffuse_intensity = diffuse_intensity_;
  }
  
  /****************************** POINT LIGHT ***************************************/
  
  Point_light::Point_light() : Light()
  {
    this->position = Math::Vector_3f(0.0f);
    this->constant = 1.0f;
    this->linear = 0.0f;
    this->exponent = 0.0f;
    this->count++;
  }
  
  Point_light::Point_light(const Color &color_, float ambient_intensity_, float diffuse_intensity_,
                           const Math::Vector_3f &position_, float constant_, float linear_, float exponent_) :
    Light(color_, ambient_intensity_, diffuse_intensity_)
  {
    this->position = position_;
    this->constant = constant_;
    this->linear = linear_;
    this->exponent = exponent_;
    this->count++;
  }
  
  std::string Point_light::to_string() const
  {
    char buffer[FILENAME_MAX]{0};
    
    if (snprintf(buffer, sizeof(buffer), "[Light] :\n%55sType --> Generic\n%55sColor --> %s\n%55s"
                                         "Ambient intensity --> %.2f\n%55sDiffuse intensity --> %.2f",
                 DEFAULT, DEFAULT, this->color.to_string().c_str(),
                 DEFAULT, this->ambient_intensity, DEFAULT,
                 this->diffuse_intensity) < 0)
    {
      alert(WAVE_LOG_ERROR, "[Light] --> Error displaying light properties! (Snprintf error)");
      return "Error displaying light properties!";
    }
    return buffer;
  }
  
  int64_t Point_light::get_count() const
  {
    return this->count;
  }
  
  const Light::Light_type_e &Point_light::get_type() const
  {
    return this->light_type;
  }
  
  const Math::Vector_3f &Point_light::get_position() const
  {
    return this->position;
  }
  
  float Point_light::get_constant() const
  {
    return this->constant;
  }
  
  float Point_light::get_linear() const
  {
    return this->linear;
  }
  
  float Point_light::get_exponent() const
  {
    return this->exponent;
  }
}