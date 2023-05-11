#include <Core/engine.h>

namespace Wave
{
  
  Engine *Engine::instance = nullptr;
  std::unique_ptr<Window> Engine::main_window = nullptr;
  float Engine::engine_framerate = 60.0f;
  long Engine::frame_drawn_counter = 0;
  bool Engine::running_state = false;
  Engine_time Engine::current_time;
  int32_t Engine::exit_code = WAVE_ENGINE_NO_ERRORS;
  [[maybe_unused]] Vector_2f Engine::last_mouse_position = Vector_2f(0);
//  static std::mutex meshes_mutex;  // For async purposes.
  
  Engine::Engine()
  {
#if defined(DEBUG)
    Wave::open_stream();  // Open log file stream.
#endif
    if (Engine::instance)
    {
      alert(WAVE_ERROR,
            "[Engine] --> App already exists! Make sure to close any instance of Wave Engine before trying again.");
    }
    Engine::instance = this;
    LOG_TASK("ENGINE", RED, 1, "--------- Launching Wave Engine ---------",
             {
               this->current_time.set_previous_engine_time(std::chrono::high_resolution_clock::now());
               Renderer::create(Renderer_api::Opengl);  // Default to OpenGL implementation.
               Renderer::set_event_callback_function(BIND_EVENT_FUNCTION(on_event));
               
               // Setup context api settings.
               Engine::main_window = Window::create(Context_api::Glfw);  // Default to OpenGL implementation.
               // Set default callbacks.
               Engine::main_window->set_event_callback_function(BIND_EVENT_FUNCTION(on_event));
               Engine::main_window->bind_api_callbacks();
               Renderer::init();
             },
             "Engine launched")
  }
  
  Engine::Engine(Renderer_api choice, Context_api api_choice)
  {
#if defined(DEBUG)
    Wave::open_stream();  // Open log file stream.
#endif
    if (Engine::instance)
    {
      alert(WAVE_ERROR,
            "[Engine] --> App already exists! Make sure to close any instance of Wave Engine before trying again.");
    }
    Engine::instance = this;
    LOG_TASK("ENGINE", RED, 1, "--------- Launching Wave Engine ---------",
             {
               this->current_time.set_previous_engine_time(std::chrono::high_resolution_clock::now());
               Renderer::create(Renderer_api::Opengl);  // Default to OpenGL implementation.
               Renderer::set_event_callback_function(BIND_EVENT_FUNCTION(on_event));
               
               switch (choice)
               {
                 case Renderer_api::Opengl:
                   // Setup context api settings.
                   Engine::main_window = Window::create(api_choice);
                   // Set default callbacks.
                   Engine::main_window->set_event_callback_function(BIND_EVENT_FUNCTION(on_event));
                   Engine::main_window->bind_api_callbacks();
                   break;
                 case Renderer_api::Vulkan:
                   alert(WAVE_WARN,
                         "Engine does NOT support Vulkan yet!\tRenderer set to OpenGL instead.");
                   // Setup render api settings.
                   Engine::main_window = Window::create(api_choice);
                   // Set default callbacks.
                   Engine::main_window->set_event_callback_function(BIND_EVENT_FUNCTION(on_event));
                   Engine::main_window->bind_api_callbacks();
                   break;
                 case Renderer_api::Directx:
                   alert(WAVE_WARN,
                         "Engine does NOT support DirectX yet!\tRenderer set to OpenGL instead.");
                   // Setup render api settings.
                   Engine::main_window = Window::create(api_choice);
                   // Set default callbacks.
                   Engine::main_window->set_event_callback_function(BIND_EVENT_FUNCTION(on_event));
                   Engine::main_window->bind_api_callbacks();
                   break;
                 default:Engine::main_window = Window::create(api_choice);
                   // Set default callbacks.
                   Engine::main_window->set_event_callback_function(BIND_EVENT_FUNCTION(on_event));
                   Engine::main_window->bind_api_callbacks();
               }
               Renderer::init();
             },
             "Engine launched")
  }
  
  Engine::~Engine()
  {
    LOG_TASK("ENGINE", RED, 1, "--------- Shutting down Wave Engine ---------",
             {
               if (this->has_crashed())
               {
                 alert(WAVE_ERROR, "[ENGINE] --> Engine exited with error code : %s0x%X%s ...",
                       RED, get_exit_status(), DEFAULT);
               }
               this->shutdown();
             },
             "Engine shut down")
#if defined(DEBUG)
    Wave::close_stream();  // Close log file stream.
#endif
  }
  
  Engine *Engine::get_app()
  {
    return Engine::instance;
  }
  
  Window *Engine::get_main_window()
  {
    return Engine::main_window.get();
  }
  
  float Engine::get_engine_framerate()
  {
    return Engine::engine_framerate;
  }
  
  long Engine::get_frame_drawn_counter()
  {
    return Engine::frame_drawn_counter;
  }
  
