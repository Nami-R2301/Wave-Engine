//
// Created by nami on 9/5/22.
//

#include <Renderer/renderer.h>
#include <Window/window.h>
#include <Events/key_event.h>
#include <Events/mouse_event.h>
#include <Events/app_event.h>
#include <Events/renderer_event.h>

namespace Wave
{
  
  std::function<void(Event &event)> Window::event_callback_function;
  
  const std::function<void(Event &event)> &Window::get_event_callback_function()
  {
    return Window::event_callback_function;
  }
  
  Gl_window::Gl_window() : Window()
  {
    log_task("WINDOW", PURPLE, 1, "Preparing app window ...", Gl_window::create_window(),
             "App window loaded") // Make glfw context.
  }
  
  Gl_window::Gl_window(Context_api context_api_chosen) : Window()
  {
    this->context_api = context_api_chosen;
  }
  
  Gl_window::~Gl_window()
  {
    log_task("WINDOW", GREEN, 4, "Closing window ...", Gl_window::shutdown(), "Window closed")
  }
  
  void Gl_window::create_window()
  {
    // Init glfw library.
    log_instruction("WINDOW", DEFAULT, "Making openGL context", glfw_call(glfwInit()))
    this->set_glfw_init_status(true);
    
    glfwGetVersion(&this->api_info.version_major, &this->api_info.version_minor, &this->api_info.version_revision);
    // So that we will only use the modern functions in it.
    glfw_call(Wave::alert(WAVE_INFO, "[WINDOW] --> Api (GLFW) version : %d.%d.%d",
                          this->api_info.version_major, this->api_info.version_minor,
                          this->api_info.version_revision));
    log_task("WINDOW", RED, 2, "Setting up monitor...", this->setup_monitor(), "Monitor loaded and set")
    log_instruction("WINDOW", DEFAULT, "Setting up window api", this->setup_api())
    // Generate a pointer to a window using our monitor info, so that we later hide it.
    log_instruction("WINDOW", DEFAULT, "Creating window (windowed mode)",
                    glfw_call(GLFWwindow * window_ = glfwCreateWindow(this->get_width(), this->get_height(),
                                                                      this->get_title(),
                                                                      nullptr, nullptr))) // Windowed mode.
    this->set_native_window(window_);
    if (!this->window)
    {
      Wave::alert(WAVE_ERROR, "[WINDOW] --> One or multiple core extensions for GLFW are not supported!\tExiting ...");
    }
    Gl_renderer::on_window_resize(this, static_cast<float>(this->get_width()),
                                  static_cast<float>(this->get_height()));
    
    glfw_call(glfwMakeContextCurrent(static_cast<GLFWwindow *>(this->get_native_window()))); // Show our window.
    glfw_call(glfwSetWindowAspectRatio(static_cast<GLFWwindow *>(this->get_native_window()), 16, 9));
    glfw_call(glfwSetInputMode(static_cast<GLFWwindow *>(this->get_native_window()), GLFW_CURSOR,
                               GLFW_CURSOR_NORMAL)); // Set our mouse cursor to default.
    glfw_call(glfwSetInputMode(static_cast<GLFWwindow *>(this->get_native_window()), GLFW_STICKY_KEYS, GLFW_TRUE));
    glfw_call(
        glfwSetInputMode(static_cast<GLFWwindow *>(this->get_native_window()), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE));
    int x_pos_, y_pos_;
    glfw_call(glfwGetWindowPos(static_cast<GLFWwindow *>(this->get_native_window()), &x_pos_,
                               &y_pos_));  // Get initial window position.
    this->set_window_pos(static_cast<float>(x_pos_), static_cast<float>(y_pos_));
    glfwSetWindowUserPointer(static_cast<GLFWwindow *>(this->get_native_window()), reinterpret_cast<void *>(this));
  }
  
