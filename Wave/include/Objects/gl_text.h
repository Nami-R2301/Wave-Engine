//
// Created by nami on 24/04/23.
//

#pragma once

#include <Objects/text.h>

namespace Wave
{
  
  class Gl_text : public Text
  {
    public:
    explicit Gl_text(const std::string &string);
    explicit Gl_text(const char *font_path);
    Gl_text(const char *font_file_name, const std::string &string);
    Gl_text(const char *font_file_name, const std::string &string, Text_format text_format_);
    ~Gl_text() override;
    
    [[nodiscard]] int32_t init(const char *font_name);
  };
}
