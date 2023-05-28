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
    Gl_text();
    explicit Gl_text(const std::string &string_);
    Gl_text(const char *font_file_name, const std::string &string_);
    Gl_text(const char *font_file_name, const std::string &string_, Text_format text_format_);
    ~Gl_text() override = default;
    
    [[nodiscard]] int32_t init(const char *font_name);
  };
}
