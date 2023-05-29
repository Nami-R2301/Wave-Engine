//
// Created by nami on 31/12/22.
//

#pragma once

#include <Renderer/texture.h>
#include <Renderer/color.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Wave
{
  
  enum class Text_style
  {
    Regular, Medium, Light, Italic, Semi_bold, Bold, Bold_italic
  };
  
  typedef struct Text_format
  {
    float offset_x = 25.0f;
    float offset_y = 25.0f;
    Vector_2f scale = Vector_2f(1.0f);
    Vector_2f text_size = Vector_2f(0.0f, 24.0f);  // Size of all glyphs for a specified string (px).
    Vector_2f box_size = Vector_2f(250.0f);  // Size of text box containing all glyphs (px).
    Text_style style = Text_style::Regular;
  } Text_format;
  
  typedef struct Glyph
  {
    Vector_2f texture_offset;  // Offset of the character glyph texture in the texture atlas.
    Color color = Color(255.0f, 255.0f, false);  // Color of glyph
    Vector_2f size;       // Size of glyph
    Vector_2f bearing;    // Offset from baseline to left/top of glyph
    uint32_t advance;    // Offset to advance to next glyph
    Texture::Texture_data_s texture_data;          // Texture buffer data.
  } Glyph;
  
  class Text : public Printable, public Movable, public Rotatable, public Copiable, public Scalable
  {
    public:
    ~Text() override = default;
    
    static std::shared_ptr<Text> create();
    static std::shared_ptr<Text> create(const std::string &string_);
    static std::shared_ptr<Text> create(const char *font_file_path, const std::string &string_);
    static std::shared_ptr<Text> create(const char *font_file_path, const std::string &string_,
                                        const Text_format &format_);
    
    [[nodiscard]] virtual float get_offset_x() const = 0;
    [[nodiscard]] virtual float get_offset_y() const = 0;
    [[nodiscard]] virtual const Color &get_color(char character) const = 0;
    [[nodiscard]] virtual const Vector_2f &get_scale() const = 0;
    [[nodiscard]] virtual const Text_format &get_format() const = 0;
    [[nodiscard]] virtual const std::string &get_string() const = 0;
    [[nodiscard]] virtual const FT_Face &get_face() const = 0;
    [[nodiscard]] virtual const Glyph &get_character(uint8_t character_code) = 0;
    [[nodiscard]] virtual const std::map<uint8_t, Glyph> &get_characters() const = 0;
    [[nodiscard]] virtual Texture *get_texture_atlas() const = 0;
    [[nodiscard]] virtual const Vector_2f &get_text_box_size() = 0;
    
    virtual void set_offset_x(float offset) = 0;
    virtual void set_offset_y(float offset) = 0;
    virtual void set_every_color(const Color &uniform) = 0;
    virtual void set_color(char character, const Color &color) = 0;
    virtual void set_scale(const Vector_2f &scale_) = 0;
    virtual void set_format(const Text_format &format_) = 0;
    virtual void set_character(uint8_t character_code, const Glyph &character) = 0;
    
    virtual Glyph &operator[](uint8_t index) = 0;
  };
}
