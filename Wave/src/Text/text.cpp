//
// Created by nami on 31/12/22.
//

#include <Text/gl_text.h>
#include <Utilities/logger.h>
#include <Renderer/gl_renderer.h>
#include <Text/text.h>



namespace Wave
{
  
  std::shared_ptr<Text_box> Text_box::create()
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>();
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>();
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const std::shared_ptr<Shader> &associated_shader_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(associated_shader_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(associated_shader_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const Vector_2f &pixel_size,
                                             const std::shared_ptr<Shader> &associated_shader_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(pixel_size, associated_shader_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(pixel_size, associated_shader_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const Vector_2f &pixel_size_, const std::string &text_,
                                             const std::shared_ptr<Shader> &associated_shader_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:
        return std::make_shared<Gl_text_box>(pixel_size_, text_,
                                             associated_shader_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(pixel_size_, text_, associated_shader_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const std::string &string_,
                                             const std::shared_ptr<Shader> &associated_shader_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(string_, associated_shader_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(string_, associated_shader_);
      }
    }
  }
  
  std::shared_ptr<Text_box>
  Text_box::create(const Vector_2f &pixel_size_, const std::string &text_, const Text_format_s &format_,
                   const std::shared_ptr<Shader> &associated_shader_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:
        return std::make_shared<Gl_text_box>(pixel_size_, text_, format_,
                                             associated_shader_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(pixel_size_, text_, format_, associated_shader_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const Vector_2f &pixel_size_, const std::string &text_,
                                             const char *font_file_path_,
                                             const std::shared_ptr<Shader> &associated_shader_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:
        return std::make_shared<Gl_text_box>(pixel_size_, text_, font_file_path_,
                                             associated_shader_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(pixel_size_, text_, font_file_path_,
                                             associated_shader_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const char *font_file_path, const std::string &string_,
                                             const std::shared_ptr<Shader> &associated_shader_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:
        return std::make_shared<Gl_text_box>(font_file_path, string_,
                                             associated_shader_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(font_file_path, string_, associated_shader_);
      }
    }
  }
  
  std::shared_ptr<Text_box>
  Text_box::create(const char *font_file_path, const std::string &string_, const Text_format_s &format_,
                   const std::shared_ptr<Shader> &associated_shader_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:
        return std::make_shared<Gl_text_box>(font_file_path, string_, format_,
                                             associated_shader_);
      case Renderer_api::Vulkan:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text_box::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text_box>(font_file_path, string_, format_,
                                             associated_shader_);
      }
    }
  }
  
  /********************** EVENTS **************************/
  
  void Text_box::on_box_resize(const Vector_2f &new_size)
  {
    if (new_size.get_x() < 0.0f || new_size.get_y() < 0 || new_size.get_x() > 8000.0f || new_size.get_y() > 8000.0f)
    {
      alert(WAVE_LOG_ERROR, "[On text box resize] --> Attempted to resize text box to (%.2f, %.2f)!",
            new_size.get_x(), new_size.get_y());
      return;
    }
    
    if (this->get_pixel_size().get_y() > new_size.get_y())
      this->format.box_size += Vector_2f(0.0f, this->get_pixel_size().get_y() + 25.0f);
    
    // Append overflowing text to the bottom left of the box to mimic newlines.
    float length = 0;
    bool text_rearranged = false;
    float new_line_advance_offset;
    for (const auto &character: this->text)
    {
      if (length >= new_size.get_x())
      {
        text_rearranged = true;
        new_line_advance_offset = -length;
        length = this->characters.at(character).advance.get_x();
        if (this->characters.contains(character))
          this->characters.at(character).advance = Vector_2f(new_line_advance_offset,
                                                             this->get_pixel_size().get_y() + 25.0f);
      }
      if (this->characters.contains(character))
        length += (float) this->characters.at(character).advance.get_x() * this->format.scale.get_x();
    }
    if (text_rearranged) this->send_gpu();
  }
  
  void Text_box::on_box_resize(float new_width, float new_height)
  {
    if (new_width < 0.0f || new_height < 0.0f || new_width > 8000.0f || new_height > 8000.0f)
    {
      alert(WAVE_LOG_ERROR, "[On text box resize] --> Attempted to resize text box to (%.2f, %.2f)!",
            new_width, new_height);
      return;
    }
    
    if (this->get_text_length() > new_width)
      this->format.box_size += Vector_2f(0.0f, this->get_pixel_size().get_y() + 25.0f);
    
    // Append overflowing text to the bottom left of the box to mimic newlines.
    float length = 0;
    bool text_rearranged = false;
    float new_line_advance_offset;
    for (const auto &character: this->text)
    {
      if (length >= new_width)
      {
        new_line_advance_offset = -length;
        if (this->characters.contains(character))
        {
          text_rearranged = true;
          length = this->characters.at(character).advance.get_x();
          this->characters.at(character).advance = Vector_2f(new_line_advance_offset,
                                                             this->get_pixel_size().get_y() + 25.0f);
        }
      }
      if (this->characters.contains(character))
        length += (float) this->characters.at(character).advance.get_x() * this->format.scale.get_x();
    }
    if (text_rearranged) this->send_gpu();
  }
  
  void Text_box::on_text_resize(const Vector_2f &new_size, const std::string &section_resized)
  {
    if (new_size.get_x() < 0.0f || new_size.get_y() < 0.0f || new_size.get_x() > 500.0f || new_size.get_y() > 500.0f)
    {
      alert(WAVE_LOG_ERROR, "[On text box resize] --> Attempted to resize text to (%.2f, %.2f)!",
            new_size.get_x(), new_size.get_y());
      return;
    }
    
    for (const auto &character: section_resized)
    {
      if (this->characters.contains(character))
      {
        this->characters[character].size_x = (int) new_size.get_x();
        this->characters[character].size_y = (int) new_size.get_y();
      }
    }
    this->send_gpu();
  }
  
  void Text_box::on_text_resize(float new_width, float new_height, const std::string &section_resized)
  {
    if (new_width < 0.0f || new_height < 0.0f || new_width > 500.0f || new_height > 500.0f)
    {
      alert(WAVE_LOG_ERROR, "[On text box resize] --> Attempted to resize text to (%.2f, %.2f)!",
            new_width, new_height);
      return;
    }
    
    for (const auto &character: section_resized)
    {
      if (this->characters.contains(character))
      {
        this->characters[character].size_x = (int) new_width;
        this->characters[character].size_y = (int) new_height;
      }
    }
    this->send_gpu();
  }
  
  void Text_box::on_recolor(const Color &new_color, const std::string &section_recolored)
  {
    for (const auto &character: section_recolored)
    {
      if (this->characters.contains(character)) this->characters[character].color = new_color;
    }
  }
  
  void Text_box::on_move(const Vector_2f &new_position)
  {
    if (this->format.offset == new_position) return;
    this->format.offset = new_position;
  }
  
  void Text_box::on_move(float new_x_coord, float new_y_coord)
  {
    if (this->format.offset == Vector_2f(new_x_coord, new_y_coord)) return;
    this->format.offset = Vector_2f(new_x_coord, new_y_coord);
  }
  
  const Vector_2f &Text_box::get_pixel_size() const
  {
    return this->format.text_size;
  }
  
  Vector_2f &Text_box::get_pixel_size()
  {
    return this->format.text_size;
  }
  
  const std::string &Text_box::get_text_string() const
  {
    return this->text;
  }
  
  const Text_format_s &Text_box::get_text_format() const
  {
    return this->format;
  }
  
  const Vector_2f &Text_box::get_text_offset() const
  {
    return this->format.offset;
  }
  
  Vector_2f &Text_box::get_text_offset()
  {
    return this->format.offset;
  }
  
  const Color &Text_box::get_text_box_color() const
  {
    return this->format.text_box_color;
  }
  
  Color &Text_box::get_text_box_color()
  {
    return this->format.text_box_color;
  }
  
  const Color &Text_box::get_uniform_text_color() const
  {
    return this->format.text_uniform_color;
  }
  
  Color &Text_box::get_text_uniform_color()
  {
    return this->format.text_uniform_color;
  }
  
  const Color &Text_box::get_text_color(char character) const
  {
    return this->characters.at(character).color;
  }
  
  const Vector_2f &Text_box::get_text_scale() const
  {
    return this->format.scale;
  }
  
  Vector_2f &Text_box::get_text_scale()
  {
    return this->format.scale;
  }
  
  const Glyph_s &Text_box::get_character(uint8_t character_code)
  {
    return this->characters[character_code];
  }
  
  Texture *Text_box::get_texture_atlas() const
  {
    return this->texture_atlas;
  }
  
  float Text_box::get_text_length() const
  {
    float length = 0;
    for (const auto &character: this->text)
    {
      length += (float) this->characters.at(character).advance.get_x() * this->format.scale.get_x();
    }
    return length > 0 ? length : 0.0f;
  }
  
  const Vector_2f &Text_box::get_text_box_size() const
  {
    return this->format.box_size;
  }
  
  Vector_2f &Text_box::get_text_box_size()
  {
    return this->format.box_size;
  }
  
  const std::map<uint8_t, Glyph_s> &Text_box::get_characters() const
  {
    return this->characters;
  }
  
  void Text_box::set_character(uint8_t character_code, const Glyph_s &character)
  {
    this->characters[character_code] = character;
  }
  
  Glyph_s &Text_box::operator[](uint8_t index)
  {
    return index < this->characters.size() ? this->characters[index] : this->characters[0];
  }
  
  void Text_box::set_text_format(const Text_format_s &format_)
  {
    if (this->format == format_) return;
    if (this->format.box_size != format_.box_size) on_box_resize(format_.box_size);
    if (this->format.text_size != format_.text_size) on_text_resize(format_.text_size, this->text);
    //TODO if (this->format.style != format_.style) on_text_restyle(format_.style, this->text);
    this->format = format_;
  }
  
  void Text_box::set_text_offset_x(float offset_x)
  {
    on_move(Vector_2f(offset_x, this->get_text_offset().get_y()));
  }
  
  void Text_box::set_text_offset_y(float offset_y)
  {
    on_move(Vector_2f(this->get_text_offset().get_x(), offset_y));
  }
  
  void Text_box::set_text_offset(const Vector_2f &offset_coords)
  {
    on_move(offset_coords);
  }
  
  void Text_box::set_text_offset(float offset_x, float offset_y)
  {
    if (this->format.offset == Vector_2f(offset_x, offset_y)) return;
    this->format.offset = Vector_2f(offset_x, offset_y);
  }
  
  void Text_box::blend_text_color(char character, const Color &character_color)
  {
    this->characters[character].color += character_color;
  }
  
  void Text_box::blend_text_color(const Color &uniform_color)
  {
    for (auto &character: this->text) this->characters.at(character).color += uniform_color;
  }
  
  void Text_box::set_text_color(char character, const Color &color)
  {
    if (this->characters.contains(character) && this->characters.at(character).color == color) return;
    if (this->characters.contains(character)) this->characters[character].color = color;
  }
  
  void Text_box::set_text_color(const Color &color)
  {
    on_recolor(color, this->text);
  }
  
  void Text_box::set_text_scale(const Vector_2f &scale_)
  {
    if (this->format.scale == scale_) return;
    this->format.scale = scale_;
  }
  
  void Text_box::append_text(const std::string &text_)
  {
    this->text += text_;
  }
  
  void Text_box::append_text(const std::string &text_, const Color &uniform_color)
  {
    this->text += text_;
    for (const char &character: text_) this->characters[character].color = uniform_color;
  }
  
  void Text_box::set_text_string(const std::string &text_)
  {
    this->text = text_;
  }
  
  void Text_box::set_text_box_size(const Vector_2f &size)
  {
    this->format.box_size = size;
  }
  
  void Text_box::set_pixel_size(const Vector_2f &size)
  {
    if (this->format.text_size == size) return;
    on_text_resize(size, this->text);
    this->format.text_size = size;
  }
  
  void Text_box::set_pixel_size(float x, float y)
  {
    this->format.text_size = Vector_2f(x, y);
  }
  
  void Text_box::set_text_box_color(const Color &color)
  {
    this->format.text_box_color = color;
  }
  
  void Text_box::set_text_uniform_color(const Color &uniform_color)
  {
    if (this->format.text_uniform_color == uniform_color) return;
    this->format.text_uniform_color = uniform_color;
    for (const auto &character: this->text)
    {
      if (this->characters.contains(character)) this->characters[character].color = uniform_color;
    }
  }
  
  const Transform &Text_box::get_text_transform() const
  {
    return this->text_transform;
  }
  
  Transform &Text_box::get_text_transform()
  {
    return this->text_transform;
  }
  
  const std::shared_ptr<Shader> &Text_box::get_shader() const
  {
    return this->associated_shader;
  }
}
