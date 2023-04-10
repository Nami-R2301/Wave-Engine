//
// Created by nami on 2023-02-01.
//

#pragma once

#include <wave_pch.h>

namespace Wave
{
  
  #define COLOR_SIZE ((sizeof(float) * 4))
  
  // Set custom colors for the background or for fragments.
  class Color
  {
  public:
    explicit Color(float uniform, float alpha, bool normalized = false);
    Color(float red, float green, float blue, float alpha, bool normalized = false); // Dark gray.
    explicit Color(unsigned long hex_color = 0xFFFFFFFF);
    explicit Color(const char *hex_color_code);
    Color(const Color &color);
    ~Color() = default;
    
    void normalize();
    void clear();
    [[nodiscard]] std::string to_string() const;
    void print() const;
    
    [[nodiscard]] float get_red() const;
    [[nodiscard]] float get_blue() const;
    [[nodiscard]] float get_green() const;
    [[nodiscard]] float get_alpha() const;
    
    void set_red(float red);
    void set_green(float green);
    void set_blue(float blue);
    void set_alpha(float alpha);
    
    Color &operator =(const Color &other_color);
    bool operator !=(const Color &other_color) const;
    bool operator ==(const Color &other_color);
  
  private:
    float red = 1.0f;
    float green = 1.0f;
    float blue = 1.0f;
    float alpha = 1.0f;
  };
}