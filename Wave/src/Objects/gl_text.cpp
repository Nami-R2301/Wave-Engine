//
// Created by nami on 24/04/23.
//

#include <Objects/gl_text.h>
#include <Renderer/renderer.h>

namespace Wave
{
  
  Gl_text::~Gl_text()
  {
    for (const auto &character: this->characters)
    {
      GL_CALL(glDeleteTextures(1, &character.second.texture_id));
    }
  }
  
  Gl_text::Gl_text(const std::string &string)
  {
    int32_t result = Gl_text::init("../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf");
    if (result != 0) alert(WAVE_ERROR, "[Gl Text] --> Error when initializing freetype, error %d", result);
    this->text = string;
  }
  
  Gl_text::Gl_text(const char *font_file_name)
  {
    int32_t result = Gl_text::init(font_file_name);
    if (result != 0) alert(WAVE_ERROR, "[Gl Text] --> Error when initializing freetype, error %d", result);
    this->text = "Example text";
  }
  
  Gl_text::Gl_text(const char *font_file_name, const std::string &string)
  {
    int32_t result = Gl_text::init(font_file_name);
    if (result != 0) alert(WAVE_ERROR, "[Gl Text] --> Error when initializing freetype, error %d", result);
    this->text = string;
  }
  
  Gl_text::Gl_text(const char *font_file_name, const std::string &string, Text_format format_)
  {
    int32_t result = Gl_text::init(font_file_name);
    if (result != 0) alert(WAVE_ERROR, "[Gl Text] --> Error when initializing freetype, error %d", result);
    this->text = string;
    this->format = format_;
  }
  
  int32_t Gl_text::init(const char *font_file_path)
  {
    if (FT_Init_FreeType(&this->ft)) return -1;
    
    if (FT_New_Face(this->ft, font_file_path, 0, &this->face))
    {
      alert(WAVE_ERROR, "[TEXT] --> Error loading font file %s!");
      return -1;
    }
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 24);
    
    // disable byte-alignment restriction
    GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    
    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
      // Load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        alert(WAVE_ERROR, "[TEXT] --> Failed to load Glyph");
        continue;
      }
      
      // generate texture
      unsigned int texture;
      GL_CALL(glGenTextures(1, &texture));
      GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
      GL_CALL(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
      ));
      // set texture options
      GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
      GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
      GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      // now store character for later use
      Glyph character = {
        texture,
        Vector_2f(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        Vector_2f(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)
      };
      this->characters.insert(std::pair<char, Glyph>(c, character));
    }
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    
    FT_Done_Face(this->face);
    FT_Done_FreeType(this->ft);
    return 0;
  }
}