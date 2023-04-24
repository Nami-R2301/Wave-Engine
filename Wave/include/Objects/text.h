//
// Created by nami on 31/12/22.
//

#pragma once

#include <Renderer/texture.h>
#include <Renderer/color.h>
#include <Math/vector.h>
#include <wave_pch.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Wave
{
  
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
    Text();
    explicit Text(const std::string &text_);
    explicit Text(const char *font_file_name);
    Text(const char *font_file_name, const std::string &text_);
    Text(const char *font_file_name, const std::string &text_, float offset_y_ = 0.0f, float offset_x_ = 0.0f,
         float scale_ = 1.0f);
    virtual ~Text();
    
    [[nodiscard]] float get_offset_x() const;
    [[nodiscard]] float get_offset_y() const;
    [[nodiscard]] float get_scale() const;
    [[nodiscard]] const std::string &get_text() const;
    [[nodiscard]] const FT_Face &get_face() const;
    [[nodiscard]] const Glyph &get_character(uint8_t character_code);
    [[nodiscard]] const std::map<uint8_t, Glyph> &get_characters() const;
    
    void set_character(uint8_t character_code, const Glyph &character);
  
  protected:
    std::string text;
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    float scale = 0.0f;
    FT_Library ft = nullptr;
    FT_Face face = nullptr;
    std::map<uint8_t, Glyph> characters;
    
    virtual int init_ft(const char *font_file_name);
  };
  
  class Gl_Text : public Text
  {
  public:
    Gl_Text();
    ~Gl_Text() override = default;
  };
}
