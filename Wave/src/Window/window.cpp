//
// Created by nami on 9/5/22.
//
#include <Window/window.h>
#include <Events/key_event.h>
#include <Events/mouse_event.h>
#include <Events/app_event.h>
#include <Events/renderer_event.h>
#include <Renderer/renderer.h>

namespace Wave
{
  
  Window::Window(const Window &window_)
  {
    if (this == &window_) return;
    this->window = window_.window;
    this->monitor = window_.monitor;
    this->properties.title = window_.properties.title;
    this->properties.width = window_.properties.width;
    this->properties.height = window_.properties.height;
    this->properties.refresh_rate = window_.properties.refresh_rate;
    this->vsync = window_.vsync;
    this->max_refresh_rate = window_.max_refresh_rate;
    this->position_on_screen = window_.position_on_screen;
    this->x_scale = window_.x_scale;
    this->y_scale = window_.y_scale;
    this->bg_color = window_.bg_color;
    this->fullscreen = window_.fullscreen;
  }
  
  Window::Window(Context_api context_api_chosen) : Window()
  {
    this->context_api = context_api_chosen;
  }
  
  void Window::setup_api()
  {
  
  }
  
  void Window::setup_monitor()
  {
  
  }
  
  void Window::create_window()
  {
  
  }
  
  void Window::on_update()
  {
  
  }
  
  void Window::toggle_fullscreen()
  {
  
  }
  
  bool Window::is_closing()
  {
    return false;
  }
  
  void Window::close()
  {
  
  }
  
  void Window::hide()
  {
  
  }
  
  Context_api Window::get_context() const
  {
    return this->context_api;
  }
  
  const std::function<void(Event &event)> &Window::get_event_callback_function() const
  {
    return this->event_callback_function;
  }
  
  void *Window::get_native_window() const
  {
    return this->window;
  }
  
  void *Window::get_monitor() const
  {
    return this->monitor;
  }
  
  const char *Window::get_title() const
  {
    return this->properties.title;
  }
  
  uint32_t Window::get_width() const
  {
    return (int32_t) this->properties.width;
  }
  
  uint32_t Window::get_height() const
  {
    return (int) this->properties.height;
  }
  
  uint32_t Window::get_refresh_rate() const
  {
    return this->properties.refresh_rate;
  }
  
  bool Window::is_fullscreen() const
  {
    return this->fullscreen;
  }
  
  bool Window::is_vsync() const
  {
    return this->vsync;
  }
  
  bool Window::is_hovered() const
  {
    return false;
  }
  
  bool Window::is_focused() const
  {
    return false;
  }
  
  Window_pos Window::get_window_pos() const
  {
    return this->position_on_screen;
  }
  
  float Window::get_x_scale() const
  {
    return this->x_scale;
  }
  
  float Window::get_y_scale() const
  {
    return this->y_scale;
  }
  
  Color Window::get_bg_color()
  {
    return this->bg_color;
  }
  
  uint32_t Window::get_max_refresh_rate() const
  {
    return this->max_refresh_rate;
  }
  
  void Window::set_event_callback_function(const std::function<void(Event &)> &callback_function)
  {
    this->event_callback_function = callback_function;
  }
  
  void Window::set_native_window(void *window_)
  {
    this->window = window_;
  }
  
  void Window::set_native_monitor(void *monitor_)
  {
    this->monitor = monitor_;
  }
  
  void Window::set_title(const char *title_)
  {
    this->properties.title = title_;
  }
  
  void Window::set_width(uint32_t width_)
  {
    this->properties.width = width_;
  }
  
  void Window::set_height(uint32_t height_)
  {
    this->properties.height = height_;
  }
  
  void Window::set_fullscreen(bool fullscreen_state)
  {
    this->fullscreen = fullscreen_state;
  }
  
  void Window::set_refresh_rate(uint32_t refresh_rate_)
  {
    this->properties.refresh_rate = refresh_rate_;
  }
  
  void Window::set_max_refresh_rate(uint32_t refresh_rate_)
  {
    this->max_refresh_rate = refresh_rate_;
  }
  
  void Window::set_vsync(bool vsync_)
  {
    this->vsync = vsync_;
  }
  
