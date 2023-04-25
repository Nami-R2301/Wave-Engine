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

/* ---------------------logger.h-------------------- */

#define WAVE_INFO  0
#define WAVE_WARN  1
#define WAVE_ERROR  2
#define WAVE_STATUS_IN_PROGRESS  3
#define WAVE_INSTRUCTION_DONE  4
#define WAVE_INSTRUCTION_FAIL  5
#define WAVE_TASK_DONE  6
#define WAVE_TASK_FAIL  7

#define ERROR_HEAP_ALLOC  (-11)
#define ERROR_READ_LOG  (-12)
#define ERROR_READ_SHADER  (-13)
#define ERROR_SNPRINTF  (-14)
#define ERROR_VASPRINTF  (-15)
#define ERROR_LOCALTIME  (-16)

#define BIT(x) (1 << x)
#define BIND_EVENT_FUNCTION(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

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