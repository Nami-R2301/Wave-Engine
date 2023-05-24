#pragma once

#include <wave_pch.h>

namespace Wave
{
  
  using namespace std::chrono;
  
  typedef struct Timer
  {
    private:
    high_resolution_clock::time_point start_time = high_resolution_clock::time_point();
    high_resolution_clock::time_point end_time = high_resolution_clock::time_point();
    
    public:
    void start()
    {
      start_time = high_resolution_clock::now();
    }
    
    void stop()
    {
      end_time = high_resolution_clock::now();
    }
    
    [[nodiscard]] float get_time_in_seconds()
    {
      return duration<float>(end_time - start_time).count();
    }
    
    [[nodiscard]] float get_time_in_milli()
    {
      return duration<float>(end_time - start_time).count() * 1000.0f;
    }
    
    [[nodiscard]] float get_time_in_micro()
    {
      return duration<float>(end_time - start_time).count() * 1'000'000.0f;
    }
    
    [[nodiscard]] float get_time_in_nano()
    {
      return duration<float>(end_time - start_time).count() * 1'000'000'000.0f;
    }
  } Timer;
  
  class Engine_time
  {
    public:
    static time_t get_real_time();
    [[nodiscard]] float get_up_time() const;
    [[nodiscard]] float get_previous_engine_time() const;
    void update_engine_run_time();
    
    void set_previous_engine_time(const std::chrono::high_resolution_clock::time_point &previous_time_);
    private:
    std::chrono::high_resolution_clock::time_point previous_time;
    std::chrono::high_resolution_clock::time_point current_time;
  };
}