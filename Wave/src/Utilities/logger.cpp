//
// Created by nami on 5/16/22.
//

#include <Utilities/logger.h>

#ifndef ASPRINTF_H
#define ASPRINTF_H

/*
 * vscprintf:
 * MSVC implements this as _vscprintf, thus we just 'symlink' it here
 * GNU-C-compatible compilers do not implement this, thus we implement it here
 */
#ifdef _MSC_VER
#ifndef vscprintf
#define vscprintf _vscprintf
#endif

 /*
  * asprintf, vasprintf:
  * MSVC does not implement these, thus we implement them here
  * GNU-C-compatible compilers implement these with the same names, thus we
  * don't have to do anything
  */
#ifndef vasprintf
int vasprintf(char** strp, const char* format, va_list ap)
{
    int len = vscprintf(format, ap);
    if (len == -1)
    {
        return -1;
    }
    char* str = (char*)malloc((size_t)len + 1);
    if (!str)
    {
        return -1;
    }
    int retval = vsnprintf(str, static_cast<size_t>(len) + 1, format, ap);
    if (retval == -1)
    {
        free(str);
        return -1;
    }
    *strp = str;
    return retval;
}
#endif  // VASPRINTF
#ifndef asprintf
int asprintf(char** strp, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    int retval = vasprintf(strp, format, ap);
    va_end(ap);
    return retval;
}
#endif  // ASPRINTF
#endif  // _MSC_VER
#endif // ASPRINTF_H

namespace Wave
{
  static FILE *file_ptr = nullptr;
  static std::ofstream log_stream;
  static std::streambuf *err_buf = nullptr;
  
  void open_stream()
  {
    if (log_stream.is_open()) return;
    log_stream.open("wave_engine.log", std::ios_base::app);
    if (!log_stream.is_open()) log_stream.open("wave_engine.log");
    if (!log_stream)
    {
      std::cerr << "ERROR OPENING LOG FILE!\tMAKE SURE YOU HAVE THE REQUIRED PERMISSIONS IN"
                   " THE CURRENT DIRECTORY!" << std::endl;
    }
    
    err_buf = std::cerr.rdbuf(); // Save standard out buffer.
    std::cerr.rdbuf(log_stream.rdbuf()); // Redirect std::cout to log.txt!
  }
  
  void print_logs()
  {
    if (!file_ptr) file_ptr = fopen("wave_engine.log", "r");
    if (!file_ptr)
    {
      std::cout << "NO LOGS DOCUMENTED YET!\tTRY LAUNCHING THE PROGRAM AT LEAST ONCE..." << std::endl;
      return;
    }
    if (fseek(file_ptr, 0, SEEK_END) < 0)  // Get last character position in file.
    {
      std::cerr << "ERROR SEEKING CURSOR POSITION IN LOG FILE\tMAKE SURE FILE IS IN TEXT FORMAT!" << std::endl;
      return;
    }
    long file_size = ftell(file_ptr) + 1;  // use the last position as size.
    if (file_size == 1) return;
    char *file_contents = (char *) calloc(1, file_size);
    if (fseek(file_ptr, 0, SEEK_SET) < 0)  // Get last character position in file.
    {
      std::cerr << "ERROR SEEKING CURSOR POSITION IN LOG FILE\tMAKE SURE FILE IS IN TEXT FORMAT!" << std::endl;
      return;
    }
    if (file_contents)
    {
      if (!fread(file_contents, 1, file_size, file_ptr))
      {
        std::cout << "NO LOGS DOCUMENTED YET!\tTRY LAUNCHING THE PROGRAM AT LEAST ONCE..." << std::endl;
        free(file_contents);
        fclose(file_ptr);
        return;
      }
    }
    
    fflush(stdout);
    
    std::cout << "\n" << GREEN << "------------START OF LOGS--------------\n" << DEFAULT << file_contents << DEFAULT;
    std::cout << "\n" << GREEN << "------------END OF LOGS--------------" << DEFAULT << std::endl;
    free(file_contents);
    fclose(file_ptr);
  }
  
  void reset_logs()
  {
    if (!file_ptr) file_ptr = fopen("wave_engine.log", "r");
    int32_t result = 0;
    if (file_ptr) result = remove("wave_engine.log");
    fflush(stdout);
    if (result == 0)
    {
      std::cout << "LOG FILE RESET!\tPLEASE RUN THE PROGRAM AGAIN TO VIEW LOGS..." << std::endl;
    } else
    {
      std::cerr << "[LOG ERROR] : CAN'T RESET LOG FILE!\tMAKE SURE YOU RAN THE PROGRAM AT LEAST ONCE..." << std::endl;
    }
    fclose(file_ptr);
  }

#if defined(DEBUG)
  
