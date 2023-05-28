//
// Created by nami on 24/04/23.
//

#include <Objects/gl_text.h>
#include <Renderer/gl_renderer.h>

#include <utility>

namespace Wave
{
  
  Gl_text::Gl_text()
  {
    this->format = {25.0f,
                    1080.0f - 25.0f,
                    1.0f,
                    26.0f,
                    Wave::Text_style::REGULAR,
                    Wave::Color(1.0f, 1.0f, 1.0f, 1.0f, true)};
    this->string = "?Example text?";
    
    int32_t result = Gl_text::init("../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf");
    if (result != 0) alert(WAVE_ERROR, "[GL Text] --> Error when initializing freetype, error %d", result);
  }
  
  Gl_text::Gl_text(const std::string &string_)
  {
    this->format = {25.0f,
                    1080.0f - 25.0f,
                    1.0f,
                    26.0f,
                    Wave::Text_style::REGULAR,
                    Wave::Color(1.0f, 1.0f, 1.0f, 1.0f, true)};
    
    this->string = string_;
    int32_t result = Gl_text::init("../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf");
    if (result != 0) alert(WAVE_ERROR, "[GL Text] --> Error when initializing freetype, error %d", result);
  }
  
  Gl_text::Gl_text(const char *font_file_name, const std::string &string_)
  {
    this->format = {25.0f,
                    1080.0f - 25.0f,
                    1.0f,
                    26.0f,
                    Wave::Text_style::REGULAR,
                    Wave::Color(1.0f, 1.0f, 1.0f, 1.0f, true)};
    
    this->string = string_;
    int32_t result = Gl_text::init(font_file_name);
    if (result != 0) alert(WAVE_ERROR, "[GL Text] --> Error when initializing freetype, error %d", result);
  }
  
  Gl_text::Gl_text(const char *font_file_name, const std::string &string_, Text_format format_)
  {
    int32_t result = Gl_text::init(font_file_name);
    if (result != 0) alert(WAVE_ERROR, "[GL Text] --> Error when initializing freetype, error %d", result);
    this->string = string_;
    this->format = std::move(format_);
  }

  int32_t Gl_text::init(const char *font_file_path)
  {
    if (FT_Init_FreeType(&this->ft)) return -1;
    
    if (FT_New_Face(this->ft, font_file_path, 0, &this->face))
    {
      alert(WAVE_ERROR, "[TEXT] --> Error loading font file %s!", font_file_path);
      return -1;
    }
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(this->face, 0, 24);
    
    // disable byte-alignment restriction
    CHECK_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    
    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
      // Load character glyph
      if (FT_Load_Char(this->face, c, FT_LOAD_RENDER))
      {
        alert(WAVE_ERROR, "[TEXT] --> Failed to load Glyph");
        continue;
      }
      
      // Generate texture for each glyph.
      auto texture = Texture::create(font_file_path, {Texture_type::Texture_2D,
                                                      static_cast<int32_t>(face->glyph->bitmap.width),
                                                      static_cast<int32_t>(face->glyph->bitmap.rows),
                                                      WAVE_VALUE_DONT_CARE,
                                                      0,
                                                      WAVE_VALUE_DONT_CARE,
                                                      face->glyph->bitmap.buffer});
      
      // Store character for later use
      Glyph character = {
        texture,
        Vector_2f((float) face->glyph->bitmap.width, (float) face->glyph->bitmap.rows),
        Vector_2f((float) face->glyph->bitmap_left, (float) face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)
      };
      this->characters.insert(std::pair<char, Glyph>(c, character));
    }
    
    FT_Done_Face(this->face);
    FT_Done_FreeType(this->ft);
    return 0;
  }
}