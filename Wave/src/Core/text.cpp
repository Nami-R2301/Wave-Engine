//
// Created by nami on 31/12/22.
//

#include <Objects/text.h>
#include <Utilities/logger.h>

namespace Wave
{
  
  Text::Text()
  {
    if (Text::init_ft("Comfortaa.ttf") != 0) alert(WAVE_ERROR, "[TEXT] --> Error initialising freetype Library!");
  }
  
  Text::Text(const char *font_file_name)
  {
    if (Text::init_ft(font_file_name) != 0) alert(WAVE_ERROR, "[TEXT] --> Error initialising freetype Library!");
  }
  
  Text::Text(const std::string &text_)
  {
    if (Text::init_ft("Comfortaa.ttf") != 0) alert(WAVE_ERROR, "[TEXT] --> Error initialising freetype Library!");
    this->text = text_;
  }
  
  Text::Text(const char *font_file_name, const std::string &text_)
  {
    if (Text::init_ft(font_file_name) != 0) alert(WAVE_ERROR, "[TEXT] --> Error initialising freetype Library!");
    this->text = text_;
  }
  
  Text::Text(const char *font_file_name, const std::string &text_, float offset_y_, float offset_x_, float scale_)
  {
    if (Text::init_ft(font_file_name) != 0) alert(WAVE_ERROR, "[TEXT] --> Error initialising freetype Library!");
    this->text = text_;
    this->offset_y = offset_y_;
    this->offset_x = offset_x_;
    this->scale = scale_;
  }
  
  Text::~Text()
  {
    FT_Done_Face(this->face);
    FT_Done_FreeType(this->ft);
  }
  
  const std::string &Text::get_text() const
  {
    return this->text;
  }
  
  float Text::get_offset_x() const
  {
    return this->offset_x;
  }
  
  float Text::get_offset_y() const
  {
    return this->offset_y;
  }
  
  float Text::get_scale() const
  {
    return this->scale;
  }
  
  const FT_Face &Text::get_face() const
  {
    return this->face;
  }
  
  const Glyph &Text::get_character(uint8_t character_code)
  {
    return this->characters[character_code];
  }
  
  const std::map<uint8_t, Glyph> &Text::get_characters() const
  {
    return this->characters;
  }
  
  void Text::set_character(uint8_t character_code, const Glyph &character)
  {
    this->characters[character_code] = character;
  }
  
  int32_t Text::init_ft(const char *font_file_name)
  {
    if (FT_Init_FreeType(&this->ft)) return -1;
    
    char buffer[FILENAME_MAX];
    if (snprintf(buffer, FILENAME_MAX, "../Wave/Resources/Fonts/%s", font_file_name) < 0)
    {
      alert(WAVE_ERROR, "[TEXT] --> Error loading font file %s!", font_file_name);
      return -1;
    }
    
    if (FT_New_Face(this->ft, buffer, 0, &this->face))
    {
      alert(WAVE_ERROR, "[GUI] --> FREETYPE: Failed to load font!");
      return -1;
    }
    return 0;
  }
  
  Gl_Text::Gl_Text() : Text()
  {
  
  }
}
