//
// Created by nami on 2023-02-01.
//
#include <Renderer/color.h>
#include <Utilities/logger.h>

namespace Wave
{
  
  Color::Color(float uniform, float alpha, bool normalized)
  {
    this->red = uniform;
    this->green = uniform;
    this->blue = uniform;
    this->alpha = alpha;
    
    if (!normalized) normalize();
  }
  
  Color::Color(float red, float green, float blue, float alpha, bool normalized)
  {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
    
    if (!normalized) normalize();
  }
  
  Color::Color(const Color &new_color)
  {
    if (this == &new_color) return;
    *this = new_color;
  }
  
  // When given a hex value for the desired color. For example : FF2F3F4F --> FF = red bits, 2F = green bits,
  // 3F = blue bits, and 4F = alpha bits. The bits are then masked with the ceiling value of RGB coloring (255).
  Color::Color(unsigned long hex_color)
  {
    this->red = (float) ((hex_color >> 24) & 255); // First 8 bits (left to right).
    this->green = (float) ((hex_color >> 16) & 255);
    this->blue = (float) ((hex_color >> 8) & 255);
    this->alpha = (float) (hex_color & 255);  // Last 8 bits (left to right).
    
    normalize();
  }
  
  Color::Color(const char *hex_color_code)
  {
    if (hex_color_code && hex_color_code[0] == '#')
    {
      const char *color_code = ++hex_color_code;  // Discard the # symbol.
      // Transform to base 16 number and call the hex color constructor.
      *this = Color(strtoul(color_code, nullptr, 16));
    }
  }
  
  void Color::normalize()
  {
    // Normalize RGB (0-255) to values between 0 and 1.
    float min = 0.0f, max = 255.0f;
    this->red = (get_red() - min) / (max - min);
    this->green = (get_green() - min) / (max - min);
    this->blue = (get_blue() - min) / (max - min);
    this->alpha = (get_alpha() - min) / (max - min);
  }
  
  void Color::clear()
  {
    *this = Color(); // Default color (Dark gray).
  }
  
  std::string Color::to_string() const
  {
    char buffer[76 + sizeof(float) * 4] {0};
    
    if (snprintf(buffer, 75 + sizeof(float) * 4,
                 "Red value : %.2f, Green value : %.2f, Blue value : %.2f, Alpha value : %.2f",
                 this->get_red(), this->get_green(), this->get_blue(), this->get_alpha()) < 0)
    {
      return {"[MINOR] [COLOR ERROR] --> Error converting color to string, snprintf failed!\n"};
    }
    
    return {buffer};
  }
  
  void Color::print() const
  {
    Wave::alert(WAVE_WARN, "%s", to_string().c_str());
  }
  
  float Color::get_red() const
  {
    return this->red;
  }
  
  float Color::get_green() const
  {
    return this->green;
  }
  
  float Color::get_blue() const
  {
    return this->blue;
  }
  
  float Color::get_alpha() const
  {
    return this->alpha;
  }
  
  void Color::set_red(float red_)
  {
    this->red = red_;
  }
  
  void Color::set_green(float green_)
  {
    this->green = green_;
  }
  
  void Color::set_blue(float blue_)
  {
    this->blue = blue_;
  }
  
  void Color::set_alpha(float alpha_)
  {
    this->alpha = alpha_;
  }
  
  float &Color::operator [](unsigned int index)
  {
    switch (index)
    {
      case 0:return this->red;
      case 1:return this->green;
      case 2:return this->blue;
      case 3:return this->alpha;
      default:return this->red;
    }
  }
  
  Color &Color::operator =(const Color &other_color)
  {
    if (this == &other_color) return *this;
    this->red = other_color.red;
    this->green = other_color.green;
    this->blue = other_color.blue;
    this->alpha = other_color.alpha;
    return *this;
  }
  
  bool Color::operator ==(const Color &other_color)
  {
    if (this == &other_color) return true;
    return (this->red == other_color.red &&
            this->green == other_color.green &&
            this->blue == other_color.blue &&
            this->alpha == other_color.alpha);
  }
  
  bool Color::operator !=(const Color &other_color) const
  {
    if (this == &other_color) return true;
    return this->red == other_color.red &&
           this->green == other_color.green &&
           this->blue == other_color.blue &&
           this->alpha == other_color.alpha;
  }
}
