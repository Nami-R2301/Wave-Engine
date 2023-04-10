//
// Created by nami on 5/16/22.
//

#pragma once

// Engine_time
#include <Core/core.h>
#include <Core/engine_time.h>
#include <wave_pch.h>

namespace Wave
{
  
  #ifndef COLORLESS
  // The following are ANSI terminal color codes.
#define DEFAULT  "\033[0m"
#define RED  "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN  "\033[32m"
#define CYAN  "\033[0;36m"
#define PURPLE  "\033[0;35m"
  #else
    #define DEFAULT  ""
#define RED  ""
#define YELLOW  ""
#define GREEN  ""
#define CYAN  ""
#define PURPLE  ""
  #endif
  
  void open_stream();
  
  void alert(int info_type = WAVE_INFO, const char *format = "", ...);
  
  void print_logs();
  
  void reset_logs();
  
  void close_stream();
}