  void Window::set_window_pos(uint32_t x_pos_, uint32_t y_pos_)
  {
    if (x_pos_ > this->properties.width || y_pos_ > this->properties.height)
    {
      this->position_on_screen = {this->properties.width / 2,
                                  this->properties.height / 2};
      return;
    }
    this->position_on_screen = {x_pos_,
                                y_pos_};
  }
  
  void Window::set_x_scale(float x_scale_)
  {
    this->x_scale = x_scale_;
  }
  
  void Window::set_y_scale(float y_scale_)
  {
    this->y_scale = y_scale_;
  }
  
  bool Window::operator ==(const Wave::Window &window_)
  {
    if (this == &window_) return true;
    return false;
  }
  
  bool Window::operator !=(const Wave::Window &window_)
  {
    if (this == &window_) return false;
    return true;
  }
  
  // OPENGL WINDOW (GLFW)
  Gl_window::Gl_window() : Window()
  {
    log_task("WINDOW", PURPLE, 1, "Preparing app window ...", Gl_window::create_window(),
             "App window loaded") // Make glfw context.
  }
  
  void Gl_window::setup_api()
  {
    glfw_call(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE));
    glfw_call(glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE));
    glfw_call(glfwWindowHint(GLFW_FLOATING, GLFW_TRUE));
    glfw_call(glfwWindowHint(GLFW_SAMPLES, 4));
    glfw_call(glfwWindowHint(GLFW_RED_BITS, this->properties.red_bits));
    glfw_call(glfwWindowHint(GLFW_GREEN_BITS, this->properties.green_bits));
    glfw_call(glfwWindowHint(GLFW_BLUE_BITS, this->properties.blue_bits));
    glfw_call(glfwWindowHint(GLFW_REFRESH_RATE, this->properties.refresh_rate));
    glfw_call(glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE));
    glfw_call(glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3));
    glfw_call(glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3));
