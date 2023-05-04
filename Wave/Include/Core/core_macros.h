//
// Created by nami on 27/03/23.
//

#pragma once

#define _USE_MATH_DEFINES  // For Visual Studio.

/* ---------------------engine.h-------------------- */

#define PROCESSING  1
#define ENGINE_CRASH  2
#define GAME_CRASH  5

/* ---------------------renderer.h-------------------- */

enum class Renderer_error_type
{
  NO_ERROR = 0,
  API_ERROR = 222,
  APPLICATION_ERROR,
  THIRD_PARTY_ERROR,
  WINDOW_SYSTEM_ERROR,
  SHADER_CREATION_ERROR,
  SHADER_COMPILATION_ERROR,
  INVALID_OPERATION,
  INVALID_ENUM,
  INVALID_VALUE,
  UNDEFINED_FRAMEBUFFER,
  INCOMPLETE_FRAMEBUFFER_ATTACHMENT,
  MISSING_FRAMEBUFFER_ATTACHMENT,
  INVALID_FRAMEBUFFER,
  INVALID_UNIFORM,
  OUT_OF_MEMORY,
  UNKNOWN_ERROR = -1
};

#define BIT(x) (1 << x)
#define BIND_EVENT_FUNCTION(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include <Utilities/logger.h>

#ifdef DEBUG
#define INTERFACE_PRINT  [[nodiscard]] std::string to_string() const override;\
                         void print([[maybe_unused]] Print_type type) const override                          \
                         {                                                    \
                          std::string printed(this->to_string());             \
                          if (type == Print_type::Default)     \
                            Wave::alert(WAVE_INFO, "%s", this->to_string().c_str());  \
                          else if (type == Print_type::Warn)   \
                            Wave::alert(WAVE_WARN, "%s", this->to_string().c_str()); \
                         else Wave::alert(WAVE_ERROR, "%s", this->to_string().c_str());  \
                         };
#else
  #ifdef RELEASE
    #define INTERFACE_PRINT  [[nodiscard]] std::string to_string() const override;\
                             void print([[maybe_unused]] Print_type type) const override                          \
                             {                                                    \
                             };
  #endif
#endif
