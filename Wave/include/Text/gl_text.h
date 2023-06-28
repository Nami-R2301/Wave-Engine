//
// Created by nami on 24/04/23.
//

#pragma once

#include <Text/text.h>
#include <Math/matrix_4f.h>

namespace Wave
{
  
  class Gl_text_box : public Text_box
  {
    public:
    Gl_text_box();
    explicit Gl_text_box(int32_t id_);
    explicit Gl_text_box(const std::shared_ptr<Shader> &associated_shader_, int32_t id_);
    explicit Gl_text_box(const std::string &string_, const std::shared_ptr<Shader> &associated_shader_ = nullptr,
                         int32_t id_ = 0);
    explicit Gl_text_box(const Math::Vector_2f &pixel_size, const std::shared_ptr<Shader> &associated_shader_ = nullptr,
                         int32_t id_ = 0);
    Gl_text_box(const Math::Vector_2f &pixel_size, const std::string &string_,
                const std::shared_ptr<Shader> &associated_shader_ = nullptr, int32_t id_ = 0);
    Gl_text_box(const char *font_file_name, const std::string &string_,
                const std::shared_ptr<Shader> &associated_shader_ = nullptr, int32_t id_ = 0);
    Gl_text_box(const Math::Vector_2f &pixel_size, const std::string &string_, const Text_format_s &text_format,
                const std::shared_ptr<Shader> &associated_shader_ = nullptr, int32_t id_ = 0);
    Gl_text_box(const Math::Vector_2f &pixel_size, const std::string &string_, const char *font_file_path,
                const std::shared_ptr<Shader> &associated_shader_ = nullptr, int32_t id_ = 0);
    Gl_text_box(const char *font_file_name, const std::string &string_, Text_format_s text_format_,
                const std::shared_ptr<Shader> &associated_shader_ = nullptr, int32_t id_ = 0);
    ~Gl_text_box() override;
    
    void prepare_vertices() override;
    void set_transform(const Math::Transform &new_transform) override;
    void update_transform() override;
    
    void append_text(const std::string &text_) override;
    void append_text(const std::string &text_, const Color &uniform_color) override;
    void set_text_box_size(const Math::Vector_2f &size) override;
    void set_text_box_color(const Color &color) override;
    void set_text_uniform_color(const Color &uniform_color) override;
    
    // Events.
    void on_box_resize(const Math::Vector_2f &new_size) override;
    void on_box_resize(float new_width, float new_height) override;
    
    void on_text_resize(const Math::Vector_2f &new_size, const std::string &section_resized) override;
    void on_text_resize(float new_width, float new_height, const std::string &section_resized) override;
    
    void on_recolor(const Color &new_color, const std::string &section_recolored) override;
    
    void on_move(const Math::Vector_2f &new_position) override;
    void on_move(float new_x_coord, float new_y_coord) override;
    
    [[nodiscard]] const Math::Transform &get_text_transform() const override;
    [[nodiscard]] Math::Transform &get_text_transform() override;
    [[nodiscard]] const Math::Vector_2f &get_pixel_size() const override;
    [[nodiscard]] Math::Vector_2f &get_pixel_size() override;
    [[nodiscard]] const Color &get_uniform_text_color() const override;
    [[nodiscard]] Color &get_text_uniform_color() override;
    [[nodiscard]] const Math::Vector_2f &get_text_offset() const override;
    [[nodiscard]] Math::Vector_2f &get_text_offset() override;
    [[nodiscard]] const Color &get_text_color(char character) const override;
    [[nodiscard]] const Math::Vector_2f &get_text_scale() const override;
    [[nodiscard]] Math::Vector_2f &get_text_scale() override;
    [[nodiscard]] const Text_format_s &get_text_format() const override;
    [[nodiscard]] const std::string &get_text_string() const override;
    [[nodiscard]] float get_text_length() const override;
    
    [[nodiscard]] const void *get_vertices() const override;
    [[nodiscard]] uint64_t get_vertex_count() const override;
    
    [[nodiscard]] const Glyph_s &get_character(uint8_t character_code) override;
    [[nodiscard]] const std::map<uint8_t, Glyph_s> &get_characters() const override;
    [[nodiscard]] Texture *get_texture_atlas() const override;
    [[nodiscard]] const Math::Vector_2f &get_text_box_size() const override;
    [[nodiscard]] Math::Vector_2f &get_text_box_size() override;
    [[nodiscard]] const Color &get_text_box_color() const override;
    [[nodiscard]] Color &get_text_box_color() override;
    
    [[nodiscard]] const std::shared_ptr<Shader> &get_shader() const override;
    
    
    void set_text_offset(const Math::Vector_2f &offset_coords) override;
    void set_text_offset(float offset_x, float offset_y) override;
    void set_text_offset_x(float offset_x) override;
    void set_text_offset_y(float offset_y) override;
    
    void blend_text_color(char character, const Color &character_color) override;
    void blend_text_color(const Color &uniform_color) override;
    void set_text_color(char character, const Color &character_color) override;
    void set_text_color(const Color &uniform_color) override;
    void set_text_scale(const Math::Vector_2f &scale_) override;
    
    void set_pixel_size(const Math::Vector_2f &size) override;
    void set_pixel_size(float x, float y) override;
    
    void set_text_format(const Text_format_s &format_) override;
    void set_character(uint8_t character_code, const Glyph_s &character) override;
    void set_text_string(const std::string &text_) override;
    
    // Interfaces.
    INTERFACE_IDENTIFIABLE;
    INTERFACE_SENDABLE;
    INTERFACE_PRINTABLE;
    INTERFACE_MOVABLE;
    INTERFACE_ROTATABLE;
    INTERFACE_COPIABLE;
    INTERFACE_SCALABLE;
    
    private:
    void init_freetype();
  };
}
