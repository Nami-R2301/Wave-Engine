//
// Created by nami on 5/16/22.
//

#pragma once

// Engine_time
#include <cstdarg>
#include <iostream>
#include <fstream>

namespace Wave
{

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
