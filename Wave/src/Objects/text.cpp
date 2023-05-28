//
// Created by nami on 31/12/22.
//

#include <Objects/gl_text.h>
#include <Utilities/logger.h>
#include <Renderer/gl_renderer.h>
#include <Objects/text.h>


namespace Wave
{
  
  std::shared_ptr<Text> Text::create()
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text>();
      case Renderer_api::Vulkan:
        alert(WAVE_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text>();
      }
    }
  }
  
  std::shared_ptr<Text> Text::create(const std::string &string_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text>(string_);
      case Renderer_api::Vulkan:
        alert(WAVE_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text>(string_);
      }
    }
  }
  
  std::shared_ptr<Text> Text::create(const char *font_file_path, const std::string &string_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text>(font_file_path, string_);
      case Renderer_api::Vulkan:
        alert(WAVE_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text>(font_file_path, string_);
      }
    }
  }
  
  std::shared_ptr<Text> Text::create(const char *font_file_path, const std::string &string_, const Text_format &format_)
  {
    switch (Renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::OpenGL:return std::make_shared<Gl_text>(font_file_path, string_, format_);
      case Renderer_api::Vulkan:
        alert(WAVE_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:
      {
        Gl_renderer::gl_synchronous_error_callback(GL_DEBUG_SOURCE_API,
                                                   "Api not supported at the moment! Auto selecting OpenGL instead.",
                                                   "Text::create()",
                                                   "text.cpp", __LINE__ - 2);
        return std::make_shared<Gl_text>(font_file_path, string_, format_);
      }
    }
  }
  
  const std::string &Text::get_string() const
  {
    return this->string;
  }
  
  const Text_format &Text::get_format() const
  {
    return this->format;
  }
  
  float Text::get_offset_x() const
  {
    return this->format.offset_x;
  }
  
  float Text::get_offset_y() const
  {
    return this->format.offset_y;
  }
  
  const Color &Text::get_color() const
  {
    return this->format.color;
  }
  
  float Text::get_scale() const
  {
    return this->format.scale;
  }
  
  const FT_Face &Text::get_face() const
  {
    return this->face;
  }
  
  const Glyph &Text::get_character(uint8_t character_code)
  {
    return this->characters[character_code];
  }
  
  float Text::get_total_horizontal_text_size()
  {
    float total_size = this->format.offset_x;
    for (const auto &character: this->get_string())
    {
      total_size += this->characters[character].bearing.get_x() == 0 ? this->format.size :
                    this->characters[character].bearing.get_x() * this->format.scale;
    }
    total_size += this->format.offset_x;
    return total_size;
  }
  
  const std::map<uint8_t, Glyph> &Text::get_characters() const
  {
    return this->characters;
  }
  
  void Text::set_character(uint8_t character_code, const Glyph &character)
  {
    this->characters[character_code] = character;
  }
  
  Glyph &Text::operator[](uint8_t index)
  {
    return index < this->characters.size() ? this->characters[index] : this->characters[0];
  }
  
  void Text::set_format(const Text_format &format_)
  {
    this->format = format_;
  }
  
  void Text::set_offset_x(float offset)
  {
    this->format.offset_x = offset;
  }
  
  void Text::set_offset_y(float offset)
  {
    this->format.offset_y = offset;
  }
  
  void Text::set_color(const Color &color)
  {
    this->format.color = color;
  }
  
  void Text::set_scale(float scale_)
  {
    this->format.scale = scale_;
  }
}
