//
// Created by nami on 31/12/22.
//

#include <Objects/gl_text.h>
#include <Utilities/logger.h>
#include <Renderer/renderer.h>
#include <Objects/text.h>


namespace Wave
{
  
  const std::string &Text::get_string() const
  {
    return this->text;
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
  
  const std::map<uint8_t, Glyph> &Text::get_characters() const
  {
    return this->characters;
  }
  
  void Text::set_character(uint8_t character_code, const Glyph &character)
  {
    this->characters[character_code] = character;
  }
  
  std::shared_ptr<Text> Text::create(const char *font_file_path, const std::string &text_, const Text_format& format_)
  {
    switch (Gl_renderer::get_api())
    {
      case Renderer_api::None:
        alert(WAVE_ERROR, "[BUFFER] --> None is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Opengl:return create_shared_pointer<Gl_text>(font_file_path, text_, format_);
      case Renderer_api::Vulkan:
        alert(WAVE_ERROR, "[BUFFER] --> Vulkan is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      case Renderer_api::Directx:
        alert(WAVE_ERROR, "[BUFFER] --> DirectX is currently not supported! (on line %d in file %s) !",
              __LINE__, __FILE__);
        return nullptr;
      default:return nullptr;
    }
  }
  
  Glyph &Text::operator [](uint8_t index)
  {
    return index < this->characters.size() ? this->characters[index] : this->characters[0];
  }
}
