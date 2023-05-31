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
    Vector_2f offset = Vector_2f(25.0f);
    Vector_2f scale = Vector_2f(1.0f);
    Vector_2f text_size = Vector_2f(0.0f, 24.0f);  // Size of all glyphs for a specified string (px).
    Vector_2f box_size = Vector_2f(250.0f);  // Size of text box containing all glyphs (px).
    Text_style style = Text_style::Regular;
  } Text_format;
  
  typedef struct Glyph
  {
    float texture_offset;  // Offset of the character glyph texture in the texture atlas.
    Color color = Color(255.0f, 255.0f, false);  // Color of glyph
    Vector_2f size;       // Size of glyph
    Vector_2f bearing;    // Offset from baseline to left/top of glyph
    Vector_2f advance;    // Offset to advance to next glyph
  } Glyph;
  
  class Text_box : public Printable, public Movable, public Rotatable, public Copiable, public Scalable,
                   public Loadable, public Destroyable
  {
    public:
    ~Text_box() override = default;
    
    static std::shared_ptr<Text_box> create();
    static std::shared_ptr<Text_box> create(const Vector_2f &pixel_size_);
    static std::shared_ptr<Text_box> create(const std::string &text_);
    static std::shared_ptr<Text_box> create(const char *font_file_path_, const std::string &text_);
    static std::shared_ptr<Text_box> create(const Vector_2f &pixel_size_, const std::string &text_);
    static std::shared_ptr<Text_box> create(const Vector_2f &pixel_size_, const std::string &text_,
                                            const char *font_file_path_);
    static std::shared_ptr<Text_box> create(const char *font_file_path_, const std::string &text_,
                                            const Text_format &format_);
    
    void append_text(const std::string &text);
    void set_text_box_size(const Vector_2f &size);
    
    [[nodiscard]] const Vector_2f &get_pixel_size() const;
    
    [[nodiscard]] const Vector_2f &get_text_offset() const;
    [[nodiscard]] const Color &get_text_color(char character) const;
    [[nodiscard]] const Vector_2f &get_text_scale() const;
    [[nodiscard]] const Text_format &get_text_format() const;
    [[nodiscard]] const std::string &get_text_string() const;
    [[nodiscard]] float get_text_length() const;
    
    [[nodiscard]] const Glyph &get_character(uint8_t character_code);
    [[nodiscard]] const std::map<uint8_t, Glyph> &get_characters() const;
    [[nodiscard]] Texture *get_texture_atlas() const;
    [[nodiscard]] const Vector_2f &get_text_box_size() const;
    
    void set_text_offset(const Vector_2f &offset_coords);
    void set_text_offset(float offset_x, float offset_y);
    void set_text_offset_x(float offset_x);
    void set_text_offset_y(float offset_y);
    void set_text_color(char character, const Color &color);
    void set_text_uniform_color(const Color &color);
    void set_text_scale(const Vector_2f &scale_);
    
    void set_pixel_size(const Vector_2f &size);
    void set_pixel_size(float x, float y);
    
    void set_text_format(const Text_format &format_);
    void set_character(uint8_t character_code, const Glyph &character);
    void set_text_string(const std::string &text_);
    
    Glyph &operator[](uint8_t index);
    protected:
    const char *font_file_path = nullptr;
    std::string text;
    Text_format format;
    Texture *texture_atlas = nullptr;
    std::map<uint8_t, Glyph> characters;
    bool loaded = false;
    FT_Face face;
    FT_Library library;
  };
}
