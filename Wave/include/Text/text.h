//
// Created by nami on 31/12/22.
//

#pragma once

#include <Renderer/texture.h>
#include <Renderer/color.h>
#include <Math/transform.h>
#include <Renderer/shader.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Wave
{
  
  enum class Text_style_e
  {
    Regular, Medium, Light, Italic, Semi_bold, Bold, Bold_italic
  };
  
  typedef struct Text_format_s
  {
    Vector_2f offset = Vector_2f(25.0f);
    Vector_2f scale = Vector_2f(1.0f);
    Vector_2f text_size = Vector_2f(0.0f, 48.0f);  // Size of all glyphs for a specified string (px).
    Vector_2f box_size = Vector_2f(300.0f, 100.0f);  // Size of text box containing all glyphs (px).
    Text_style_e style = Text_style_e::Regular;
    Color text_uniform_color = Color(1.0f, 1.0f, true);
    Color text_box_color = Color(0.0f, 0.0f, true);
    
    inline bool operator==(const Text_format_s &other_format) const
    {
      return this->offset == other_format.offset && this->scale == other_format.scale &&
             this->text_size == other_format.text_size && this->box_size == other_format.box_size &&
             this->style == other_format.style && this->text_box_color == other_format.text_box_color;
    };
  } Text_format_s;
  
  typedef struct Glyph_s
  {
    float texture_offset;  // Offset of the character glyph texture in the texture atlas.
    Color color = Color(255.0f, 255.0f, false);  // Color of glyph
    unsigned int size_x, size_y;       // Size of glyph
    Vector_2f bearing;    // Offset from baseline to left/top of glyph
    Vector_2f advance;    // Offset to advance to next glyph
  } Glyph;
  
  class Text_box : public Printable, public Movable, public Rotatable, public Copiable, public Scalable,
                   public Sendable
  {
    public:
    ~Text_box() override = default;
    
    static std::shared_ptr<Text_box> create();
    static std::shared_ptr<Text_box> create(const std::shared_ptr<Shader> &associated_shader_);
    static std::shared_ptr<Text_box> create(const Vector_2f &pixel_size_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr);
    static std::shared_ptr<Text_box> create(const std::string &text_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr);
    static std::shared_ptr<Text_box> create(const char *font_file_path_, const std::string &text_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr);
    static std::shared_ptr<Text_box> create(const Vector_2f &pixel_size_, const std::string &text_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr);
    static std::shared_ptr<Text_box> create(const Vector_2f &pixel_size_, const std::string &text_,
                                            const char *font_file_path_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr);
    static std::shared_ptr<Text_box> create(const char *font_file_path_, const std::string &text_,
                                            const Text_format_s &format_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr);
    static std::shared_ptr<Text_box> create(const Vector_2f &pixel_size_, const std::string &text_,
                                            const Text_format_s &format_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr);
    
    void append_text(const std::string &text_);
    void append_text(const std::string &text_, const Color &uniform_color);
    void set_text_box_size(const Vector_2f &size);
    void set_text_box_color(const Color &color);
    void set_text_uniform_color(const Color &uniform_color);
    
    // Events.
    void on_box_resize(const Vector_2f &new_size);
    void on_box_resize(float new_width, float new_height);
    
    void on_text_resize(const Vector_2f &new_size, const std::string &section_resized);
    void on_text_resize(float new_width, float new_height, const std::string &section_resized);
    
    void on_recolor(const Color &new_color, const std::string &section_recolored);
    
    void on_move(const Vector_2f &new_position);
    void on_move(float new_x_coord, float new_y_coord);
    
    [[nodiscard]] const Transform &get_text_transform() const;
    [[nodiscard]] Transform &get_text_transform();
    [[nodiscard]] const Vector_2f &get_pixel_size() const;
    [[nodiscard]] Vector_2f &get_pixel_size();
    [[nodiscard]] const Color &get_uniform_text_color() const;
    [[nodiscard]] Color &get_text_uniform_color();
    [[nodiscard]] const Vector_2f &get_text_offset() const;
    [[nodiscard]] Vector_2f &get_text_offset();
    [[nodiscard]] const Color &get_text_color(char character) const;
    [[nodiscard]] const Vector_2f &get_text_scale() const;
    [[nodiscard]] Vector_2f &get_text_scale();
    [[nodiscard]] const Text_format_s &get_text_format() const;
    [[nodiscard]] const std::string &get_text_string() const;
    [[nodiscard]] float get_text_length() const;
    
    [[nodiscard]] const Glyph_s &get_character(uint8_t character_code);
    [[nodiscard]] const std::map<uint8_t, Glyph_s> &get_characters() const;
    [[nodiscard]] Texture *get_texture_atlas() const;
    [[nodiscard]] const Vector_2f &get_text_box_size() const;
    [[nodiscard]] Vector_2f &get_text_box_size();
    [[nodiscard]] const Color &get_text_box_color() const;
    [[nodiscard]] Color &get_text_box_color();
    
    [[nodiscard]] const std::shared_ptr<Shader> &get_shader() const;
    
    
    void set_text_offset(const Vector_2f &offset_coords);
    void set_text_offset(float offset_x, float offset_y);
    void set_text_offset_x(float offset_x);
    void set_text_offset_y(float offset_y);
    
    void blend_text_color(char character, const Color &character_color);
    void blend_text_color(const Color &uniform_color);
    void set_text_color(char character, const Color &character_color);
    void set_text_color(const Color &uniform_color);
    void set_text_scale(const Vector_2f &scale_);
    
    void set_pixel_size(const Vector_2f &size);
    void set_pixel_size(float x, float y);
    
    void set_text_format(const Text_format_s &format_);
    void set_character(uint8_t character_code, const Glyph_s &character);
    void set_text_string(const std::string &text_);
    
    Glyph_s &operator[](uint8_t index);
    protected:
    std::string text;
    Text_format_s format;
    Vector_2f atlas_size;
    Texture *texture_atlas = nullptr;
    std::map<uint8_t, Glyph_s> characters;
    bool sent = false;
    FT_Face face;
    FT_Library library;
    Transform text_transform = Transform(Vector_3f(0.0f),
                                         Vector_3f(0.0f),
                                         Vector_3f(0.0f));
    std::shared_ptr<Shader> associated_shader;
  };
}