  bool Engine::get_running_state()
  {
    return Engine::running_state;
  }
  
  int32_t Engine::get_exit_status()
  {
    return Engine::exit_code;
  }
  
  void Engine::set_engine_framerate(float time_step)
  {
    Engine::engine_framerate = 1.0f / time_step;  // Get frame time in secs.
  }
  
  void Engine::set_frame_drawn_counter(long counter)
  {
    Engine::frame_drawn_counter = counter;
  }
  
  void Engine::set_running_state(bool new_state_)
  {
    Engine::running_state = new_state_;
  }
  
  void Engine::set_exit_status(int32_t code)
  {
    Engine::exit_code |= code;  // Stack errors together if multiple are detected.
  }
  
  void Engine::push_overlay(Layer *layer)
  {
    this->layer_stack.push_overlay(layer);
    layer->on_attach();
  }
  
  void Engine::pop_overlay(Layer *layer)
  {
    this->layer_stack.pop_overlay(layer);
    layer->on_detach();
  }
  
  void Engine::push_layer(Layer *layer)
  {
    this->layer_stack.push_layer(layer);
    layer->on_attach();
  }
  
  void Engine::pop_layer(Layer *layer)
  {
    this->layer_stack.pop_layer(layer);
    layer->on_detach();
  }
  
  void Engine::init()
  {
    Timer init_time;
    init_time.start();
    
    // Default dark mode-like background.
    Renderer::set_clear_color(Wave::Color(0.03f, 1.0f, true));
    
    auto cube = std::make_unique<Cube>(Res_loader_3D("cube.obj").load_3D_mesh());
    Renderer::load_object(cube.get());
    
    init_time.stop();
    alert(WAVE_INFO, "Time spent for initialization : %.3f ms", init_time.get_time_in_mili());
    Engine::current_time.set_previous_engine_time(std::chrono::high_resolution_clock::now());
    Engine::set_exit_status(static_cast<int32_t>(Renderer::get_state().code));
  }
  
  void Engine::run()
  {
    Engine::set_running_state(true);
    
    float last_frame_time = 0.0f;
    float time_step;  // Avoid tying game speed to framerate.
    Timer draw_time;
    draw_time.start();
    while (!Engine::main_window->is_closing() && Renderer::is_running())
    {
      Engine::current_time.update_engine_run_time();
      float current_run_time = Engine::current_time.get_up_time();
      time_step = current_run_time - last_frame_time;
      last_frame_time = current_run_time;
      this->set_engine_framerate(time_step);
      
      on_update(time_step);  // Overriden on_update function gets called here.
      draw_time.stop();
      // Show how many fps were achieved if a second passed or if we rendered enough frames before the second passed.
      if (draw_time.get_time_in_seconds() >= 1.0f)
      {
        char title[33];
        alert(WAVE_INFO, "[ENGINE] --> Framerate : %ld", get_frame_drawn_counter());
        if (snprintf(title, 33, "Wave Engine | OpenGL | %ld FPS", get_frame_drawn_counter()) < 0) return;
        Engine::main_window->set_title(title);
        Engine::set_frame_drawn_counter(0);
        draw_time.start();
      }
    }
    // In case we call shutdown directly and the renderer is shutdown already.
    if (Engine::running_state && !Renderer::is_running())
    {
      Engine::set_exit_status(WAVE_ENGINE_RENDERER_CRASH);
      return;
    }
    if (!Engine::main_window->is_closing()) Engine::set_exit_status(WAVE_ENGINE_CONTEXT_CRASH);
  }
  
  void Engine::on_event(Event &event)
  {
    Event_dispatcher event_dispatcher(event);
    switch (event.get_event_type())
    {
      case Event_type::None:return;
      case Event_type::On_window_close:
      {
        event_dispatcher.dispatch<Wave::On_window_close>(BIND_EVENT_FUNCTION(window_closed_callback));
        break;
      }
      case Event_type::On_window_resize:
      {
        event_dispatcher.dispatch<Wave::On_window_resize>(BIND_EVENT_FUNCTION(window_resize_callback));
        break;
      }
      default:break;
    }
    
    for (auto it = this->layer_stack.rbegin(); it != this->layer_stack.rend(); ++it)
    {
      if (event.handled) break;
      (*it)->on_event(event);
    }
  }
  
  void Engine::on_update(float time_step)
  {
    Timer frame_time;
    frame_time.start();
    
    if (!Engine::main_window->is_minimized())
    {
      // Poll all context events
      Engine::main_window->poll_api_events();
      for (Layer *layer: this->layer_stack)
      {
        layer->on_update(time_step);
        layer->on_ui_render(time_step);
      }
    }
    // Refresh window
    Engine::main_window->on_update(time_step); // Refresh the window screen.
    
    Engine::last_mouse_position = Input::get_mouse_cursor_position();
    Engine::set_frame_drawn_counter(Engine::frame_drawn_counter + 1);
    
    frame_time.stop();
    // Set minimum wait time between each frame to control game speed.
    Engine::wait(frame_time.get_time_in_seconds(),
                 Engine::main_window->is_vsync() ? 1.0f / static_cast<float>(Engine::main_window->get_refresh_rate()) :
                 Engine::main_window->is_minimized() ? 1.0f / 30.0f :
                 0.0f);  // Set to NOT wait and to render as many frames as possible (V-Sync off).
  }
  