  void Gl_window::setup_api()
  {
    glfw_call(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE));
    glfw_call(glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE));
    glfw_call(glfwWindowHint(GLFW_FLOATING, GLFW_TRUE));
    glfw_call(glfwWindowHint(GLFW_SAMPLES, 4));
    glfw_call(glfwWindowHint(GLFW_RED_BITS, this->monitor_properties.red_bits));
    glfw_call(glfwWindowHint(GLFW_GREEN_BITS, this->monitor_properties.green_bits));
    glfw_call(glfwWindowHint(GLFW_BLUE_BITS, this->monitor_properties.blue_bits));
    glfw_call(glfwWindowHint(GLFW_REFRESH_RATE, this->monitor_properties.refresh_rate));
    glfw_call(glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE));
    glfw_call(glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3));
    glfw_call(glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3));
//    glfw_call(glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE));
  }
  
  void Gl_window::setup_monitor()
  {
    int monitor_count;
    glfw_call(GLFWmonitor **monitors = glfwGetMonitors(&monitor_count));
    if (monitors) this->set_native_monitor(monitors[0]); // Get main monitor specs.
    glfw_call(const GLFWvidmode *mode = glfwGetVideoMode(
                  static_cast<GLFWmonitor *>(this->get_native_monitor()))); // Get video specs of monitor.
    this->set_width(static_cast<float>(mode->width));
    this->set_height(static_cast<float>(mode->height));
    this->set_refresh_rate(mode->refreshRate);
    this->set_max_refresh_rate(mode->refreshRate);
    this->set_vsync(true);
    this->set_title("~ Wave Engine ~");
    float x_scale_, y_scale_;
    glfw_call(glfwGetMonitorContentScale(static_cast<GLFWmonitor *>(this->get_native_monitor()), &x_scale_,
                                         &y_scale_));  // Get monitor scale.
    this->set_x_scale(x_scale_);
    this->set_y_scale(y_scale_);
  }
  
  void Gl_window::on_update(float time_step)
  {
    if (this->is_closing())
    {
      On_window_close window_closed;
      this->get_event_callback_function()(window_closed);
      return;
    }
    Gl_renderer::set_clear_color(this->bg_color);
    // Refresh framebuffer.
    glfw_call(glfwSwapBuffers(static_cast<GLFWwindow *>(this->get_native_window())));
    glfw_call(glfwSwapInterval(this->vsync));  // Disable/enable Vertical synchronisation (Vsync).
  }
  
  void Gl_window::bind_api_callbacks()
  {
    glfw_call(glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                             [](GLFWwindow *window_, int32_t width, int32_t height)
                                             {
                                               Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(
                                                   window_);
                                               On_window_resize window_resized(width, height);
                                               this_window_instance.get_event_callback_function()(window_resized);
                                             }));
    
    
    glfw_call(glfwSetWindowIconifyCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                           [](GLFWwindow *window_, int minimized)
                                           {
                                             Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(
                                                 window_);
                                             if (minimized)
                                             {
                                               // Avoid working at max fps when minimized.
                                               this_window_instance.set_refresh_rate(30.0f);
                                             }
                                             else
                                             {
                                               this_window_instance.set_refresh_rate(
                                                   this_window_instance.get_max_refresh_rate());
                                             }
                                           }));
    
    glfw_call(glfwSetKeyCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                 [](GLFWwindow *window_, int32_t key, [[maybe_unused]] int32_t scancode,
                                    int32_t action, [[maybe_unused]] int32_t mods)
                                 {
                                   Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window_);
                                   On_any_key_event any_key;
                                   this_window_instance.get_event_callback_function()(any_key);
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
                                     
                                     default:
                                     {
                                       this_window_instance.get_event_callback_function()(any_key);
                                       break;
                                     }
                                   }
                                 }));
    glfw_call(glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                         [](GLFWwindow *window_, int32_t button, int32_t action,
                                            [[maybe_unused]] int32_t mods)
                                         {
                                           Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window_);
                                           
                                           switch (action)
                                           {
                                             case GLFW_PRESS:
                                             {
                                               On_mouse_button_press mouse_btn_pressed(button);
                                               this_window_instance.get_event_callback_function()(mouse_btn_pressed);
                                               break;
                                             }
                                             
                                             case GLFW_REPEAT:
                                             {
                                               On_mouse_button_hold mouse_btn_held(button);
                                               this_window_instance.get_event_callback_function()(mouse_btn_held);
                                               break;
                                             }
                                             
                                             case GLFW_RELEASE:
                                             {
                                               On_mouse_button_release mouse_btn_released(button);
                                               this_window_instance.get_event_callback_function()(mouse_btn_released);
                                               break;
                                             }
                                             
                                             default:break;
                                           }
                                         }));
