//
// Created by nami on 24/04/23.
//

#include <Text/gl_text.h>
#include <Renderer/gl_renderer.h>

#include <utility>

namespace Wave
{
  
  Gl_text::Gl_text()
  {
    this->format = {25.0f,
                    25.0f,
                    Vector_2f(1.0f),
                    Vector_2f(0.0f, 24.0f),
                    Vector_2f(250.0f),
                    Wave::Text_style::Regular};
    this->string = "?Example text?";
    
    int32_t result = Gl_text::init("../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf");
    if (result != 0) alert(WAVE_LOG_ERROR, "[GL Text] --> Error when initializing freetype, error %d", result);
  }
  
  Gl_text::Gl_text(const std::string &string_)
  {
    this->format = {25.0f,
                    25.0f,
                    Vector_2f(1.0f),
                    Vector_2f(0.0f, 24.0f),
                    Vector_2f(250.0f),
                    Wave::Text_style::Regular};
    
    this->string = string_;
    int32_t result = Gl_text::init("../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf");
    if (result != 0) alert(WAVE_LOG_ERROR, "[GL Text] --> Error when initializing freetype, error %d", result);
  }
  
  Gl_text::Gl_text(const char *font_file_name, const std::string &string_)
  {
    this->format = {25.0f,
                    25.0f,
                    Vector_2f(1.0f),
                    Vector_2f(0.0f, 24.0f),
                    Vector_2f(250.0f),
                    Wave::Text_style::Regular};
    
    this->string = string_;
    int32_t result = Gl_text::init(font_file_name);
    if (result != 0) alert(WAVE_LOG_ERROR, "[GL Text] --> Error when initializing freetype, error %d", result);
  }
  
  Gl_text::Gl_text(const char *font_file_name, const std::string &string_, Text_format format_)
  {
    int32_t result = Gl_text::init(font_file_name);
    if (result != 0) alert(WAVE_LOG_ERROR, "[GL Text] --> Error when initializing freetype, error %d", result);
    this->string = string_;
    this->format = std::move(format_);
  }
  
  Gl_text::~Gl_text()
  {
    delete this->texture_atlas;
  }
  
