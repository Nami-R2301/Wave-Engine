//
// Created by nami on 5/11/22.
//

#include <Core/engine_time.h>

namespace Wave
{
  
  void Engine_time::update_engine_run_time()
  {
    this->current_time = std::chrono::high_resolution_clock::now();
  }
  
  void Engine_time::set_previous_engine_time(const std::chrono::high_resolution_clock::time_point &previous_time_)
  {
    this->previous_time = previous_time_;
  }
  
  time_t Engine_time::get_real_time()
  {
    auto current_time = time(nullptr);
    return current_time;
  }
  
  float Engine_time::get_previous_engine_time() const
  {
    auto time_zero = std::chrono::high_resolution_clock::time_point();
    return std::chrono::duration<float>(this->previous_time - time_zero).count();
  }
  
  float Engine_time::get_up_time() const
  {
    return std::chrono::duration<float>(this->current_time - this->previous_time).count();
  }
}
