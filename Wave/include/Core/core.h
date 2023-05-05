#pragma once

#include <Core/platform_detection.h>
#include <Core/core_macros.h>
#include <memory>

/* ---------------------renderer.h-------------------- */

//#define RENDERER_ERROR_MAX_CHARS 100  // Documentation purposes --> Max chars to be printed out int the terminal on one line when logging.

namespace Wave
{
  template<typename T, typename ... Args>
  constexpr std::shared_ptr<T> create_shared_pointer(Args &&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }
}