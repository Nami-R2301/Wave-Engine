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
  
  Gl_text_box::Gl_text_box(const Math::Vector_2f &pixel_size, const std::shared_ptr<Shader> &associated_shader_,
                           int32_t id_)
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
  
  Gl_text_box::Gl_text_box(const Math::Vector_2f &pixel_size, const std::string &text_,
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
  
  Gl_text_box::Gl_text_box(const Math::Vector_2f &pixel_size, const std::string &text_, const char *font_file_name,
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
  
  Gl_text_box::Gl_text_box(const Math::Vector_2f &pixel_size, const std::string &string_,
                           const Text_format_s &text_format,
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
    if (!this->is_sent())
    {
      if (FT_Init_FreeType(&this->library)) return;
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
        Math::Vector_2f((float) this->face->glyph->bitmap_left, (float) this->face->glyph->bitmap_top),
        // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        Math::Vector_2f((float) (this->face->glyph->advance.x >> 6), (float) (this->face->glyph->advance.y >> 6))
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
    // If we are building the atlas for the first time.
    if (!Gl_text_box::is_sent())
    {
      CHECK_GL_CALL(glPixelStorei(GL_PACK_ALIGNMENT, 1));  // Disable byte-alignment restriction.
      CHECK_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));  // Disable byte-alignment restriction.
    }
    
    // If we are rebuilding the atlas.
    if (Gl_text_box::data_has_changed)
    {
      if (Gl_text_box::is_sent()) this->texture_atlas->free_gpu(1);
      
      // If we need to resize face glyphs.
      if (Gl_text_box::recompile_freetype) init_freetype();
      
      if (!Gl_text_box::is_sent())
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
      } else
      {
        this->texture_atlas->set_width((int32_t) this->atlas_size.get_x());
        this->texture_atlas->set_height((int32_t) this->atlas_size.get_y());
      }
      // Don't change GPU atlas data, if we haven't changed the actual texture data and instead, only changed the text box.
      if (Gl_text_box::recompile_freetype)
      {
        this->texture_atlas->send_gpu(1);
        
        this->texture_atlas->bind(this->texture_atlas->get_texture_slot());
        // Set texture glyph data for current face.
        for (int i = 32; i < 128; i++)
        {
          if (FT_Load_Char(this->face, i, FT_LOAD_RENDER))
          {
            alert(WAVE_LOG_ERROR, "[Gl text] --> Failed to build Glyph_s");
            continue;
          }
          
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
      }
      
      Gl_text_box::prepare_vertices();
      Gl_text_box::update_transform();
      
      if (Gl_text_box::is_sent())
      {
        // If we are overwriting the current text_box vbo.
        Renderer::replace_draw_command(this->id, *this->associated_shader, this->glyph_vertices, {},
                                       *this->texture_atlas);
      } else
      {
        // If we are sending the text box for the first time.
        for (uint64_t i = 0; i < instance_count; ++i)
          Renderer::add_draw_command(this->id, *this->associated_shader, this->glyph_vertices, {},
                                     *this->texture_atlas);
      }
    }
    
    Gl_text_box::recompile_freetype = false;  // Reset to avoid unnecessary recompiling of freetype for next send call.
    Gl_text_box::data_has_changed = false;
    Gl_text_box::sent = true;
  }
  
  void Gl_text_box::prepare_vertices()
  {
    if (!this->glyph_vertices.empty())
    {
      this->glyph_vertices.clear();
      this->glyph_vertices.resize(0);
    }
    
    float offset_x = get_text_offset().get_x(), offset_y = get_text_offset().get_y();
    const Math::Vector_2f &scale = get_text_scale();
    
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
                                            Math::Vector_2f(x_pos, -y_pos),
                                            Color(red, green, blue, alpha, true),
                                            Math::Vector_2f(texture_offset_x + 0.0001f, 0)
                                          });
      
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Math::Vector_2f(x_pos + w, -y_pos),
                                            Color(red, green, blue, alpha, true),
                                            Math::Vector_2f(
                                              texture_offset_x + (float) (glyph.size_x - 1) / atlas_size.get_x(), 0)
                                          });
      
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Math::Vector_2f(x_pos, (-y_pos - h)),
                                            Color(red, green, blue, alpha, true),
                                            Math::Vector_2f(texture_offset_x + 0.0001f,
                                                            (float) (glyph.size_y - 1) / atlas_size.get_y())
                                          });
      
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Math::Vector_2f(x_pos + w, -y_pos),
                                            Color(red, green, blue, alpha, true),
                                            Math::Vector_2f(
                                              texture_offset_x + (float) (glyph.size_x - 1) / atlas_size.get_x(), 0)
                                          });
      
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Math::Vector_2f(x_pos, (-y_pos - h)),
                                            Color(red, green, blue, alpha, true),
                                            Math::Vector_2f(texture_offset_x + 0.0001f,
                                                            (float) (glyph.size_y - 1) / atlas_size.get_y())
                                          });
      
      this->glyph_vertices.emplace_back(Glyph_quad_s
                                          {
                                            this->id,
                                            Math::Vector_2f(x_pos + w, (-y_pos - h)),
                                            Color(red, green, blue, alpha, true),
                                            Math::Vector_2f(
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
      if (Renderer::get_state().code != WAVE_RENDERER_SHUTDOWN)
        Renderer::remove_draw_command(this->associated_shader->get_id(), this->id);
      this->glyph_vertices.clear();
      this->glyph_vertices.resize(0);
    }
  }
  
  void Gl_text_box::on_box_resize(const Math::Vector_2f &new_size)
  {
    if (new_size.get_x() < 0.0f || new_size.get_y() < 0 || new_size.get_x() > 8000.0f || new_size.get_y() > 8000.0f)
    {
      alert(WAVE_LOG_ERROR, "[On text box resize] --> Attempted to resize text box to (%.2f, %.2f)!",
            new_size.get_x(), new_size.get_y());
      return;
    }
    
    if (this->get_pixel_size().get_y() > new_size.get_y())
      this->format.box_size += Math::Vector_2f(0.0f, this->get_pixel_size().get_y() + 25.0f);
    
    // Append overflowing text to the bottom left of the box to mimic newlines.
    float length = 0;
    float new_line_advance_offset;
    for (const auto &character: this->text)
    {
      if (length >= new_size.get_x())
      {
        new_line_advance_offset = -length;
        length = this->characters.at(character).advance.get_x();
        if (this->characters.contains(character))
          this->characters.at(character).advance = Math::Vector_2f(new_line_advance_offset,
                                                                   this->get_pixel_size().get_y() + 25.0f);
      }
      if (this->characters.contains(character))
      {
        length += (float) this->characters.at(character).advance.get_x() * this->format.scale.get_x();
        Gl_text_box::data_has_changed = true;
      }
    }
  }
  
  void Gl_text_box::on_box_resize(float new_width, float new_height)
  {
    if (new_width < 0.0f || new_height < 0.0f || new_width > 8000.0f || new_height > 8000.0f)
    {
      alert(WAVE_LOG_ERROR, "[On text box resize] --> Attempted to resize text box to (%.2f, %.2f)!",
            new_width, new_height);
      return;
    }
    
    if (this->get_text_length() > new_width)
      this->format.box_size += Math::Vector_2f(0.0f, this->get_pixel_size().get_y() + 25.0f);
    
    // Append overflowing text to the bottom left of the box to mimic newlines.
    float length = 0;
    float new_line_advance_offset;
    for (const auto &character: this->text)
    {
      if (length >= new_width)
      {
        new_line_advance_offset = -length;
        if (this->characters.contains(character))
        {
          length = this->characters.at(character).advance.get_x();
          this->characters.at(character).advance = Math::Vector_2f(new_line_advance_offset,
                                                                   this->get_pixel_size().get_y() + 25.0f);
        }
      }
      if (this->characters.contains(character))
      {
        length += (float) this->characters.at(character).advance.get_x() * this->format.scale.get_x();
        Gl_text_box::data_has_changed = true;
      }
    }
  }
  
  void Gl_text_box::on_text_resize(const Math::Vector_2f &new_size, const std::string &section_resized)
  {
    if (new_size.get_x() < 0.0f || new_size.get_y() < 0.0f || new_size.get_x() > 500.0f || new_size.get_y() > 500.0f)
    {
      alert(WAVE_LOG_ERROR, "[On text box resize] --> Attempted to resize text to (%.2f, %.2f)!",
            new_size.get_x(), new_size.get_y());
      return;
    }
    
    Gl_text_box::recompile_freetype = true;
    Gl_text_box::data_has_changed = true;
    
    for (const auto &character: section_resized)
    {
      if (this->characters.contains(character))
      {
        this->characters[character].size_x = (int) new_size.get_x();
        this->characters[character].size_y = (int) new_size.get_y();
      }
    }
  }
  
  void Gl_text_box::on_text_resize(float new_width, float new_height, const std::string &section_resized)
  {
    if (new_width < 0.0f || new_height < 0.0f || new_width > 500.0f || new_height > 500.0f)
    {
      alert(WAVE_LOG_ERROR, "[On text box resize] --> Attempted to resize text to (%.2f, %.2f)!",
            new_width, new_height);
      return;
    }
    
    Gl_text_box::recompile_freetype = true;
    Gl_text_box::data_has_changed = true;
    
    for (const auto &character: section_resized)
    {
      if (this->characters.contains(character))
      {
        this->characters[character].size_x = (int) new_width;
        this->characters[character].size_y = (int) new_height;
      }
    }
  }
  
  void Gl_text_box::on_recolor(const Color &new_color, const std::string &section_recolored)
  {
    for (const auto &character: section_recolored)
    {
      if (this->characters.contains(character))
      {
        if (this->characters.at(character).color == new_color) continue;
        this->characters[character].color = new_color;
        Gl_text_box::data_has_changed = true;
      }
    }
  }
  
  void Gl_text_box::on_move(const Math::Vector_2f &new_position)
  {
    if (this->format.offset == new_position) return;
    this->format.offset = new_position;
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::on_move(float new_x_coord, float new_y_coord)
  {
    if (this->format.offset == Math::Vector_2f(new_x_coord, new_y_coord)) return;
    this->format.offset = Math::Vector_2f(new_x_coord, new_y_coord);
    Gl_text_box::data_has_changed = true;
  }
  
  const Math::Vector_2f &Gl_text_box::get_pixel_size() const
  {
    return this->format.text_size;
  }
  
  Math::Vector_2f &Gl_text_box::get_pixel_size()
  {
    return this->format.text_size;
  }
  
  const std::string &Gl_text_box::get_text_string() const
  {
    return this->text;
  }
  
  const Text_format_s &Gl_text_box::get_text_format() const
  {
    return this->format;
  }
  
  const Math::Vector_2f &Gl_text_box::get_text_offset() const
  {
    return this->format.offset;
  }
  
  Math::Vector_2f &Gl_text_box::get_text_offset()
  {
    return this->format.offset;
  }
  
  const Color &Gl_text_box::get_text_box_color() const
  {
    return this->format.text_box_color;
  }
  
  Color &Gl_text_box::get_text_box_color()
  {
    return this->format.text_box_color;
  }
  
  const Color &Gl_text_box::get_uniform_text_color() const
  {
    return this->format.text_uniform_color;
  }
  
  Color &Gl_text_box::get_text_uniform_color()
  {
    return this->format.text_uniform_color;
  }
  
  const Color &Gl_text_box::get_text_color(char character) const
  {
    return this->characters.at(character).color;
  }
  
  const Math::Vector_2f &Gl_text_box::get_text_scale() const
  {
    return this->format.scale;
  }
  
  Math::Vector_2f &Gl_text_box::get_text_scale()
  {
    return this->format.scale;
  }
  
  const Glyph_s &Gl_text_box::get_character(uint8_t character_code)
  {
    return this->characters[character_code];
  }
  
  Texture *Gl_text_box::get_texture_atlas() const
  {
    return this->texture_atlas;
  }
  
  float Gl_text_box::get_text_length() const
  {
    float length = 0;
    for (const auto &character: this->text)
    {
      length += (float) this->characters.at(character).advance.get_x() * this->format.scale.get_x();
    }
    return length > 0 ? length : 0.0f;
  }
  
  const Math::Vector_2f &Gl_text_box::get_text_box_size() const
  {
    return this->format.box_size;
  }
  
  Math::Vector_2f &Gl_text_box::get_text_box_size()
  {
    return this->format.box_size;
  }
  
  const std::map<uint8_t, Glyph_s> &Gl_text_box::get_characters() const
  {
    return this->characters;
  }
  
  void Gl_text_box::set_character(uint8_t character_code, const Glyph_s &character)
  {
    this->characters[character_code] = character;
    if (this->text.find((int8_t) character_code) != std::string::npos) Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::set_text_format(const Text_format_s &format_)
  {
    if (this->format == format_) return;
    if (this->format.box_size != format_.box_size) on_box_resize(format_.box_size);
    if (this->format.text_size != format_.text_size) on_text_resize(format_.text_size, this->text);
    //TODO if (this->format.style != format_.style) on_text_restyle(format_.style, this->text);
    this->format = format_;
  }
  
  void Gl_text_box::set_text_offset_x(float offset_x)
  {
    on_move(Math::Vector_2f(offset_x, this->get_text_offset().get_y()));
  }
  
  void Gl_text_box::set_text_offset_y(float offset_y)
  {
    on_move(Math::Vector_2f(this->get_text_offset().get_x(), offset_y));
  }
  
  void Gl_text_box::set_text_offset(const Math::Vector_2f &offset_coords)
  {
    on_move(offset_coords);
  }
  
  void Gl_text_box::set_text_offset(float offset_x, float offset_y)
  {
    on_move(Math::Vector_2f(offset_x, offset_y));
  }
  
  void Gl_text_box::blend_text_color(char character, const Color &character_color)
  {
    this->characters[character].color += character_color;
  }
  
  void Gl_text_box::blend_text_color(const Color &uniform_color)
  {
    for (auto &character: this->text) this->characters.at(character).color += uniform_color;
  }
  
  void Gl_text_box::set_text_color(char character, const Color &color)
  {
    if (this->characters.contains(character) && this->characters.at(character).color == color) return;
    if (this->characters.contains(character))
    {
      this->characters[character].color = color;
      Gl_text_box::data_has_changed = true;
    }
  }
  
  void Gl_text_box::set_text_color(const Color &color)
  {
    on_recolor(color, this->text);
  }
  
  void Gl_text_box::set_text_scale(const Math::Vector_2f &scale_)
  {
    if (this->format.scale == scale_) return;
    this->format.scale = scale_;
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::append_text(const std::string &text_)
  {
    this->text += text_;
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::append_text(const std::string &text_, const Color &uniform_color)
  {
    this->text += text_;
    for (const char &character: text_) this->characters[character].color = uniform_color;
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::set_text_string(const std::string &text_)
  {
    if (this->text == text_) return;
    this->text = text_;
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::set_text_box_size(const Math::Vector_2f &size)
  {
    if (this->format.box_size == size) return;
    this->format.box_size = size;
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::set_pixel_size(const Math::Vector_2f &size)
  {
    if (this->format.text_size == size) return;
    on_text_resize(size, this->text);
    this->format.text_size = size;
  }
  
  void Gl_text_box::set_pixel_size(float x, float y)
  {
    if (this->format.text_size == Math::Vector_2f(x, y)) return;
    on_text_resize(Math::Vector_2f(x, y), this->text);
    this->format.text_size = Math::Vector_2f(x, y);
  }
  
  void Gl_text_box::set_text_box_color(const Color &color)
  {
    if (this->format.text_box_color == color) return;
    this->format.text_box_color = color;
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::set_text_uniform_color(const Color &uniform_color)
  {
    if (this->format.text_uniform_color == uniform_color) return;
    this->format.text_uniform_color = uniform_color;
    for (const auto &character: this->text)
    {
      if (this->characters.contains(character))
      {
        this->characters[character].color = uniform_color;
        Gl_text_box::data_has_changed = true;
      }
    }
  }
  
  const Math::Transform &Gl_text_box::get_text_transform() const
  {
    return this->text_transform;
  }
  
  Math::Transform &Gl_text_box::get_text_transform()
  {
    return this->text_transform;
  }
  
  const std::shared_ptr<Shader> &Gl_text_box::get_shader() const
  {
    return this->associated_shader;
  }
  
  const void *Gl_text_box::get_vertices() const
  {
    return this->glyph_vertices.data();
  }
  
  uint64_t Gl_text_box::get_vertex_count() const
  {
    return this->glyph_vertices.size();
  }
  
  void Gl_text_box::update_transform()
  {
    Gl_text_box::text_transform = Math::Transform(Math::Vector_3f(Gl_text_box::format.offset),
                                                  Math::Vector_3f(0.0f),
                                                  Math::Vector_3f(Gl_text_box::format.scale));
  }
  
  void Gl_text_box::set_transform(const Math::Transform &new_transform)
  {
    Gl_text_box::set_text_offset((Math::Vector_2f) new_transform.get_translation());
    Gl_text_box::set_text_scale((Math::Vector_2f) new_transform.get_scale());
    Gl_text_box::update_transform();
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
  
  void Gl_text_box::translate(const Math::Vector_3f &position)
  {
    this->format.offset += (Math::Vector_2f) position;
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::translate(float x, float y, [[maybe_unused]] float z)
  {
    this->format.offset += Math::Vector_2f(x, y);
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::rotate(const Math::Vector_3f &angle)
  {
    Math::Vector_3f temp_angle = angle;
    // Get angles and convert to radiant.
    temp_angle.set_x((float) (angle.get_x() * (M_PI / 180)));
    temp_angle.set_y((float) (angle.get_y() * (M_PI / 180)));
    temp_angle.set_z((float) (angle.get_z() * (M_PI / 180)));
    
    this->format.offset.set_x(cosf(temp_angle.get_x()) - sinf(temp_angle.get_y()));
    this->format.offset.set_y(cosf(temp_angle.get_y()) + sinf(temp_angle.get_x()));
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::rotate(float x, float y, [[maybe_unused]] float z)
  {
    // Get angles and convert to radiant.
    float angle_x = x * (float) (M_PI / 180), angle_y = y * (float) (M_PI / 180);
    
    this->format.offset.set_x(cosf(angle_x) - sinf(angle_y));
    this->format.offset.set_y(cosf(angle_y) + sinf(angle_x));
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::scale(const Math::Vector_3f &scalar)
  {
    this->format.scale *= (Math::Vector_2f) scalar;
    Gl_text_box::data_has_changed = true;
  }
  
  void Gl_text_box::scale(float x, float y, [[maybe_unused]] float z)
  {
    this->format.scale *= Math::Vector_2f(x, y);
    Gl_text_box::data_has_changed = true;
  }
  
  void *Gl_text_box::copy() const
  {
    std::shared_ptr<Text_box> copy = std::make_shared<Gl_text_box>(*this);
    return (void *) &(*copy);
  }
}