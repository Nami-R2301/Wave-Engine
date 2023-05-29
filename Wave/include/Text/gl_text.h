//
// Created by nami on 24/04/23.
//

#pragma once

#include <Text/text.h>
#include <Math/matrix_4f.h>

namespace Wave
{
  
  class Gl_text : public Text
  {
    public:
    Gl_text();
    explicit Gl_text(const std::string &string_);
    Gl_text(const char *font_file_name, const std::string &string_);
    Gl_text(const char *font_file_name, const std::string &string_, Text_format text_format_);
    ~Gl_text() override;
    
    [[nodiscard]] int32_t init(const char *font_name);
    
    // Interfaces.
    INTERFACE_PRINTABLE
    INTERFACE_MOVABLE
    INTERFACE_ROTATABLE
    INTERFACE_COPIABLE
    INTERFACE_SCALABLE
    
    [[nodiscard]] float get_offset_x() const override;
    [[nodiscard]] float get_offset_y() const override;
    [[nodiscard]] const Color &get_color(char character) const override;
    [[nodiscard]] const Vector_2f &get_scale() const override;
    [[nodiscard]] const Text_format &get_format() const override;
    [[nodiscard]] const std::string &get_string() const override;
    [[nodiscard]] const FT_Face &get_face() const override;
    [[nodiscard]] const Glyph &get_character(uint8_t character_code) override;
    [[nodiscard]] const std::map<uint8_t, Glyph> &get_characters() const override;
    [[nodiscard]] Texture *get_texture_atlas() const override;
    [[nodiscard]] const Vector_2f &get_text_box_size() override;
    
    void set_offset_x(float offset) override;
    void set_offset_y(float offset) override;
    void set_every_color(const Color &uniform) override;
    void set_color(char character, const Color &color) override;
    void set_scale(const Vector_2f &scale) override;
    void set_format(const Text_format &format_) override;
    void set_character(uint8_t character_code, const Glyph &character) override;
    
    Glyph &operator[](uint8_t index) override;
    private:
    std::string string;
    Text_format format;
    FT_Library ft = nullptr;
    FT_Face face = nullptr;
    Texture *texture_atlas = nullptr;
    std::map<uint8_t, Glyph> characters;
  };
}
