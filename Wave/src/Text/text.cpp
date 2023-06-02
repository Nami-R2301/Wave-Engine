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
  
  std::shared_ptr<Text_box> Text_box::create(const Vector_2f &pixel_size)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(pixel_size);
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
        return std::make_shared<Gl_text_box>(pixel_size);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const Vector_2f &pixel_size_, const std::string &text_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(pixel_size_, text_);
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
        return std::make_shared<Gl_text_box>(pixel_size_, text_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const std::string &string_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(string_);
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
        return std::make_shared<Gl_text_box>(string_);
      }
    }
  }
  
  std::shared_ptr<Text_box>
  Text_box::create(const Vector_2f &pixel_size_, const std::string &text_, const Text_format_s &format_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(pixel_size_, text_, format_);
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
        return std::make_shared<Gl_text_box>(pixel_size_, text_, format_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const Vector_2f &pixel_size_, const std::string &text_,
                                             const char *font_file_path_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(pixel_size_, text_, font_file_path_);
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
        return std::make_shared<Gl_text_box>(pixel_size_, text_, font_file_path_);
      }
    }
  }
  
  std::shared_ptr<Text_box> Text_box::create(const char *font_file_path, const std::string &string_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(font_file_path, string_);
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
        return std::make_shared<Gl_text_box>(font_file_path, string_);
      }
    }
  }
  
  std::shared_ptr<Text_box>
  Text_box::create(const char *font_file_path, const std::string &string_, const Text_format_s &format_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_LOG_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text_box>(font_file_path, string_, format_);
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
        return std::make_shared<Gl_text_box>(font_file_path, string_, format_);
      }
    }
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
  
  const Color &Text_box::get_text_box_color() const
  {
    return this->format.text_box_color;
  }
  
  Color &Text_box::get_text_box_color()
  {
    return this->format.text_box_color;
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
    float length = this->format.offset.get_x() * 2;  // Add padding on both sides.
    for (const auto &character: this->text)
    {
      length += (float) this->characters.at(character).advance.get_x() * this->format.scale.get_x();
    }
    return length;
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
    this->format = format_;
  }
  
  void Text_box::set_text_offset_x(float offset_x)
  {
    this->format.offset.set_x(offset_x);
  }
  
  void Text_box::set_text_offset_y(float offset_y)
  {
    this->format.offset.set_y(offset_y);
  }
  
  void Text_box::set_text_offset(const Vector_2f &offset_coords)
  {
    this->format.offset = offset_coords;
  }
  
  void Text_box::set_text_offset(float offset_x, float offset_y)
  {
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
    this->characters[character].color = color;
  }
  
  void Text_box::set_text_color(const Color &color)
  {
    for (auto &character: this->text) this->characters.at(character).color = color;
  }
  
  void Text_box::set_text_scale(const Vector_2f &scale_)
  {
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
}
