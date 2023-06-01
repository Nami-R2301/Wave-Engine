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
    explicit Gl_text_box(const std::string &string_);
    explicit Gl_text_box(const Vector_2f &pixel_size);
    Gl_text_box(const Vector_2f &pixel_size, const std::string &string_);
    Gl_text_box(const char *font_file_name, const std::string &string_);
    Gl_text_box(const Vector_2f &pixel_size, const std::string &string_, const char *font_file_path);
    Gl_text_box(const char *font_file_name, const std::string &string_, Text_format_s text_format_);
    ~Gl_text_box() override;
    
    // Interfaces.
    INTERFACE_LOADABLE
    INTERFACE_PRINTABLE
    INTERFACE_MOVABLE
    INTERFACE_ROTATABLE
    INTERFACE_COPIABLE
    INTERFACE_SCALABLE
    INTERFACE_DESTROYABLE
    private:
    void init_freetype();
  };
}
