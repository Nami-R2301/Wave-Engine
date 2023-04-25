//
// Created by nami on 31/12/22.
//

#pragma once

#include <wave_pch.h>
#include <Renderer/color.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Wave
{
  
  enum class Text_style
  {
    REGULAR, MEDIUM, LIGHT, ITALIC, SEMI_BOLD, BOLD, BOLD_ITALIC
  };
  
  typedef struct Text_format
  {
    float offset_x = 25.0f;
    float offset_y = 25.0f;
    float scale = 1.0f;
    float size = 24.0f;  // Size of all glyphs for a specified string (px).
    Text_style style = Text_style::REGULAR;
    Color color = Color(1.0f, 0.0f, 0.0f, 1.0f, true);  // Red.
  } Text_format;
  
  typedef struct Glyph
  {
    uint32_t texture_id;  // ID handle of the glyph texture
    Vector_2f size;       // Size of glyph
    Vector_2f bearing;    // Offset from baseline to left/top of glyph
    uint32_t advance;    // Offset to advance to next glyph
  } Glyph;
  
  class Text
  {
  public:
    virtual ~Text() = default;
    
    static std::shared_ptr<Text> create(const std::string &text_);
    
    [[nodiscard]] float get_offset_x() const;
    [[nodiscard]] float get_offset_y() const;
    [[nodiscard]] float get_scale() const;
    [[nodiscard]] const Text_format &get_format() const;
    [[nodiscard]] const std::string &get_string() const;
    [[nodiscard]] const FT_Face &get_face() const;
    [[nodiscard]] const Glyph &get_character(uint8_t character_code);
    [[nodiscard]] const std::map<uint8_t, Glyph> &get_characters() const;
    
    void set_character(uint8_t character_code, const Glyph &character);
    
    Glyph &operator[](uint8_t index);
  protected:
    std::string text;
    Text_format format;
    FT_Library ft = nullptr;
    FT_Face face = nullptr;
    std::map<uint8_t, Glyph> characters;
  };
}