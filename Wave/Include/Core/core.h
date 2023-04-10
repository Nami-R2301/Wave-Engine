#pragma once

#include <Core/core_macros.h>
#include <memory>

#if defined(WAVE_PLATFORM_WINDOWS)
#define WAVE_API
#endif


/* ---------------------DEBUG MODE ON/OFF-------------------- */

#ifndef DEBUG

#define gl_call(x) x
#define glfw_call(x) x

#define log_task(category, color, number, start_msg, event, end_msg)\
    event;

#define log_instruction(category, color, start_msg, instruction)\
    instruction;

#else


/* ---------------------renderer.h-------------------- */

//#define RENDERER_ERROR_MAX_CHARS 100  // Documentation purposes --> Max chars to be printed out int the terminal on one line when logging.

// Macro to log openGL errors in an efficient and clean way (renderer.h).
#define gl_call(x) Wave::Gl_renderer::gl_clear_errors(); x; if (Wave::Gl_renderer::is_running()) Wave::Gl_renderer::gl_error_callback(glGetError(), #x, "No info", __FILE__, __LINE__)
#define glfw_call(x) x; Wave::Gl_window::glfw_error_callback(#x, __FILE__, __LINE__)

/* ---------------------logger.h-------------------- */

// Macro to log engine tasks and events to monitor app in debug mode (logger.h).
#define log_task(category, color, number, start_msg, event, end_msg)\
      Wave::alert(WAVE_STATUS_IN_PROGRESS, "[%s] [%s#%d%s] --> %s", category, color, number, DEFAULT, start_msg); \
    event;\
    Wave::alert(WAVE_TASK_DONE, "[%s] [%s#%d%s] --> %s %sSUCCESSFULLY%s", category, color, number, DEFAULT, end_msg, GREEN, DEFAULT); \

#define log_instruction(category, color, start_msg, instruction)\
    Wave::alert(WAVE_STATUS_IN_PROGRESS, "[%s%s%s] --> %s", color, category, DEFAULT, start_msg); \
    instruction; \
    Wave::alert(WAVE_INSTRUCTION_DONE);
#endif

namespace Wave
{
  template<typename T, typename ... Args>
  constexpr std::shared_ptr<T> create_shared_pointer(Args &&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }
}