  int32_t Gl_text::init(const char *font_file_path)
  {
    if (FT_Init_FreeType(&this->ft)) return -1;
    
    if (FT_New_Face(this->ft, font_file_path, 0, &this->face))
    {
      alert(WAVE_LOG_ERROR, "[TEXT] --> Error loading font file %s!", font_file_path);
      return -1;
    }
    
    // Set size to load glyphs as.
    FT_Set_Pixel_Sizes(this->face, (unsigned int) this->format.text_size.get_x(),
                       (unsigned int) this->format.text_size.get_y());
    CHECK_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));  // Disable byte-alignment restriction.
    
    float atlas_total_width = 0, atlas_row_width = 0, atlas_total_height = 0, atlas_row_height = 0, offset_x = 0, offset_y = 0;
    
    // Load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
      // Load character glyph
      if (FT_Load_Char(this->face, c, FT_LOAD_RENDER))
      {
        alert(WAVE_LOG_ERROR, "[TEXT] --> Failed to load Glyph");
        continue;
      }
      
      Color glyph_color = Color(1.0f, 1.0f, true);
      
      offset_x = c;
      // Get atlas total size for the font atlas generation later.
      if (atlas_row_width + (float) this->face->glyph->bitmap.width + 1 > this->format.box_size.get_x())
      {
        atlas_total_width = std::max(atlas_total_width, atlas_row_width);
        atlas_total_height += atlas_row_height;
        atlas_row_width = 0, atlas_row_height = 0;
        offset_x = 0, offset_y += atlas_row_height;
      }
      
      
      // Store character for later use.
      Glyph character = {
        Vector_2f(offset_x, offset_y),
        glyph_color,
        Vector_2f((float) this->face->glyph->bitmap.width, (float) this->face->glyph->bitmap.rows),
        Vector_2f((float) this->face->glyph->bitmap_left, (float) this->face->glyph->bitmap_top),
        static_cast<unsigned int>(this->face->glyph->advance.x),
        {Texture::Texture_type_e::Texture_2D,
         Texture::Texture_internal_format_e::Red,
         static_cast<int32_t>(this->face->glyph->bitmap.width),
         static_cast<int32_t>(this->face->glyph->bitmap.rows),
         WAVE_VALUE_DONT_CARE,
         0,
         WAVE_VALUE_DONT_CARE,
         this->face->glyph->bitmap.buffer}
      };
      
      atlas_row_width += (float) this->face->glyph->bitmap.width + 1;
      atlas_row_height = std::max(atlas_row_height, (float) this->face->glyph->bitmap.rows);
      this->characters.insert(std::pair<char, Glyph>(c, character));
    }
    
    atlas_total_width = std::max(atlas_total_width, atlas_row_width);
    atlas_total_height += atlas_row_height;
    
    alert(WAVE_LOG_ERROR, "Total atlas texture size : (%.2f, %.2f)", atlas_total_width, atlas_total_height);
    
    // Create texture atlas for all glyphs of the current face.
    this->texture_atlas = new Gl_texture_2D(font_file_path, {Texture::Texture_type_e::Texture_2D,
                                                             Texture::Texture_internal_format_e::Red,
                                                             static_cast<int32_t>(atlas_total_width),
                                                             static_cast<int32_t>(atlas_total_height),
                                                             WAVE_VALUE_DONT_CARE,
                                                             0,
                                                             WAVE_VALUE_DONT_CARE,
                                                             nullptr});
    
    // Set texture glyph data for current face.
    for (const auto &character: this->characters)
    {
      uint32_t offset_array[2] = {static_cast<uint32_t>(character.second.texture_offset.get_x()),
                                  static_cast<uint32_t>(character.second.texture_offset.get_y())};
      if (this->texture_atlas)
        this->texture_atlas->set_data(&character.second.texture_data,
                                      reinterpret_cast<uint32_t *>(&offset_array));
    }
    
    FT_Done_Face(this->face);
    FT_Done_FreeType(this->ft);
    return 0;
  }
  
  std::string Gl_text::to_string() const
  {
    char buffer[FILENAME_MAX * 4]{0};
    std::string style;
    switch (this->format.style)
    {
      case Text_style::Light:
      {
        style = "Light";
        break;
      }
      case Text_style::Regular:
      {
        style = "Regular";
        break;
      }
      case Text_style::Medium:
      {
        style = "Medium";
        break;
      }
      case Text_style::Semi_bold:
      {
        style = "Semi bold";
        break;
      }
      case Text_style::Bold:
      {
        style = "Bold";
        break;
      }
      case Text_style::Italic:
      {
        style = "Italic";
        break;
      }
      case Text_style::Bold_italic:
      {
        style = "Bold italic";
        break;
      }
    }
    if (snprintf(buffer, sizeof(buffer), "[Gl text] :\n%55sString --> %s\n%55sOffset x --> %.2f\n"
                                         "%55sOffset y --> %.2f\n%55sScale --> (%.2f, %.2f)\n%55sText size --> (%.2f, %.2f)\n"
                                         "%55sText-box size --> (%.2f, %.2f)\n%55sStyle --> %s",
                 DEFAULT, this->string.c_str(), DEFAULT, this->format.offset_x, DEFAULT, this->format.offset_y, DEFAULT,
                 this->format.scale.get_x(), this->format.scale.get_y(),
                 DEFAULT, this->format.text_size.get_x(), this->format.text_size.get_y(), DEFAULT,
                 this->format.box_size.get_x(), this->format.box_size.get_y(), DEFAULT, style.c_str()) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Gl text]!";
    }
    return buffer;
  }
  
  void Gl_text::translate(const Wave::Vector_3f &position)
  {
    this->format.offset_x += position.get_x();
    this->format.offset_y += position.get_y();
  }
  
  void Gl_text::translate(float x, float y, [[maybe_unused]] float z)
  {
    this->format.offset_x += x;
    this->format.offset_y += y;
  }
  
  void Gl_text::rotate(const Wave::Vector_3f &angle)
  {
    Vector_3f temp_angle = angle;
    // Get angles and convert to radiant.
    temp_angle.set_x((float) (angle.get_x() * (M_PI / 180)));
    temp_angle.set_y((float) (angle.get_y() * (M_PI / 180)));
    temp_angle.set_z((float) (angle.get_z() * (M_PI / 180)));
    
    this->format.offset_x = cosf(temp_angle.get_x()) - sinf(temp_angle.get_y());
    this->format.offset_y = cosf(temp_angle.get_y()) + sinf(temp_angle.get_x());
  }
  
  void Gl_text::rotate(float x, float y, [[maybe_unused]] float z)
  {
    // Get angles and convert to radiant.
    float angle_x = x * (float) (M_PI / 180), angle_y = y * (float) (M_PI / 180);
    
    this->format.offset_x = cosf(angle_x) - sinf(angle_y);
    this->format.offset_y = cosf(angle_y) + sinf(angle_x);
  }
  
  void Gl_text::scale(const Wave::Vector_3f &scalar)
  {
    this->format.scale *= (Vector_2f) scalar;
  }
  
  void Gl_text::scale(float x, float y, [[maybe_unused]] float z)
  {
    this->format.scale *= Vector_2f(x, y);
  }
  
  void *Gl_text::copy() const
  {
    std::shared_ptr<Text> copy = std::make_shared<Gl_text>(*this);
    return (void *) &(*copy);
  }
  
  const std::string &Gl_text::get_string() const
  {
    return this->string;
  }
  
  const Text_format &Gl_text::get_format() const
  {
    return this->format;
  }
  
  float Gl_text::get_offset_x() const
  {
    return this->format.offset_x;
  }
  
  float Gl_text::get_offset_y() const
  {
    return this->format.offset_y;
  }
  
  const Color &Gl_text::get_color(char character) const
  {
    return this->characters.at(character).color;
  }
  
  const Vector_2f &Gl_text::get_scale() const
  {
    return this->format.scale;
  }
  
  const FT_Face &Gl_text::get_face() const
  {
    return this->face;
  }
  
  const Glyph &Gl_text::get_character(uint8_t character_code)
  {
    return this->characters[character_code];
  }
  
  Texture *Gl_text::get_texture_atlas() const
  {
    return this->texture_atlas;
  }
  
  const Vector_2f &Gl_text::get_text_box_size()
  {
    return this->format.box_size;
  }
  
  const std::map<uint8_t, Glyph> &Gl_text::get_characters() const
  {
    return this->characters;
  }
  
  void Gl_text::set_character(uint8_t character_code, const Glyph &character)
  {
    this->characters[character_code] = character;
  }
  
  Glyph &Gl_text::operator[](uint8_t index)
  {
    return index < this->characters.size() ? this->characters[index] : this->characters[0];
  }
  
  void Gl_text::set_format(const Text_format &format_)
  {
    this->format = format_;
  }
  
  void Gl_text::set_offset_x(float offset)
  {
    this->format.offset_x = offset;
  }
  
  void Gl_text::set_offset_y(float offset)
  {
    this->format.offset_y = offset;
  }
  
  void Gl_text::set_every_color(const Color &uniform)
  {
    for (const auto &c: this->characters) this->characters[c.first].color = uniform;
  }
  
  void Gl_text::set_color(char character, const Color &color)
  {
    this->characters[character].color = color;
  }
  
  void Gl_text::set_scale(const Vector_2f &scale)
  {
    this->format.scale = scale;
  }
}