//    glfw_call(glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE));
  }
  
  void Gl_window::setup_monitor()
  {
    int monitor_count;
    glfw_call(GLFWmonitor **monitors = glfwGetMonitors(&monitor_count));
    if (monitors) set_gl_monitor(monitors[0]); // Get main monitor specs.
    glfw_call(const GLFWvidmode *mode = glfwGetVideoMode(
                  static_cast<GLFWmonitor *>(get_gl_monitor()))); // Get video specs of monitor.
    set_width((int) (mode->width / 2));
    set_height((int) (mode->height / 2));
    set_refresh_rate(mode->refreshRate);
    set_max_refresh_rate(mode->refreshRate);
    set_vsync(true);
    set_title("Main window");
    float x_scale_, y_scale_;
    glfw_call(glfwGetMonitorContentScale(static_cast<GLFWmonitor *>(get_gl_monitor()), &x_scale_,
                                         &y_scale_));  // Get monitor scale.
    set_x_scale(x_scale_);
    set_y_scale(y_scale_);
  }
  
  void Gl_window::create_window()
  {
    // Init glfw library.
    int res;
    log_instruction("WINDOW", DEFAULT, "Making openGL context", res = glfwInit())
    if (!res)
    {
      Gl_renderer::set_state({nullptr,
                              "Fatal",
                              "Error initializing OpenGL!",
                              Renderer_error_type::API_ERROR});
      return;
    }
    set_glfw_init_status(true);
    
    glfwGetVersion(&this->api_info.version_major, &this->api_info.version_minor, &this->api_info.version_revision);
    // So that we will only use the modern functions in it.
    glfw_call(Wave::alert(WAVE_INFO, "[WINDOW] --> Api (GLFW) version : %d.%d.%d",
                          this->api_info.version_major, this->api_info.version_minor,
                          this->api_info.version_revision));
    log_task("WINDOW", RED, 2, "Setting up monitor...", setup_monitor(), "Monitor loaded and set")
    log_instruction("WINDOW", DEFAULT, "Setting up window api", setup_api())
    // Generate a pointer to a window using our monitor info, so that we later hide it.
    log_instruction("WINDOW", DEFAULT, "Creating window (windowed mode)",
                    glfw_call(GLFWwindow * window_ = glfwCreateWindow(get_width(), get_height(), get_title(),
                                                                      nullptr, nullptr))) // Windowed mode.
    set_gl_window(window_);
    if (!this->window)
    {
      Wave::alert(WAVE_ERROR, "[WINDOW] --> One or multiple core extensions for GLFW are not supported!\tExiting ...");
    }
    glfw_call(glfwMakeContextCurrent(static_cast<GLFWwindow *>(get_gl_window()))); // Show our window.
    // Specify which coordinates to draw for our window -> from (0,0) to (monitor_width, monitor_height).
    gl_call(glViewport(0, 0, get_width(), get_height()));
    glfw_call(glfwSetWindowAspectRatio(static_cast<GLFWwindow *>(get_gl_window()), 16, 9));
    glfw_call(glfwSetInputMode(static_cast<GLFWwindow *>(get_gl_window()), GLFW_CURSOR,
                               GLFW_CURSOR_NORMAL)); // Set our mouse cursor to default.
    glfw_call(glfwSetInputMode(static_cast<GLFWwindow *>(get_gl_window()), GLFW_STICKY_KEYS, GLFW_TRUE));
    glfw_call(glfwSetInputMode(static_cast<GLFWwindow *>(get_gl_window()), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE));
    int x_pos_, y_pos_;
    glfw_call(glfwGetWindowPos(static_cast<GLFWwindow *>(get_gl_window()), &x_pos_,
                               &y_pos_));  // Get initial window position.
    set_window_pos(x_pos_, y_pos_);
    glfwSetWindowUserPointer(static_cast<GLFWwindow *>(this->get_native_window()), reinterpret_cast<void *>(this));
  }
  
  void Gl_window::on_update()
  {
    if (is_closing())
    {
      On_window_close window_closed;
      this->get_event_callback_function()(window_closed);
      return;
    }
    // Refresh framebuffer.
    glfw_call(glfwSwapBuffers(static_cast<GLFWwindow *>(this->get_native_window())));
    glfw_call(glfwSwapInterval(this->vsync));  // Disable/enable Vertical synchronisation (Vsync).
  }
  
  void Gl_window::bind_api_callbacks()
  {
    glfw_call(glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                             [](GLFWwindow *window, int32_t width, int32_t height)
                                             {
                                               Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(
                                                   window);
                                               On_window_resize window_resized(width, height);
                                               this_window_instance.get_event_callback_function()(window_resized);
                                             }));
    
    glfw_call(glfwSetCharCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                  [](GLFWwindow *window, [[maybe_unused]] uint32_t keycode)
                                  {
                                    Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window);
                                    
                                    On_any_key_callback key_typed;
                                    this_window_instance.get_event_callback_function()(key_typed);
                                  }
                                 ));
    
    glfw_call(glfwSetKeyCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                 [](GLFWwindow *window, int32_t key, [[maybe_unused]] int32_t scancode,
                                    int32_t action, [[maybe_unused]] int32_t mods)
                                 {
                                   Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window);
                                   On_any_key_callback new_key_event;
                                   this_window_instance.get_event_callback_function()(new_key_event);
                                   
                                   switch (action)
                                   {
                                     case GLFW_PRESS:
                                     {
                                       On_key_press key_pressed(key);
                                       this_window_instance.get_event_callback_function()(key_pressed);
                                       break;
                                     }
                                     
                                     case GLFW_REPEAT:
                                     {
                                       On_key_hold key_held(key);
                                       this_window_instance.get_event_callback_function()(key_held);
                                       break;
                                     }
                                     
                                     case GLFW_RELEASE:
                                     {
                                       On_key_release key_released(key);
                                       this_window_instance.get_event_callback_function()(key_released);
                                       break;
                                     }
                                     
                                     default:break;
                                   }
                                 }));
    glfw_call(glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                         [](GLFWwindow *window, int32_t button, int32_t action,
                                            [[maybe_unused]] int32_t mods)
                                         {
                                           Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window);
                                           
                                           switch (action)
                                           {
                                             case GLFW_PRESS:
                                             {
                                               On_any_key_callback new_key_event;
                                               this_window_instance.get_event_callback_function()(new_key_event);
                                               On_mouse_button_press mouse_btn_pressed(button);
                                               this_window_instance.get_event_callback_function()(mouse_btn_pressed);
                                               break;
                                             }
                                             
                                             case GLFW_REPEAT:
                                             {
                                               On_any_key_callback new_key_event;
                                               this_window_instance.get_event_callback_function()(new_key_event);
                                               On_mouse_button_hold mouse_btn_held(button);
                                               this_window_instance.get_event_callback_function()(mouse_btn_held);
                                               break;
                                             }
                                             
                                             case GLFW_RELEASE:
                                             {
                                               On_any_key_callback new_key_event;
                                               this_window_instance.get_event_callback_function()(new_key_event);
                                               On_mouse_button_release mouse_btn_released(button);
                                               this_window_instance.get_event_callback_function()(mouse_btn_released);
                                               break;
                                             }
                                             
                                             default:break;
                                           }
                                         }));
    glfw_call(glfwSetCursorPosCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                       
                                       [](GLFWwindow *window, double x, double y)
                                       {
                                         Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window);
                                         On_mouse_movement mouse_movement_event(
                                             Vector_2f(static_cast<float>(x), static_cast<float>(y)));
                                         this_window_instance.get_event_callback_function()(mouse_movement_event);
                                       }));
    glfw_call(glfwSetWindowSizeCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                        [](GLFWwindow *window, int32_t width_, int32_t height_)
                                        {
                                          Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window);
                                          On_window_resize window_resized(width_, height_);
                                          this_window_instance.get_event_callback_function()(window_resized);
                                        }));
    glfw_call(glfwSetScrollCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                    [](GLFWwindow *window, double x_offset, double y_offset)
                                    {
                                      Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window);
                                      On_mouse_wheel_scroll wheel_input(
                                          Vector_2f(static_cast<float>(x_offset),
                                                    static_cast<float>(y_offset)));
                                      this_window_instance.get_event_callback_function()(wheel_input);
                                    }));
  }
  
  void Gl_window::unbind_api_callback(const Event &event)
  {
    if (event.get_category_flags() & Event_category::EVENT_CATEGORY_KEYBOARD)
    {
      glfw_call(glfwSetKeyCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr));
      return;
    }
    
    switch (event.get_event_type())
    {
      case Event_type::On_mouse_event:
      {
        glfw_call(glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr));
        break;
      }
      case Event_type::On_mouse_movement:
      {
        glfw_call(glfwSetCursorPosCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr));
        break;
      }
      case Event_type::On_window_resize:
      {
        glfw_call(glfwSetWindowSizeCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr));
        break;
      }
      case Event_type::On_mouse_wheel_scroll:
      {
        glfw_call(glfwSetScrollCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr));
        break;
      }
      default:break;
    }
  }
  
  void Gl_window::glfw_error_callback(const char *function_name, const char *source_file,
                                      size_t line_number)
  {
    const char *_err_str;
    if (glfwGetCurrentContext())
    {
      uint32_t error_code = glfwGetError(&_err_str);
      if (error_code != GLFW_NO_ERROR)
      {
        Gl_renderer::set_state({"Glfw error",
                                "Fata error",
                                _err_str,
                                Renderer_error_type::NO_ERROR});
        Wave::alert(WAVE_ERROR,
                    "%sFATAL%s : [GLFW ERROR]\n%48sIN FILE %s,"
                    "\n%48sIN FUNCTION %s,\n%48sAT LINE %d,\n%48s%s%s\n",
                    RED, DEFAULT, " ", source_file, " ", function_name, " ", line_number, " ", RED, _err_str);
      }
    }
  }
  
  void Gl_window::toggle_fullscreen()
  {
    if (!is_fullscreen())
    {
      alert(WAVE_INFO, "[WINDOW] --> Set window position to : (%d, %d)",
            this->get_window_pos().x_pos, this->get_window_pos().y_pos);
      
      glfw_call(set_gl_monitor(glfwGetPrimaryMonitor()));
      glfw_call(const GLFWvidmode *mode = glfwGetVideoMode(static_cast<GLFWmonitor *>(get_gl_monitor())));
      glfw_call(
          glfwSetWindowMonitor(static_cast<GLFWwindow *>(get_gl_window()), static_cast<GLFWmonitor *>(get_gl_monitor()),
                               0, 0,
                               mode->width, mode->height, mode->refreshRate));
      set_fullscreen(true);
      set_width(mode->width);
      set_height(mode->height);
    }
    else
    {
      int width_ = 0, height_ = 0;
      glfw_call(glfwGetWindowSize(static_cast<GLFWwindow *>(get_gl_window()), &width_, &height_));
      glfw_call(glfwSetWindowMonitor(static_cast<GLFWwindow *>(get_gl_window()), nullptr, get_window_pos().x_pos,
                                     get_window_pos().y_pos,
                                     int(width_ / 2), (int) (height_ / 2),
                                     get_refresh_rate()));  // Set maximum update rate possible.
      set_fullscreen(false);
      set_width(width_ / 2);
      set_height(height_ / 2);
    }
    gl_call(glViewport(0, 0, get_width(), get_height()));
  }
  
  // Let the window open as long as the close flag (gathered by glfwPollEvents) is not set to true.
  bool Gl_window::is_closing()
  {
    if (this->glfw_init)
    {
      return glfw_call(
                 glfwWindowShouldClose(static_cast<GLFWwindow *>(get_gl_window())) ? true : false |
                                                                                            this->request_closing);
    }
    return false;
  }
  
  // Signal that the window is closing.
  void Gl_window::close()
  {
    glfw_call(glfwSetWindowShouldClose(static_cast<GLFWwindow *>(get_gl_window()), GLFW_TRUE));
    this->request_closing = true;
  }
  
  void Gl_window::shutdown()
  {
    if (glfw_is_init())  // If window has been created.
    {
      log_instruction("WINDOW", DEFAULT, "Terminating GLFW",
                      {
                        this->close();
                        glfw_call(glfwDestroyWindow(static_cast<GLFWwindow *>(get_gl_window())));
                        glfwTerminate();
                      })
    }
    this->glfw_init = false;
  }
  
  // Show the window created.
  void Gl_window::hide()
  {
    glfw_call(glfwMakeContextCurrent(nullptr)); // Hide our window.
  }
  
  bool Gl_window::glfw_is_init() const
  {
    return this->glfw_init;
  }
  
  // Return a glfwWindow instance in case there's a need to manipulate the window directly.
  void *Gl_window::get_gl_window() const
  {
    return Window::get_native_window();
  }
  
  void *Gl_window::get_gl_monitor() const
  {
    return Window::get_monitor();
  }
  
  bool Gl_window::is_focused() const
  {
    bool focus;
    glfw_call(focus = glfwGetWindowAttrib(static_cast<GLFWwindow *>(get_gl_window()), GLFW_FOCUSED));
    return focus;
  }
  
  bool Gl_window::is_hovered() const
  {
    bool hover;
    glfw_call(hover = glfwGetWindowAttrib(static_cast<GLFWwindow *>(get_gl_window()), GLFW_HOVERED));
    return hover;
  }
  
  void Gl_window::set_glfw_init_status(bool status)
  {
    this->glfw_init = status;
  }
  
  void Gl_window::set_gl_window(void *window)
  {
    set_native_window(window);
  }
  
  void Gl_window::set_gl_monitor(void *monitor)
  {
    set_native_monitor(monitor);
  }
  
  void Gl_window::set_title(const char *title_)
  {
    Window::set_title(title_);
    if (get_gl_window())
    {
      glfw_call(glfwSetWindowTitle(static_cast<GLFWwindow *>(get_gl_window()), get_title()));
    }
  }
  
  bool Gl_window::operator !=(const Gl_window &window_)
  {
    if (this == &window_) return false;
    return static_cast<GLFWwindow *>(get_native_window()) != static_cast<GLFWwindow *>(window_.get_native_window());
  }
  
  bool Gl_window::operator ==(const Gl_window &window_)
  {
    if (this == &window_) return true;
    return static_cast<GLFWwindow *>(get_native_window()) == static_cast<GLFWwindow *>(window_.get_native_window());
  }
}