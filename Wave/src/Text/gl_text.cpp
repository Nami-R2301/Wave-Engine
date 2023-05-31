//
// Created by nami on 24/04/23.
//

#include <Text/gl_text.h>
#include <Renderer/gl_renderer.h>

#include <utility>

namespace Wave
{
  
  Gl_text_box::Gl_text_box()
  {
    this->font_file_path = "../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf";
    this->text = "?Example text?";
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size)
  {
    this->format.text_size = pixel_size;
    this->font_file_path = "../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf";
    this->text = "?Example text?";
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const std::string &text_)
  {
    this->font_file_path = "../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf";
    this->text = text_;
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size, const std::string &text_)
  {
    this->format.text_size = pixel_size;
    this->font_file_path = "../Wave/res/Fonts/Comfortaa/Comfortaa-Regular.ttf";
    this->text = text_;
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size, const std::string &text_, const char *font_file_name)
  {
    this->format.text_size = pixel_size;
    this->font_file_path = font_file_name;
    this->text = text_;
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const char *font_file_name, const std::string &text_)
  {
    this->font_file_path = font_file_name;
    this->text = text_;
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const char *font_file_name, const std::string &text_, Text_format format_)
  {
    this->font_file_path = font_file_name;
    this->text = text_;
    this->format = std::move(format_);
    init_freetype();
  }
  
  Gl_text_box::~Gl_text_box()
  {
    Gl_text_box::destroy();
  }
  
  void Gl_text_box::init_freetype()
  {
    if (FT_Init_FreeType(&this->library)) return;
    
    if (FT_New_Face(this->library, this->font_file_path, 0, &this->face))
    {
      alert(WAVE_LOG_ERROR, "[Gl text] --> Error loading font file %s!", this->font_file_path);
      return;
    }
    
    // Set size to load glyphs as.
    FT_Set_Pixel_Sizes(this->face, 0, (unsigned int) this->format.text_size.get_y());
    
    float atlas_total_width = 0, atlas_row_width = 0, atlas_total_height = 0, atlas_row_height = 0;
    Color glyph_color = Color(1.0f, 1.0f, true);
    int texture_offset_x = 0;
    
    // Load first 128 characters of ASCII set
    for (unsigned char character = 32; character < 128; character++)
    {
      // Load character glyph
      if (FT_Load_Char(this->face, character, FT_LOAD_RENDER))
      {
        alert(WAVE_LOG_ERROR, "[TEXT] --> Failed to load Glyph");
        continue;
      }
      
      atlas_total_width += (float) this->face->glyph->bitmap.width;  // Add padding to avoid artefacts.
      atlas_total_height =
        std::max(atlas_total_height, (float) this->face->glyph->bitmap.rows);  // Add padding to avoid artefacts.
      
      // Store character for later use.
      Glyph character_glyph = {
        (float) texture_offset_x,
        glyph_color,
        Vector_2f((float) this->face->glyph->bitmap.width, (float) this->face->glyph->bitmap.rows),
        Vector_2f((float) this->face->glyph->bitmap_left, (float) this->face->glyph->bitmap_top),
        // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        Vector_2f((float) (this->face->glyph->advance.x >> 6), (float) (this->face->glyph->advance.y >> 6))
      };
      this->characters.insert(std::pair<char, Glyph>(character, character_glyph));
      
      texture_offset_x += (int) this->face->glyph->bitmap.width;
    }
    
    atlas_total_width = std::max(atlas_total_width, atlas_row_width + 5);
    atlas_total_height += atlas_row_height + 5;
    
    alert(WAVE_LOG_ERROR, "Total atlas texture size : (%.2f, %.2f)", atlas_total_width, atlas_total_height);
    
    // Create texture atlas for all glyphs of the current face.
    this->texture_atlas = new Gl_texture_2D(this->font_file_path, {Texture::Texture_type_e::Texture_2D,
                                                                   Texture::Texture_internal_format_e::Red,
                                                                   atlas_total_width,
                                                                   atlas_total_height,
                                                                   WAVE_VALUE_DONT_CARE,
                                                                   0,
                                                                   WAVE_VALUE_DONT_CARE,
                                                                   nullptr});
  }
  
