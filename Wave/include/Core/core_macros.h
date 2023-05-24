//
// Created by nami on 27/03/23.
//

#pragma once

#define _USE_MATH_DEFINES  // For Visual Studio.

/* ---------------------engine.h-------------------- */

#define WAVE_VALUE_DONT_CARE (-1)
#define WAVE_ENGINE_NO_ERRORS 0x000
#define WAVE_MULTIPLE_INSTANCES 0x001
#define WAVE_ENGINE_RENDERER_CRASH  0x010
#define WAVE_ENGINE_CONTEXT_CRASH  0x100

/* ---------------------renderer.h-------------------- */

#define WAVE_RENDERER_INIT 0x11
#define WAVE_RENDERER_RUNNING 0x12
#define WAVE_RENDERER_SHUTDOWN 0x1F

#define GL_DEBUG_SOURCE_INVALID_UNIFORM 0x222
#define GL_INVALID_VIEWPORT_DIMENSIONS 0x223

/* ---------------------logger.h-------------------- */

#define WAVE_INFO  0
#define WAVE_WARN  1
#define WAVE_ERROR  2
#define WAVE_DEBUG  3
#define WAVE_STATUS_IN_PROGRESS  4
#define WAVE_INSTRUCTION_DONE  5
#define WAVE_INSTRUCTION_FAIL  6
#define WAVE_TASK_DONE  7
#define WAVE_TASK_FAIL  8

#define ERROR_HEAP_ALLOC  (-11)
#define ERROR_READ_LOG  (-12)
#define ERROR_READ_SHADER  (-13)
#define ERROR_SNPRINTF  (-14)
#define ERROR_VASPRINTF  (-15)
#define ERROR_LOCALTIME  (-16)

#define BIT(x) (1 << x)
#define BIND_EVENT_FUNCTION(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define WAVE_PROFILE_INSTRUCTIONS(title, color, id, instructions) \
      Wave::alert(WAVE_DEBUG, "[%s%s%s] [%s#%d%s] --> %s", CYAN, "Profile", DEFAULT, color, id, DEFAULT, title); \
      Wave::Timer timer; \
      timer.start(); \
      instructions; \
      timer.stop(); \
      Wave::alert(WAVE_DEBUG, "[%s%s%s] [%s#%d%s] --> %s took %.3f milliseconds", CYAN, "Profile", DEFAULT, \
      color, id, DEFAULT, title, timer.get_time_in_milli());

#ifndef DEBUG

#define WAVE_LOG_TASK(category, color, number, start_msg, event, end_msg)\
    event;

#define WAVE_LOG_INSTRUCTION(category, color, start_msg, instruction)\
    instruction;

#else

// Macro to log engine tasks and events to monitor app in debug mode (logger.h).
#define WAVE_LOG_TASK(category, color, number, start_msg, event, end_msg)\
      Wave::alert(WAVE_STATUS_IN_PROGRESS, "[%s] [%s#%d%s] --> %s", category, color, number, DEFAULT, start_msg); \
    event;\
    if (!Wave::Renderer::has_crashed())                                  \
    Wave::alert(WAVE_TASK_DONE, "[%s] [%s#%d%s] --> %s %sSuccessfully%s", category, color, number, DEFAULT, end_msg, GREEN, DEFAULT); \
    else Wave::alert(WAVE_TASK_FAIL, "[%s] [%s#%d%s] --> %s %sUnsuccessfully%s", category, color, number, DEFAULT, end_msg, RED, DEFAULT);\

#define WAVE_LOG_INSTRUCTION(category, color, start_msg, instruction)\
    Wave::alert(WAVE_STATUS_IN_PROGRESS, "[%s%s%s] --> %s", color, category, DEFAULT, start_msg); \
    instruction; \
    Wave::alert(WAVE_INSTRUCTION_DONE);
#endif  // Debug

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