  void alert(int info_type, const char *format, ...)
  {
    if (!log_stream.is_open()) return;
    va_list args;
    va_start(args, format);
    
    char *string;
    char current_time[sizeof(args) + (FILENAME_MAX * 4)];
    if (vasprintf(&string, format, args) < 0)
    {
      std::cerr << "ERROR : " << WAVE_INTERNAL_ERROR_VASPRINTF << std::endl;
      free(string);
      return;
    }
    auto time = Engine_time::get_real_time();  // Get std::time_t struct.
    struct tm time_info{};
#ifdef _MSC_VER
    if (localtime_s(&time_info, &time)) exit(WAVE_INTERNAL_ERROR_LOCALTIME);
#endif
#ifdef _POSIX_VERSION
    if (!localtime_r(&time, &time_info))
    {
        std::cerr << "ERROR : " << ERROR_LOCALTIME << std::endl;
        free(string);
        return;
    }
    size_t chars_written = strftime(current_time, FILENAME_MAX, "%c", &time_info);
#elif defined(__unix__)  // Non POSIX UNIX systems.
    struct tm* time_info_non_posix = localtime(&time);
    strftime(current_time, FILENAME_MAX, "%c", time_info_non_posix);
#endif

    uint64_t chars_written = strftime(current_time, FILENAME_MAX * 4, "%c", &time_info);
    
    int64_t max_size = FILENAME_MAX;
    auto data_size = static_cast<int64_t>(strlen(string) + chars_written);
    // Avoid allocating a huge amount of memory to account for large inputs by adjusting to the right size.
    if (max_size < data_size) max_size = data_size;
    char *buffer = (char *) calloc(max_size + chars_written, sizeof(char));
    int snprintf_result;
    
    switch (info_type)
    {
      case WAVE_LOG_INFO:
        
        snprintf_result = snprintf(buffer, max_size, "%s[INFO]%4s [%s] :%10s", DEFAULT,
                                   " ", strlen(current_time) > 1 ? current_time
                                                                 :
                                        "\033[31mERROR WHILE TRYING TO FETCH LOCALTIME!\033[0m", DEFAULT);
        break;
      case WAVE_LOG_STATUS_IN_PROGRESS:
        snprintf_result = snprintf(buffer, max_size, "%s[PROGRESS] [%s] :%10s", PURPLE,
                                   strlen(current_time) > 1 ? current_time
                                                            : "\033[31mERROR WHILE TRYING TO FETCH LOCALTIME!\033[0m",
                                   DEFAULT);
        break;
      case WAVE_LOG_INSTRUCTION_DONE:snprintf_result = snprintf(buffer, max_size, " -->%10sDONE%s", GREEN, DEFAULT);
        break;
      case WAVE_LOG_INSTRUCTION_FAIL:snprintf_result = snprintf(buffer, max_size, " -->%10sFAIL%s", RED, DEFAULT);
        break;
      case WAVE_LOG_TASK_DONE:
        snprintf_result = snprintf(buffer, max_size, "%s[DONE]%4s [%s] :%10s", GREEN, " ",
                                   strlen(current_time) > 1 ? current_time : "ERROR WHILE TRYING TO FETCH LOCALTIME!",
                                   DEFAULT);
        break;
      case WAVE_LOG_TASK_FAIL:
        snprintf_result = snprintf(buffer, max_size, "%s[FAIL]%4s [%s] :%10s", RED, " ",
                                   strlen(current_time) > 1 ? current_time
                                                            : "\033[31mERROR WHILE TRYING TO FETCH LOCALTIME!\033[0m",
                                   DEFAULT);
        break;
      case WAVE_LOG_WARN:
        snprintf_result = snprintf(buffer, max_size, "%s[WARN]%4s [%s] :%10s", YELLOW, " ",
                                   current_time, DEFAULT);
        break;
      case WAVE_LOG_ERROR:
        snprintf_result = snprintf(buffer, max_size, "%s[ERROR]%3s [%s] :%10s", RED, " ",
                                   strlen(current_time) > 1 ? current_time
                                                            : "\033[31mERROR WHILE TRYING TO FETCH LOCALTIME!\033[0m",
                                   DEFAULT);
        break;
      case WAVE_LOG_DEBUG:
        snprintf_result = snprintf(buffer, max_size, "%s[DEBUG]%3s [%s] :%10s", CYAN, " ",
                                   strlen(current_time) > 1 ? current_time
                                                            : "\033[31mERROR WHILE TRYING TO FETCH LOCALTIME!\033[0m",
                                   DEFAULT);
        break;
      default:
        snprintf_result = snprintf(buffer, max_size,
                                   "\n%s[ERROR]%-3s [%s] :\t\t UNEXPECTED ERROR OCCURRED...\tSEE LOGS FOR MORE INFO",
                                   RED, " ", strlen(current_time) > 1 ? current_time
                                                                      :
                                             "\033[31mERROR WHILE TRYING TO FETCH LOCALTIME!\033[0m");
        break;
    }
    if (snprintf_result < 0)
    {
      std::cerr << "ERROR : " << WAVE_INTERNAL_ERROR_SNPRINTF << std::endl;
      free(string);
      return;
    }
    
    const char *output;
    // Separate the buffer with the actual message logged in order to let operator overloading take effect for strings
    // passed in (i.e. events).
    output = strcat(buffer, string);
    if (info_type == WAVE_LOG_INSTRUCTION_DONE || info_type == WAVE_LOG_INSTRUCTION_FAIL)
    {
      std::cout << output;
      log_stream << output;
    } else
    {
      std::cout << std::endl << output;
      log_stream << std::endl << output;  // Save to log file.
    }
    fflush(stdout);
    
    va_end(args);
    free(string);
    free(buffer);
  }

#else
  void alert([[maybe_unused]] int info_type, [[maybe_unused]] const char *format, ...)
  {
  }
#endif
  
  void close_stream()
  {
    std::cout << std::endl;
    if (log_stream.is_open())
    {
      log_stream << std::endl;
      log_stream.close();
    }
    std::cerr.rdbuf(err_buf); // Reset to standard output again
  }
}