  void Gl_text_box::load()
  {
    if (this->is_loaded()) return;
    
    CHECK_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));  // Disable byte-alignment restriction.
    if (!this->texture_atlas) return;
    this->texture_atlas->load();
    
    int texture_offset_x = 0;
    
    // Set texture glyph data for current face.
    for (int i = 32; i < 128; i++)
    {
      if (FT_Load_Char(this->face, i, FT_LOAD_RENDER))
        continue;
      
      this->texture_atlas->bind(this->texture_atlas->get_texture_slot());
      CHECK_GL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0, texture_offset_x, 0,
                                    (int) this->face->glyph->bitmap.width,
                                    (int) this->face->glyph->bitmap.rows,
                                    GL_RED, GL_UNSIGNED_BYTE,
                                    this->face->glyph->bitmap.buffer));
      
      this->characters[i].texture_offset /= (float) this->texture_atlas->get_width();
      
      texture_offset_x += (int) this->face->glyph->bitmap.width;
    }
    
    FT_Done_Face(this->face);
    FT_Done_FreeType(this->library);
    
    this->loaded = true;
  }
  
  void Gl_text_box::destroy()
  {
    if (this->is_loaded())
    {
      delete this->texture_atlas;
      this->loaded = false;
    }
  }
  
  std::string Gl_text_box::to_string() const
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
                 DEFAULT, this->text.c_str(), DEFAULT, this->format.offset.get_x(), DEFAULT,
                 this->format.offset.get_y(), DEFAULT, this->format.scale.get_x(), this->format.scale.get_y(),
                 DEFAULT, this->format.text_size.get_x(), this->format.text_size.get_y(), DEFAULT,
                 this->format.box_size.get_x(), this->format.box_size.get_y(), DEFAULT, style.c_str()) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Gl text]!";
    }
    return buffer;
  }
  
  void Gl_text_box::translate(const Wave::Vector_3f &position)
  {
    this->format.offset += (Vector_2f) position;
  }
  
  void Gl_text_box::translate(float x, float y, [[maybe_unused]] float z)
  {
    this->format.offset += Vector_2f(x, y);
  }
  
  void Gl_text_box::rotate(const Wave::Vector_3f &angle)
  {
    Vector_3f temp_angle = angle;
    // Get angles and convert to radiant.
    temp_angle.set_x((float) (angle.get_x() * (M_PI / 180)));
    temp_angle.set_y((float) (angle.get_y() * (M_PI / 180)));
    temp_angle.set_z((float) (angle.get_z() * (M_PI / 180)));
    
    this->format.offset.set_x(cosf(temp_angle.get_x()) - sinf(temp_angle.get_y()));
    this->format.offset.set_y(cosf(temp_angle.get_y()) + sinf(temp_angle.get_x()));
  }
  
  void Gl_text_box::rotate(float x, float y, [[maybe_unused]] float z)
  {
    // Get angles and convert to radiant.
    float angle_x = x * (float) (M_PI / 180), angle_y = y * (float) (M_PI / 180);
    
    this->format.offset.set_x(cosf(angle_x) - sinf(angle_y));
    this->format.offset.set_y(cosf(angle_y) + sinf(angle_x));
  }
  
  void Gl_text_box::scale(const Wave::Vector_3f &scalar)
  {
    this->format.scale *= (Vector_2f) scalar;
  }
  
  void Gl_text_box::scale(float x, float y, [[maybe_unused]] float z)
  {
    this->format.scale *= Vector_2f(x, y);
  }
  
  void *Gl_text_box::copy() const
  {
    std::shared_ptr<Text_box> copy = std::make_shared<Gl_text_box>(*this);
    return (void *) &(*copy);
  }
}