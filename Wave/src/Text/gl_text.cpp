//
// Created by nami on 24/04/23.
//

#include <Text/gl_text.h>
#include <Renderer/gl_renderer.h>

static const char *s_font_file_path = nullptr;

namespace Wave
{
  
  Gl_text_box::Gl_text_box()
  {
    s_font_file_path = "../Wave/res/Fonts/Roboto/Roboto-Regular.ttf";
    this->text = "?Example text?";
    this->associated_shader = Shader::create("Text_box",
                                             Wave::Resource_loader::load_shader_source(
                                               "../Wave/res/Shaders/text-glyph.vert"),
                                             Wave::Resource_loader::load_shader_source(
                                               "../Wave/res/Shaders/text-glyph.frag"));
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(int32_t id_)
  {
    this->id = id_;
    s_font_file_path = "../Wave/res/Fonts/Roboto/Roboto-Regular.ttf";
    this->text = "?Example text?";
    this->associated_shader = Shader::create("Text_box",
                                             Wave::Resource_loader::load_shader_source(
                                               "../Wave/res/Shaders/text-glyph.vert"),
                                             Wave::Resource_loader::load_shader_source(
                                               "../Wave/res/Shaders/text-glyph.frag"));
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    this->id = id_;
    s_font_file_path = "../Wave/res/Fonts/Roboto/Roboto-Regular.ttf";
    this->text = "?Example text?";
    this->associated_shader = associated_shader_;
    if (!this->associated_shader)
    {
      this->associated_shader = Shader::create("Text_box",
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.vert"),
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.frag"));
    }
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size, const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    this->id = id_;
    this->format.text_size = pixel_size;
    s_font_file_path = "../Wave/res/Fonts/Roboto/Roboto-Regular.ttf";
    this->text = "?Example text?";
    this->associated_shader = associated_shader_;
    if (!this->associated_shader)
    {
      this->associated_shader = Shader::create("Text_box",
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.vert"),
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.frag"));
    }
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const std::string &text_, const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    this->id = id_;
    s_font_file_path = "../Wave/res/Fonts/Roboto/Roboto-Regular.ttf";
    this->text = text_;
    this->associated_shader = associated_shader_;
    if (!this->associated_shader)
    {
      this->associated_shader = Shader::create("Text_box",
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.vert"),
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.frag"));
    }
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size, const std::string &text_,
                           const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    this->id = id_;
    this->format.text_size = pixel_size;
    s_font_file_path = "../Wave/res/Fonts/Roboto/Roboto-Regular.ttf";
    this->text = text_;
    this->associated_shader = associated_shader_;
    if (!this->associated_shader)
    {
      this->associated_shader = Shader::create("Text_box",
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.vert"),
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.frag"));
    }
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size, const std::string &text_, const char *font_file_name,
                           const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    this->id = id_;
    this->format.text_size = pixel_size;
    s_font_file_path = font_file_name;
    this->text = text_;
    this->associated_shader = associated_shader_;
    if (!this->associated_shader)
    {
      this->associated_shader = Shader::create("Text_box",
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.vert"),
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.frag"));
    }
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const Vector_2f &pixel_size, const std::string &string_, const Text_format_s &text_format,
                           const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    this->id = id_;
    this->format.text_size = pixel_size;
    this->text = string_;
    this->format = text_format;
    this->associated_shader = associated_shader_;
    if (!this->associated_shader)
    {
      this->associated_shader = Shader::create("Text_box",
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.vert"),
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.frag"));
    }
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const char *font_file_name, const std::string &text_,
                           const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    this->id = id_;
    s_font_file_path = font_file_name;
    this->text = text_;
    this->associated_shader = associated_shader_;
    if (!this->associated_shader)
    {
      this->associated_shader = Shader::create("Text_box",
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.vert"),
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.frag"));
    }
    init_freetype();
  }
  
  Gl_text_box::Gl_text_box(const char *font_file_name, const std::string &text_, Text_format_s format_,
                           const std::shared_ptr<Shader> &associated_shader_, int32_t id_)
  {
    this->id = id_;
    s_font_file_path = font_file_name;
    this->text = text_;
    this->format = std::move(format_);
    this->associated_shader = associated_shader_;
    if (!this->associated_shader)
    {
      this->associated_shader = Shader::create("Text_box",
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.vert"),
                                               Wave::Resource_loader::load_shader_source(
                                                 "../Wave/res/Shaders/text-glyph.frag"));
    }
    init_freetype();
  }
  
  Gl_text_box::~Gl_text_box()
  {
    Gl_text_box::free_gpu(1);
    delete this->texture_atlas;
  }
  
  void Gl_text_box::init_freetype()
  {
    if (!this->is_sent()) if (FT_Init_FreeType(&this->library)) return;
    
    if (!this->is_sent())
    {
      if (FT_New_Face(this->library, s_font_file_path, 0, &this->face))
      {
        alert(WAVE_LOG_ERROR, "[Gl text] --> Error loading font file %s!", s_font_file_path);
        // If the font file is missing, use the callback (default) one.
        FT_New_Face(this->library, "../Wave/res/Fonts/Roboto/Roboto-Regular.ttf", 0, &this->face);
      }
    }
    
    // Set size to build glyphs as.
    FT_Set_Pixel_Sizes(this->face, ((long) this->format.text_size.get_x() - (long) this->format.text_size.get_x() % 2),
                       ((long) this->format.text_size.get_y() - (long) this->format.text_size.get_y() % 2));
    
    unsigned int atlas_total_width = 0, atlas_row_width = 0, atlas_total_height = 0, atlas_row_height = 0;
    Color glyph_default_color = Color(1.0f, 1.0f, true);
    unsigned int texture_offset_x = 0;
    
    // Load first 128 characters of ASCII set
    for (unsigned char character = 32; character < 128; character++)
    {
      // Load character glyph
      if (FT_Load_Char(this->face, character, FT_LOAD_RENDER))
      {
        alert(WAVE_LOG_ERROR, "[Gl text] --> Failed to build Glyph %c", character);
        continue;
      }
      
      atlas_total_width += this->face->glyph->bitmap.width;  // Add padding to avoid artefacts.
      atlas_total_height =
        std::max(atlas_total_height, this->face->glyph->bitmap.rows);  // Add padding to avoid artefacts.
      
      // Store character for later use.
      Glyph_s character_glyph = {
        (float) texture_offset_x,
        this->characters.contains(character) ? this->characters.at(character).color : glyph_default_color,
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
  }
  
  void Gl_text_box::send_gpu([[maybe_unused]] uint64_t instance_count)
  {
    if (!this->is_sent())
    {
      CHECK_GL_CALL(glPixelStorei(GL_PACK_ALIGNMENT, 1));  // Disable byte-alignment restriction.
      CHECK_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));  // Disable byte-alignment restriction.
    }
    
    // If we are rebuilding.
    if (this->is_sent())
    {
      if (this->texture_atlas) this->texture_atlas->free_gpu(1);
      
      init_freetype();
      this->texture_atlas->set_width((int32_t) this->atlas_size.get_x());
      this->texture_atlas->set_height((int32_t) this->atlas_size.get_y());
    } else  // If we are building for the first time.
    {
      // Create texture atlas for all glyphs of the current face.
      this->texture_atlas = new Gl_texture_2D(nullptr, {Texture::Texture_type_e::Texture_2D,
                                                        Texture::Texture_internal_format_e::Red,
                                                        (int32_t) this->atlas_size.get_x(),
                                                        (int32_t) this->atlas_size.get_y(),
                                                        0,
                                                        6,
                                                        1,
                                                        nullptr});
    }
    this->texture_atlas->send_gpu(1);
    
    if (!this->texture_atlas) return;
    
    // Set texture glyph data for current face.
    for (int i = 32; i < 128; i++)
    {
      if (FT_Load_Char(this->face, i, FT_LOAD_RENDER))
      {
        alert(WAVE_LOG_ERROR, "[Gl text] --> Failed to build Glyph_s");
        continue;
      }
      
      this->texture_atlas->bind(this->texture_atlas->get_texture_slot());
      CHECK_GL_CALL(glTexSubImage2D(GL_TEXTURE_2D,
                                    0,
                                    this->characters[i].texture_offset,
                                    0,
                                    this->face->glyph->bitmap.width,
                                    this->face->glyph->bitmap.rows,
                                    GL_RED, GL_UNSIGNED_BYTE,
                                    this->face->glyph->bitmap.buffer));
      
      this->characters[i].texture_offset /= (float) this->texture_atlas->get_width();
    }
    
    this->prepare_vertices();
    if (!this->is_sent())
      Renderer::send_text(*this->associated_shader, *this->texture_atlas, get_vertices(),
                          get_vertex_count(), get_vertex_size());
    else
      Renderer::send_text(*this->associated_shader, *this->texture_atlas, get_vertices(),
                          get_vertex_count(), get_vertex_size(), nullptr,
                          0, 0);  // If we are overwriting the current text_box vbo.
    this->sent = true;
  }
  
  void Gl_text_box::prepare_vertices()
  {
    if (!this->glyph_vertices.empty())
    {
      this->glyph_vertices.clear();
      this->glyph_vertices.resize(0);
    }
    
    float offset_x = get_text_offset().get_x(), offset_y = get_text_offset().get_y();
    const Vector_2f &scale = get_text_scale();
    
    for (char character: this->text)
    {
      Glyph_s glyph = this->characters.at(character);
      
      float red = this->characters.at(character).color.get_red(),
        green = this->characters.at(character).color.get_green(),
        blue = this->characters.at(character).color.get_blue(),
        alpha = this->characters.at(character).color.get_alpha();
      
      auto texture_offset_x = (float) glyph.texture_offset;
      
      float x_pos = offset_x + (glyph.bearing.get_x() * scale.get_x());
      float y_pos = -offset_y - (glyph.bearing.get_y() * scale.get_y());
      float w = (float) glyph.size_x * scale.get_x();
      float h = (float) glyph.size_y * scale.get_y();
      
      // Advance cursors for next glyph (note that advance is number of 1/64 pixels)
      offset_x += glyph.advance.get_x() * scale.get_x();
      offset_y += glyph.advance.get_y() * scale.get_y();
      
      // Update VBO for each character
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Vector_2f(x_pos, -y_pos),
                                            Color(red, green, blue, alpha, true),
                                            Vector_2f(texture_offset_x + 0.0001f, 0)
                                          });
      
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Vector_2f(x_pos + w, -y_pos),
                                            Color(red, green, blue, alpha, true),
                                            Vector_2f(
                                              texture_offset_x + (float) (glyph.size_x - 1) / atlas_size.get_x(), 0)
                                          });
      
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Vector_2f(x_pos, (-y_pos - h)),
                                            Color(red, green, blue, alpha, true),
                                            Vector_2f(texture_offset_x + 0.0001f,
                                                      (float) (glyph.size_y - 1) / atlas_size.get_y())
                                          });
      
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Vector_2f(x_pos + w, -y_pos),
                                            Color(red, green, blue, alpha, true),
                                            Vector_2f(
                                              texture_offset_x + (float) (glyph.size_x - 1) / atlas_size.get_x(), 0)
                                          });
      
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Vector_2f(x_pos, (-y_pos - h)),
                                            Color(red, green, blue, alpha, true),
                                            Vector_2f(texture_offset_x + 0.0001f,
                                                      (float) (glyph.size_y - 1) / atlas_size.get_y())
                                          });
      
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Vector_2f(x_pos + w, (-y_pos - h)),
                                            Color(red, green, blue, alpha, true),
                                            Vector_2f(
                                              texture_offset_x + (float) (glyph.size_x - 1) / atlas_size.get_x(),
                                              (float) (glyph.size_y - 1) /
                                              atlas_size.get_y())
                                          });
    }
  }
  
  void Gl_text_box::free_gpu(uint64_t instance_count)
  {
    if (this->is_sent())
    {
      FT_Done_Face(this->face);
      FT_Done_FreeType(this->library);
      this->sent = false;
      this->texture_atlas->free_gpu(instance_count);
      this->glyph_vertices.clear();
      this->glyph_vertices.resize(0);
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