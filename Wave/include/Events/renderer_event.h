//
// Created by nami on 30/03/23.
//

#pragma once

#include <Events/event.h>
#include <Core/interfaces.h>

namespace Wave
{
  
  enum class Renderer_api
  {
    None = 0,
    Opengl = 1,
    Vulkan = 2,
    Directx = 3
  };
  
  typedef struct Renderer_state
  {
    const char *type;
    const char *severity;
    const char *description;
    int32_t code;
  } Renderer_state;
  
  class On_renderer_error : public Event
  {
    public:
    explicit On_renderer_error(Renderer_state state_, Renderer_api api_);
    ~On_renderer_error() override = default;
    
    [[nodiscard]] Renderer_state get_renderer_state() const;
    [[nodiscard]] Renderer_api get_renderer_api() const;
    
    EVENT_CLASS_TYPE(Event_type::On_renderer_error);
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_RENDERER);
    
    INTERFACE_PRINT
    private:
    Renderer_state state{};
    Renderer_api api{};
  };
}
