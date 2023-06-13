//
// Created by nami on 27/03/23.
//

#pragma once

#ifdef _MSVC
#define _USE_MATH_DEFINES  // For Visual Studio.
#endif

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

#define WAVE_GL_DEBUG_SOURCE_INVALID_UNIFORM 0x222
#define WAVE_GL_BUFFER_NOT_LOADED 0x223
#define WAVE_GL_INVALID_VIEWPORT_DIMENSIONS 0x224

/* ---------------------logger.h-------------------- */

#define WAVE_LOG_INFO  0
#define WAVE_LOG_WARN  1
#define WAVE_LOG_ERROR  2
#define WAVE_LOG_DEBUG  3
#define WAVE_LOG_STATUS_IN_PROGRESS  4
#define WAVE_LOG_INSTRUCTION_DONE  5
#define WAVE_LOG_INSTRUCTION_FAIL  6
#define WAVE_LOG_TASK_DONE  7
#define WAVE_LOG_TASK_FAIL  8

#define WAVE_INTERNAL_ERROR_HEAP_ALLOC  (-11)
#define WAVE_INTERNAL_ERROR_READ_LOG  (-12)
#define WAVE_INTERNAL_ERROR_READ_SHADER  (-13)
#define WAVE_INTERNAL_ERROR_SNPRINTF  (-14)
#define WAVE_INTERNAL_ERROR_VASPRINTF  (-15)
#define WAVE_INTERNAL_ERROR_LOCALTIME  (-16)

#define BIT(x) (1 << x)
#define BIND_EVENT_FUNCTION(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define WAVE_PROFILE_INSTRUCTIONS(title, color, id, instructions) \
      Wave::alert(WAVE_LOG_DEBUG, "[%s%s%s] [%s#%d%s] --> %s", CYAN, "Profile", DEFAULT, color, id, DEFAULT, title); \
      Wave::Timer timer; \
      timer.start(); \
      instructions; \
      timer.stop(); \
      Wave::alert(WAVE_LOG_DEBUG, "[%s%s%s] [%s#%d%s] --> %s took %.3f milliseconds", CYAN, "Profile", DEFAULT, \
      color, id, DEFAULT, title, timer.get_time_in_milli());

#ifndef DEBUG

#define WAVE_LOG_TASK(category, color, number, start_msg, event, end_msg)\
    event;

#define WAVE_LOG_INSTRUCTION(category, color, start_msg, instruction)\
    instruction;

#else

// Macro to log engine tasks and events to monitor app in debug mode (logger.h).
#define WAVE_LOG_TASK(category, color, number, start_msg, event, end_msg)\
      Wave::alert(WAVE_LOG_STATUS_IN_PROGRESS, "[%s] [%s#%d%s] --> %s", category, color, number, DEFAULT, start_msg); \
    event;\
    if (!Wave::Renderer::has_crashed())                                  \
    Wave::alert(WAVE_LOG_TASK_DONE, "[%s] [%s#%d%s] --> %s %sSuccessfully%s", category, color, number, DEFAULT, end_msg, GREEN, DEFAULT); \
    else Wave::alert(WAVE_LOG_TASK_FAIL, "[%s] [%s#%d%s] --> %s %sUnsuccessfully%s", category, color, number, DEFAULT, end_msg, RED, DEFAULT);\

#define WAVE_LOG_INSTRUCTION(category, color, start_msg, instruction)\
    Wave::alert(WAVE_LOG_STATUS_IN_PROGRESS, "[%s%s%s] --> %s", color, category, DEFAULT, start_msg); \
    instruction; \
    Wave::alert(WAVE_LOG_INSTRUCTION_DONE);
#endif  // Debug
