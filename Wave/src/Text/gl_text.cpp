//
// Created by nami on 24/04/23.
//

#include <Text/gl_text.h>
#include <Renderer/gl_renderer.h>

#include <utility>

static const char *s_font_file_path = nullptr;

namespace Wave
{
  
  Gl_text_box::Gl_text_box()
  {
    s_font_file_path = "../Wave/res/Fonts/Comfortaa/Comfortaa-SemiBold.ttf";
    this->text = "?Example text?";
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size)
  {
    this->format.text_size = pixel_size;
    s_font_file_path = "../Wave/res/Fonts/Comfortaa/Comfortaa-SemiBold.ttf";
    this->text = "?Example text?";
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const std::string &text_)
  {
    s_font_file_path = "../Wave/res/Fonts/Comfortaa/Comfortaa-SemiBold.ttf";
    this->text = text_;
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size, const std::string &text_)
  {
    this->format.text_size = pixel_size;
    s_font_file_path = "../Wave/res/Fonts/Comfortaa/Comfortaa-SemiBold.ttf";
    this->text = text_;
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size, const std::string &text_, const char *font_file_name)
  {
    this->format.text_size = pixel_size;
    s_font_file_path = font_file_name;
    this->text = text_;
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size, const std::string &string_, const Text_format_s &text_format)
  {
    this->format.text_size = pixel_size;
    this->text = string_;
    this->format = text_format;
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const char *font_file_name, const std::string &text_)
  {
    s_font_file_path = font_file_name;
    this->text = text_;
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const char *font_file_name, const std::string &text_, Text_format_s format_)
  {
    s_font_file_path = font_file_name;
    this->text = text_;
    this->format = std::move(format_);
    init_freetype();
  }
  
  Gl_text_box::~Gl_text_box()
  {
    Gl_text_box::unbuild();
  }
  
  void Gl_text_box::init_freetype()
  {
    if (!this->is_built()) if (FT_Init_FreeType(&this->library)) return;
    
    if (!this->is_built())
    {
      if (FT_New_Face(this->library, s_font_file_path, 0, &this->face))
      {
        alert(WAVE_LOG_ERROR, "[Gl text] --> Error loading font file %s!", s_font_file_path);
        // If the font file is missing, use the callback (default) one.
        FT_New_Face(this->library, "../Wave/res/Fonts/Comfortaa/Comfortaa-SemiBold.ttf", 0, &this->face);
      }
    }
    
    // Set size to build glyphs as.
    FT_Set_Pixel_Sizes(this->face, ((long) this->format.text_size.get_x() - (long) this->format.text_size.get_x() % 2),
                       ((long) this->format.text_size.get_y() - (long) this->format.text_size.get_y() % 2));
    
    unsigned int atlas_total_width = 0, atlas_row_width = 0, atlas_total_height = 0, atlas_row_height = 0;
    Color glyph_color = Color(1.0f, 1.0f, true);
    unsigned int texture_offset_x = 0;
    
    // Load first 128 characters of ASCII set
    for (unsigned char character = 32; character < 128; character++)
    {
      // Load character glyph
      if (FT_Load_Char(this->face, character, FT_LOAD_RENDER))
      {
        alert(WAVE_LOG_ERROR, "[TEXT] --> Failed to build Glyph %c", character);
        continue;
      }
      
      atlas_total_width += this->face->glyph->bitmap.width;  // Add padding to avoid artefacts.
      atlas_total_height =
        std::max(atlas_total_height, this->face->glyph->bitmap.rows);  // Add padding to avoid artefacts.
      
      // Store character for later use.
      Glyph_s character_glyph = {
        (float) texture_offset_x,
        glyph_color,
        this->face->glyph->bitmap.width,
        this->face->glyph->bitmap.rows,
        Vector_2f((float) this->face->glyph->bitmap_left, (float) this->face->glyph->bitmap_top),
        // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        Vector_2f((float) (this->face->glyph->advance.x >> 6), (float) (this->face->glyph->advance.y >> 6))
      };
      this->characters[character] = character_glyph;
      
      texture_offset_x += this->face->glyph->bitmap.width;
    }
    
    atlas_total_width = std::max(atlas_total_width, atlas_row_width);
    atlas_total_height += atlas_row_height;
    
    this->atlas_size.set_x((float) atlas_total_width);
    this->atlas_size.set_y((float) atlas_total_height);
    
    alert(WAVE_LOG_DEBUG, "Total atlas texture size : (%d, %d)", atlas_total_width, atlas_total_height);
  }
  
  void Gl_text_box::build()
  {
    if (!this->is_built())
    {
      CHECK_GL_CALL(glPixelStorei(GL_PACK_ALIGNMENT, 1));  // Disable byte-alignment restriction.
      CHECK_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));  // Disable byte-alignment restriction.
    }
    
    // If we are rebuilding.
    if (this->is_built())
    {
      if (this->texture_atlas) this->texture_atlas->unbuild();
      
      init_freetype();
      this->texture_atlas->set_width(this->atlas_size.get_x());
      this->texture_atlas->set_height(this->atlas_size.get_y());
    } else  // If we are building for the first time.
    {
      // Create texture atlas for all glyphs of the current face.
      this->texture_atlas = new Gl_texture_2D(s_font_file_path, {Texture::Texture_type_e::Texture_2D,
                                                                 Texture::Texture_internal_format_e::Red,
                                                                 this->atlas_size.get_x(),
                                                                 this->atlas_size.get_y(),
                                                                 WAVE_VALUE_DONT_CARE,
                                                                 0,
                                                                 WAVE_VALUE_DONT_CARE,
                                                                 nullptr});
    }
    this->texture_atlas->build();
    
    if (!this->texture_atlas) return;
    
    // Set texture glyph data for current face.
    for (int i = 32; i < 128; i++)
    {
      if (FT_Load_Char(this->face, i, FT_LOAD_RENDER))
      {
        alert(WAVE_LOG_ERROR, "[TEXT] --> Failed to build Glyph_s");
        continue;
      }
      
      this->texture_atlas->bind(this->texture_atlas->get_texture_slot());
      CHECK_GL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0, this->characters[i].texture_offset,
                                    0,
                                    this->face->glyph->bitmap.width,
                                    this->face->glyph->bitmap.rows,
                                    GL_RED, GL_UNSIGNED_BYTE,
                                    this->face->glyph->bitmap.buffer));
      
      this->characters[i].texture_offset /= (float) this->texture_atlas->get_width();
    }
    
    this->built = true;
  }
  
  void Gl_text_box::unbuild()
  {
    if (this->is_built())
    {
      FT_Done_Face(this->face);
      FT_Done_FreeType(this->library);
      delete this->texture_atlas;
      this->built = false;
    }
  }
  
  std::string Gl_text_box::to_string() const
  {
    char buffer[FILENAME_MAX * 4]{0};
    std::string style;
    switch (this->format.style)
    {
      case Text_style_e::Light:
      {
        style = "Light";
        break;
      }
      case Text_style_e::Regular:
      {
        style = "Regular";
        break;
      }
      case Text_style_e::Medium:
      {
        style = "Medium";
        break;
      }
      case Text_style_e::Semi_bold:
      {
        style = "Semi bold";
        break;
      }
      case Text_style_e::Bold:
      {
        style = "Bold";
        break;
      }
      case Text_style_e::Italic:
      {
        style = "Italic";
        break;
      }
      case Text_style_e::Bold_italic:
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