  void Engine::wait(float start_time, float end_time)
  {
    Timer time_waited;
    time_waited.start();
    while (start_time + time_waited.get_time_in_seconds() < end_time) time_waited.stop();  // Update the timer.
  }
  
  void Engine::shutdown()
  {
    Engine::set_running_state(false);
    
    Engine::main_window->close();
    Renderer::shutdown();
    
    if ((Engine::get_exit_status() >> 4) &
        (WAVE_ENGINE_CONTEXT_CRASH >> 4))  // Shift 4 bits to the right to mask error.
    {
      alert(WAVE_ERROR, "[ENGINE] --> Window has not been requested to close by the user!");
    }
    if ((Engine::get_exit_status() >> 4) &
        (WAVE_ENGINE_RENDERER_CRASH >> 4))  // Shift 4 bits to the right to mask error.
    {
      alert(WAVE_ERROR, "[ENGINE] --> Renderer has encountered a fatal error!");
    }
  }
  
  bool Engine::has_crashed()
  {
    return get_exit_status() != WAVE_ENGINE_NO_ERRORS;  // In case of an unexpected crash.
  }
  
  /******************* CALLBACKS ******************/
  
  bool Engine::any_key_callback([[maybe_unused]] On_any_key_event &any_key)
  {
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_ENTER))
    {
      Wave::Display_settings::toggle_fullscreen(Wave::Engine::get_main_window());
      return true;
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_V))
    {
      Wave::Display_settings::set_vsync(Wave::Engine::get_main_window(),
                                        !Wave::Engine::get_main_window()->is_vsync());
      return true;
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_6))
    {
      Wave::Display_settings::set_refresh_rate(Wave::Engine::get_main_window(), 60);
      return true;
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_3))
    {
      Wave::Display_settings::set_refresh_rate(Wave::Engine::get_main_window(), 30);
      return true;
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_1))
    {
      Wave::Display_settings::set_refresh_rate(Wave::Engine::get_main_window(), 1);
      return true;
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_9))
    {
      Wave::Display_settings::set_refresh_rate(Wave::Engine::get_main_window(), 144);
      return true;
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_F4))
    {
      Wave::alert(WAVE_WARN, "[SETTING] --> Force shutdown");
      Wave::Engine::get_main_window()->close();
      return true;
    }
    return false;
  }
  
  bool Engine::key_press_callback([[maybe_unused]] On_key_press &key_press)
  {
    return true;
  }
  
  bool Engine::key_hold_callback([[maybe_unused]] On_key_hold &key_hold)
  {
    return true;
  }
  
  bool Engine::key_release_callback([[maybe_unused]] On_key_release &key_release)
  {
    return true;
  }
  
  [[maybe_unused]] bool Engine::mouse_button_press_callback([[maybe_unused]] On_mouse_button_press &mouse_button_press)
  {
    return true;
  }
  
  [[maybe_unused]] bool Engine::mouse_button_hold_callback(On_mouse_button_hold &mouse_button_hold)
  {
    switch (mouse_button_hold.get_current_mouse_button())
    {
      case WAVE_MOUSE_BUTTON_LEFT:Input::set_mouse_locked(true);
        break;
      default:break;
    }
    return true;
  }
  
  bool Engine::mouse_button_release_callback([[maybe_unused]] On_mouse_button_release &mouse_button_release)
  {
    return true;
  }
  
  [[maybe_unused]] bool Engine::mouse_wheel_callback([[maybe_unused]] On_mouse_wheel_scroll &mouse_wheel_input)
  {
    return true;
  }
  
  [[maybe_unused]] bool Engine::mouse_movement_callback([[maybe_unused]] On_mouse_movement &mouse_cursor_event)
  {
    return true;
  }
  
  bool Engine::window_closed_callback([[maybe_unused]] On_window_close &window_closed_event)
  {
    window_closed_event.print(Print_type::Warn);
    Engine::shutdown();
    return true;
  }
  
  [[maybe_unused]] bool Engine::window_resize_callback(On_window_resize &resize_event)
  {
    // If we called on_window_resize with the same viewport dimensions.
    if (resize_event.get_height() == Engine::main_window->get_height() &&
        resize_event.get_width() == Engine::main_window->get_width())
    {
      return true;
    }
    
    resize_event.print(Print_type::Warn);
    if (resize_event.get_width() <= 0.0f || resize_event.get_height() <= 0.0f) return false;
    Engine::main_window->resize(resize_event.get_width(), resize_event.get_height());
    Renderer::on_event(resize_event);
    return true;
  }
}