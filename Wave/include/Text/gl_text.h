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
