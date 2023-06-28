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
    Math::Vector_2f offset = Math::Vector_2f(25.0f);
    Math::Vector_2f scale = Math::Vector_2f(1.0f);
    Math::Vector_2f text_size = Math::Vector_2f(0.0f, 48.0f);  // Size of all glyphs for a specified string (px).
    Math::Vector_2f box_size = Math::Vector_2f(300.0f, 100.0f);  // Size of text box containing all glyphs (px).
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
    Math::Vector_2f bearing;    // Offset from baseline to left/top of glyph
    Math::Vector_2f advance;    // Offset to advance to next glyph
  } Glyph;
  
  typedef struct Glyph_quad_s
  {
    int32_t entity_id = -1;
    Math::Vector_2f position = Math::Vector_2f(0.0f);
    Color color = Color(1.0f, 1.0f, true);
    Math::Vector_2f texture_offset = Math::Vector_2f(0.0f);
  } Glyph_quad_s;
  
  class Text_box : public I_printable, public I_movable, public I_rotatable, public I_copiable, public I_scalable,
                   public I_sendable, public I_identifiable
  {
    public:
    ~Text_box() override = default;
    
    static std::shared_ptr<Text_box> create();
    static std::shared_ptr<Text_box> create(int32_t id_);
    static std::shared_ptr<Text_box> create(const std::shared_ptr<Shader> &associated_shader_, int32_t id_ = 0);
    static std::shared_ptr<Text_box> create(const Math::Vector_2f &pixel_size_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr,
                                            int32_t id_ = 0);
    static std::shared_ptr<Text_box> create(const std::string &text_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr,
                                            int32_t id_ = 0);
    static std::shared_ptr<Text_box> create(const char *font_file_path_, const std::string &text_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr,
                                            int32_t id_ = 0);
    static std::shared_ptr<Text_box> create(const Math::Vector_2f &pixel_size_, const std::string &text_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr,
                                            int32_t id_ = 0);
    static std::shared_ptr<Text_box> create(const Math::Vector_2f &pixel_size_, const std::string &text_,
                                            const char *font_file_path_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr,
                                            int32_t id_ = 0);
    static std::shared_ptr<Text_box> create(const char *font_file_path_, const std::string &text_,
                                            const Text_format_s &format_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr,
                                            int32_t id_ = 0);
    static std::shared_ptr<Text_box> create(const Math::Vector_2f &pixel_size_, const std::string &text_,
                                            const Text_format_s &format_,
                                            const std::shared_ptr<Shader> &associated_shader_ = nullptr,
                                            int32_t id_ = 0);
    
    virtual void prepare_vertices() = 0;
    
    virtual void append_text(const std::string &text_) = 0;
    virtual void append_text(const std::string &text_, const Color &uniform_color) = 0;
    virtual void set_text_box_size(const Math::Vector_2f &size) = 0;
    virtual void set_text_box_color(const Color &color) = 0;
    virtual void set_text_uniform_color(const Color &uniform_color) = 0;
    
    // Events.
    virtual void on_box_resize(const Math::Vector_2f &new_size) = 0;
    virtual void on_box_resize(float new_width, float new_height) = 0;
    
    virtual void on_text_resize(const Math::Vector_2f &new_size, const std::string &section_resized) = 0;
    virtual void on_text_resize(float new_width, float new_height, const std::string &section_resized) = 0;
    
    virtual void on_recolor(const Color &new_color, const std::string &section_recolored) = 0;
    
    virtual void on_move(const Math::Vector_2f &new_position) = 0;
    virtual void on_move(float new_x_coord, float new_y_coord) = 0;
    
    [[nodiscard]] virtual const Math::Transform &get_text_transform() const = 0;
    [[nodiscard]] virtual Math::Transform &get_text_transform() = 0;
    [[nodiscard]] virtual const Math::Vector_2f &get_pixel_size() const = 0;
    [[nodiscard]] virtual Math::Vector_2f &get_pixel_size() = 0;
    [[nodiscard]] virtual const Color &get_uniform_text_color() const = 0;
    [[nodiscard]] virtual Color &get_text_uniform_color() = 0;
    [[nodiscard]] virtual const Math::Vector_2f &get_text_offset() const = 0;
    [[nodiscard]] virtual Math::Vector_2f &get_text_offset() = 0;
    [[nodiscard]] virtual const Color &get_text_color(char character) const = 0;
    [[nodiscard]] virtual const Math::Vector_2f &get_text_scale() const = 0;
    [[nodiscard]] virtual Math::Vector_2f &get_text_scale() = 0;
    [[nodiscard]] virtual const Text_format_s &get_text_format() const = 0;
    [[nodiscard]] virtual const std::string &get_text_string() const = 0;
    [[nodiscard]] virtual float get_text_length() const = 0;
    
    [[nodiscard]] virtual const void *get_vertices() const = 0;
    [[nodiscard]] virtual uint64_t get_vertex_count() const = 0;
    [[nodiscard]] static uint64_t get_vertex_size();
    
    [[nodiscard]] virtual const Glyph_s &get_character(uint8_t character_code) = 0;
    [[nodiscard]] virtual const std::map<uint8_t, Glyph_s> &get_characters() const = 0;
    [[nodiscard]] virtual Texture *get_texture_atlas() const = 0;
    [[nodiscard]] virtual const Math::Vector_2f &get_text_box_size() const = 0;
    [[nodiscard]] virtual Math::Vector_2f &get_text_box_size() = 0;
    [[nodiscard]] virtual const Color &get_text_box_color() const = 0;
    [[nodiscard]] virtual Color &get_text_box_color() = 0;
    
    [[nodiscard]] virtual const std::shared_ptr<Shader> &get_shader() const = 0;
    
    
    virtual void set_text_offset(const Math::Vector_2f &offset_coords) = 0;
    virtual void set_text_offset(float offset_x, float offset_y) = 0;
    virtual void set_text_offset_x(float offset_x) = 0;
    virtual void set_text_offset_y(float offset_y) = 0;
    
    virtual void blend_text_color(char character, const Color &character_color) = 0;
    virtual void blend_text_color(const Color &uniform_color) = 0;
    virtual void set_text_color(char character, const Color &character_color) = 0;
    virtual void set_text_color(const Color &uniform_color) = 0;
    virtual void set_text_scale(const Math::Vector_2f &scale_) = 0;
    
    virtual void set_pixel_size(const Math::Vector_2f &size) = 0;
    virtual void set_pixel_size(float x, float y) = 0;
    
    virtual void set_text_format(const Text_format_s &format_) = 0;
    virtual void set_character(uint8_t character_code, const Glyph_s &character) = 0;
    virtual void set_text_string(const std::string &text_) = 0;
    
    virtual void set_transform(const Math::Transform &new_transform) = 0;
    virtual void update_transform() = 0;
    
    Glyph_s &operator[](uint8_t index);
    protected:
    int32_t id = -1;
    std::string text;
    Text_format_s format;
    Math::Vector_2f atlas_size;
    Texture *texture_atlas = nullptr;
    std::map<uint8_t, Glyph_s> characters;
    bool sent = false;
    bool recompile_freetype = true;
    bool data_has_changed = false;
    FT_Face face;
    FT_Library library;
    Math::Transform text_transform = Math::Transform(Math::Vector_3f(0.0f),
                                                     Math::Vector_3f(0.0f),
                                                     Math::Vector_3f(0.0f));
    std::shared_ptr<Shader> associated_shader;
    std::vector<Glyph_quad_s> glyph_vertices;
  };
}