//    glfw_call(glfwSetCursorPosCallback(static_cast<GLFWwindow *>(this->get_native_window()),
//
//                                       [](GLFWwindow *window_, double x, double y)
//                                       {
//                                         Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window_);
//                                         On_mouse_movement mouse_movement_event(
//                                             Vector_2f(static_cast<float>(x), static_cast<float>(y)));
//                                         this_window_instance.get_event_callback_function()(mouse_movement_event);
//                                       }));
    glfw_call(glfwSetWindowSizeCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                        [](GLFWwindow *window_, int32_t width_, int32_t height_)
                                        {
                                          Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window_);
                                          On_window_resize window_resized(width_, height_);
                                          this_window_instance.get_event_callback_function()(window_resized);
                                        }));
    glfw_call(glfwSetScrollCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                    [](GLFWwindow *window_, double x_offset, double y_offset)
                                    {
                                      Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window_);
                                      On_mouse_wheel_scroll wheel_input(
                                          Vector_2f(static_cast<float>(x_offset),
                                                    static_cast<float>(y_offset)));
                                      this_window_instance.get_event_callback_function()(wheel_input);
                                    }));
  }
  
  void Gl_window::unbind_api_callback(const Event &event)
  {
    // If any of the subcategories of keyboard is requested to be unbound, we are forced to unbind this single callback to undo all.
    if (event.get_category_flags() & Event_category::EVENT_CATEGORY_KEYBOARD)
    {
      glfw_call(glfwSetKeyCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr));
      return;
    }
    
    // If any of the subcategories of mouse button is requested to be unbound, we are forced to unbind this single callback to undo all.
    if (event.get_category_flags() & Event_category::EVENT_CATEGORY_MOUSE_BUTTON)
    {
      glfw_call(glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr));
      return;
    }
    
    switch (event.get_event_type())
    {
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
      default:return;
    }
  }
  
  bool Gl_window::is_minimized() const
  {
    return glfwGetWindowAttrib(static_cast<GLFWwindow *>(this->get_native_window()), GLFW_ICONIFIED);
  }
  
  // Let the window open as long as the close flag (gathered by glfwPollEvents) is not set to true.
  bool Gl_window::is_closing()
  {
    if (this->glfw_init)
    {
      return glfw_call(
          glfwWindowShouldClose(static_cast<GLFWwindow *>(get_native_window())) ? true : false |
                                                                                         this->request_closing);
    }
    return false;
  }
  
  // Show the window created.
  void Gl_window::hide()
  {
    glfw_call(glfwHideWindow(static_cast<GLFWwindow *>(this->get_native_window()))); // Hide our window.
  }
  
  void Gl_window::toggle_fullscreen()
  {
    if (!is_fullscreen())
    {
      alert(WAVE_INFO, "[WINDOW] --> Set window position to : (%d, %d)",
            this->get_window_pos().get_x(), this->get_window_pos().get_y());
      
      glfw_call(this->set_native_monitor(glfwGetPrimaryMonitor()));
      glfw_call(const GLFWvidmode *mode = glfwGetVideoMode(static_cast<GLFWmonitor *>(get_native_monitor())));
      glfw_call(
          glfwSetWindowMonitor(static_cast<GLFWwindow *>(this->get_native_window()),
                               static_cast<GLFWmonitor *>(get_native_monitor()),
                               0, 0,
                               mode->width, mode->height, mode->refreshRate));
      this->set_fullscreen(true);
      this->set_width(static_cast<float>(mode->width));
      this->set_height(static_cast<float>(mode->height));
    }
    else
    {
      int width_ = 0, height_ = 0;
      glfw_call(glfwGetWindowSize(static_cast<GLFWwindow *>(this->get_native_window()), &width_, &height_));
      glfw_call(glfwSetWindowMonitor(static_cast<GLFWwindow *>(this->get_native_window()), nullptr,
                                     this->get_window_pos().get_x(),
                                     this->get_window_pos().get_y(),
                                     width_, height_,
                                     this->get_refresh_rate()));  // Set maximum update rate possible.
      this->set_fullscreen(false);
      this->set_width(static_cast<float>(width_));
      this->set_height(static_cast<float>(height_));
    }
    Gl_renderer::on_window_resize(this, static_cast<float>(this->get_width()),
                                  static_cast<float>(this->get_height()));
  }
  
  // Signal that the window is closing.
  void Gl_window::close()
  {
    glfw_call(glfwSetWindowShouldClose(static_cast<GLFWwindow *>(get_native_window()), GLFW_TRUE));
    this->request_closing = true;
  }
  
  void Gl_window::shutdown()
  {
    if (glfw_is_init())  // If window has been created.
    {
      log_instruction("WINDOW", DEFAULT, "Terminating GLFW",
                      {
                        this->close();
                        glfw_call(glfwDestroyWindow(static_cast<GLFWwindow *>(get_native_window())));
                        glfw_call(glfwTerminate());
                      })
    }
    this->glfw_init = false;
  }
  
  Context_api Gl_window::get_context() const
  {
    return this->context_api;
  }
  
  void *Gl_window::get_native_window() const
  {
    return this->window;
  }
  
  void *Gl_window::get_native_monitor() const
  {
    return this->monitor;
  }
  
  const char *Gl_window::get_title() const
  {
    return this->monitor_properties.title;
  }
  
  float Gl_window::get_width() const
  {
    return this->monitor_properties.width;
  }
  
  float Gl_window::get_height() const
  {
    return this->monitor_properties.height;
  }
  
  const Vector_2f &Gl_window::get_aspect_ratio() const
  {
    return this->aspect_ratio;
  }
  
  uint32_t Gl_window::get_refresh_rate() const
  {
    return this->monitor_properties.refresh_rate;
  }
  
  bool Gl_window::is_fullscreen() const
  {
    return this->fullscreen;
  }
  
  bool Gl_window::is_vsync() const
  {
    return this->vsync;
  }
  
  const Vector_2f &Gl_window::get_window_pos() const
  {
    return this->position_on_screen;
  }
  
  float Gl_window::get_x_scale() const
  {
    return this->x_scale;
  }
  
  float Gl_window::get_y_scale() const
  {
    return this->y_scale;
  }
  
  Color &Gl_window::get_bg_color()
  {
    return this->bg_color;
  }
  
  uint32_t Gl_window::get_max_refresh_rate() const
  {
    return this->max_refresh_rate;
  }
  
  void Window::set_event_callback_function(const std::function<void(Event &)> &callback_function)
  {
    Window::event_callback_function = callback_function;
  }
  
  void Gl_window::set_native_window(void *window_)
  {
    this->window = window_;
  }
  
  void Gl_window::set_native_monitor(void *monitor_)
  {
    this->monitor = monitor_;
  }
  
  void Gl_window::set_title(const char *title_)
  {
    this->monitor_properties.title = title_;
    if (get_native_window())
    {
      glfw_call(glfwSetWindowTitle(static_cast<GLFWwindow *>(get_native_window()), title_));
    }
  }
  
  bool Gl_window::glfw_is_init() const
  {
    return this->glfw_init;
  }
  
  bool Gl_window::is_focused() const
  {
    bool focus;
    glfw_call(focus = glfwGetWindowAttrib(static_cast<GLFWwindow *>(get_native_window()), GLFW_FOCUSED));
    return focus;
  }
  
  bool Gl_window::is_hovered() const
  {
    bool hover;
    glfw_call(hover = glfwGetWindowAttrib(static_cast<GLFWwindow *>(get_native_window()), GLFW_HOVERED));
    return hover;
  }
  
  void Gl_window::set_glfw_init_status(bool status)
  {
    this->glfw_init = status;
  }
  
  void Gl_window::set_width(float width_)
  {
    this->monitor_properties.width = width_;
  }
  
  void Gl_window::set_height(float height_)
  {
    this->monitor_properties.height = height_;
  }
  
  void Gl_window::set_aspect_ratio(const Vector_2f &aspect_ratio_)
  {
    this->aspect_ratio = aspect_ratio_;
    glfw_call(glfwSetWindowAspectRatio(static_cast<GLFWwindow *>(get_native_window()),
                                       static_cast<int32_t>(aspect_ratio_.get_x()),
                                       static_cast<int32_t>(aspect_ratio_.get_y())));
  }
  
  void Gl_window::resize(float width_, float height_)
  {
    Gl_window::set_width(width_);
    Gl_window::set_height(height_);
    Gl_window::set_aspect_ratio({width_,
                                 height_});
    Gl_renderer::on_window_resize(this, width_, height_);
  }
  
  void Gl_window::set_fullscreen(bool fullscreen_state)
  {
    this->fullscreen = fullscreen_state;
  }
  
  void Gl_window::set_refresh_rate(uint32_t refresh_rate_)
  {
    this->monitor_properties.refresh_rate = refresh_rate_;
  }
  
  void Gl_window::set_max_refresh_rate(uint32_t refresh_rate_)
  {
    this->max_refresh_rate = refresh_rate_;
  }
  
  void Gl_window::set_vsync(bool vsync_)
  {
    this->vsync = vsync_;
  }
  
  void Gl_window::set_window_pos(float x_pos_, float y_pos_)
  {
    if (x_pos_ > this->monitor_properties.width || y_pos_ > this->monitor_properties.height)
    {
      this->position_on_screen = {this->monitor_properties.width / 2,
                                  this->monitor_properties.height / 2};
      return;
    }
    this->position_on_screen = {x_pos_,
                                y_pos_};
  }
  
  void Gl_window::set_x_scale(float x_scale_)
  {
    this->x_scale = x_scale_;
  }
  
  void Gl_window::set_y_scale(float y_scale_)
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
  
  void Gl_window::glfw_error_callback(const char *function_name, const char *source_file,
                                      size_t line_number)
  {
    const char *_err_str;
    char buffer[FILENAME_MAX * 4] {0};
    if (glfwGetCurrentContext())
    {
      int32_t error_code = glfwGetError(&_err_str);
      if (error_code != GLFW_NO_ERROR)
      {
        if (snprintf(buffer, sizeof(buffer),
                     "%s,\n%48sIN FILE %s,\n%48sIN FUNCTION %s,\n%48sAT LINE %zu",
                     _err_str, DEFAULT, source_file, DEFAULT, function_name, DEFAULT, line_number) < 0)
        {
          On_context_error glfw_error({"Glfw error",
                                       "Fatal",
                                       _err_str,
                                       error_code}, Context_api::GLFW);
          return get_event_callback_function()(glfw_error);
        }
        On_context_error glfw_error({"Glfw error",
                                     "Fatal",
                                     buffer,
                                     error_code}, Context_api::GLFW);
        return get_event_callback_function()(glfw_error);
      }
    }
  }
}