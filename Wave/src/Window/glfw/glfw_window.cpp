//
// Created by nami on 05/05/23.
//

#include <Renderer/gl_renderer.h>
#include <Window/glfw/glfw_window.h>
#include <Events/key_event.h>
#include <Events/mouse_event.h>
#include <Events/app_event.h>

namespace Wave
{
  
  std::function<void(Event &event)> Glfw_window::event_callback_function;
  
  Glfw_window::Glfw_window(Window_properties_s options) : Window()
  {
    glfwSetErrorCallback(glfw_error_callback);
    this->window_properties = options;
    WAVE_LOG_TASK("GLFW window", PURPLE, 2, "Preparing app window ...", Glfw_window::create_window(),
                  "App window loaded") // Make glfw context.
  }
  
  Glfw_window::~Glfw_window()
  {
    WAVE_LOG_TASK("GLFW window", PURPLE, 2, "Closing window ...", Glfw_window::shutdown(), "GLFW window closed")
  }
  
  void Glfw_window::create_window()
  {
    // Init glfw library.
    WAVE_LOG_INSTRUCTION("GLFW window", DEFAULT, "Making openGL context", glfwInit())
    this->set_glfw_init_status(true);
    
    glfwGetVersion(&this->api_info.version_major, &this->api_info.version_minor, &this->api_info.version_revision);
    // So that we will only use the modern functions in it.
    Wave::alert(WAVE_LOG_INFO, "[GLFW window] --> Api (GLFW) version : %d.%d.%d",
                this->api_info.version_major, this->api_info.version_minor,
                this->api_info.version_revision);
    WAVE_LOG_TASK("GLFW window", RED, 2, "Setting up monitor...", this->setup_monitor(), "Monitor loaded and set")
    WAVE_LOG_INSTRUCTION("GLFW window", DEFAULT, "Setting up window api", this->setup_api())
    // Generate a pointer to a window using our monitor info, so that we later hide it.
    WAVE_LOG_INSTRUCTION("GLFW window", DEFAULT, "Creating window (windowed mode)",
                         GLFWwindow *window_ = glfwCreateWindow(this->get_width(), this->get_height(),
                                                                this->get_title(),
                           nullptr, nullptr)) // Windowed mode.
    this->set_native_window(window_);
    if (!this->window)
    {
      Wave::alert(WAVE_LOG_ERROR,
                  "[GLFW window] --> One or multiple core extensions for GLFW are not supported!\tExiting ...");
    }
    
    glfwMakeContextCurrent(static_cast<GLFWwindow *>(this->get_native_window())); // Show our window.
    glfwSetWindowAspectRatio(static_cast<GLFWwindow *>(this->get_native_window()), 16, 9);
    glfwSetInputMode(static_cast<GLFWwindow *>(this->get_native_window()), GLFW_CURSOR,
                     GLFW_CURSOR_NORMAL); // Set our mouse cursor to default.
    glfwSetInputMode(static_cast<GLFWwindow *>(this->get_native_window()), GLFW_STICKY_KEYS, GLFW_TRUE);
    
    glfwSetInputMode(static_cast<GLFWwindow *>(this->get_native_window()), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    int x_pos_, y_pos_;
    glfwGetWindowPos(static_cast<GLFWwindow *>(this->get_native_window()), &x_pos_,
                     &y_pos_);  // Get initial window position.
    this->set_window_pos(static_cast<float>(x_pos_), static_cast<float>(y_pos_));
    glfwSetWindowUserPointer(static_cast<GLFWwindow *>(this->get_native_window()), reinterpret_cast<void *>(this));
  }
  
  void Glfw_window::setup_api() const
  {
#if defined(OPENGL_VERSION_4_3_PLUS) && defined(DEBUG)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
    if (this->window_properties.sample_rate > 0) glfwWindowHint(GLFW_SAMPLES, this->window_properties.sample_rate);
    glfwWindowHint(GLFW_RED_BITS, this->window_properties.red_bits);
    glfwWindowHint(GLFW_GREEN_BITS, this->window_properties.green_bits);
    glfwWindowHint(GLFW_BLUE_BITS, this->window_properties.blue_bits);
    glfwWindowHint(GLFW_REFRESH_RATE, this->window_properties.refresh_rate);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
  }
  
  void Glfw_window::setup_monitor()
  {
    int monitor_count;
    GLFWmonitor **monitors = glfwGetMonitors(&monitor_count);
    if (monitors) this->set_native_monitor(monitors[0]); // Get main monitor specs.
    const GLFWvidmode *mode = glfwGetVideoMode(
      static_cast<GLFWmonitor *>(this->get_native_monitor())); // Get video specs of monitor.
    if (this->window_properties.width == WAVE_VALUE_DONT_CARE) this->set_width(static_cast<float>(mode->width));
    if (this->window_properties.height == WAVE_VALUE_DONT_CARE) this->set_height(static_cast<float>(mode->height));
    if (this->window_properties.refresh_rate == WAVE_VALUE_DONT_CARE) this->set_refresh_rate(mode->refreshRate);
    this->set_max_refresh_rate(mode->refreshRate);
    if (!this->window_properties.title) this->set_title("No title");
    this->set_vsync(this->window_properties.vsync);
    
    float x_scale_, y_scale_;
    glfwGetMonitorContentScale(static_cast<GLFWmonitor *>(this->get_native_monitor()), &x_scale_,
                               &y_scale_);  // Get monitor scale.
    this->set_x_scale(x_scale_);
    this->set_y_scale(y_scale_);
  }
  
  void Glfw_window::poll_api_events()
  {
    glfwPollEvents();
  }
  
  void Glfw_window::on_render()
  {
    if (this->is_closing())
    {
      On_window_close window_closed;
      Glfw_window::get_event_callback_function()(window_closed);
      return;
    }
    // Refresh framebuffer.
    glfwSwapBuffers(static_cast<GLFWwindow *>(this->get_native_window()));
    glfwSwapInterval(this->window_properties.vsync);  // Disable/enable Vertical synchronisation (Vsync).
  }
  
  void Glfw_window::bind_api_callbacks()
  {
    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                   []([[maybe_unused]] GLFWwindow *window_, int32_t width, int32_t height)
                                   {
                                     On_window_resize window_resized(static_cast<float>(width),
                                                                     static_cast<float>(height));
                                     Glfw_window::get_event_callback_function()(window_resized);
                                   });
    
    
    glfwSetWindowIconifyCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                                 [](GLFWwindow *window_, int minimized)
                                 {
                                   Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(
                                     window_);
                                   if (minimized)
                                   {
                                     // Avoid working at max fps when minimized.
                                     this_window_instance.set_refresh_rate(30.0f);
                                   } else
                                   {
                                     this_window_instance.set_refresh_rate(
                                       this_window_instance.get_max_refresh_rate());
                                   }
                                 });
    
    glfwSetKeyCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                       []([[maybe_unused]] GLFWwindow *window_, int32_t key, [[maybe_unused]] int32_t scancode,
                          int32_t action, [[maybe_unused]] int32_t mods)
                       {
                         switch (action)
                         {
                           case GLFW_PRESS:
                           {
                             On_key_press key_pressed(key);
                             Glfw_window::get_event_callback_function()(key_pressed);
                             break;
                           }
                           
                           case GLFW_REPEAT:
                           {
                             On_key_hold key_held(key);
                             Glfw_window::get_event_callback_function()(key_held);
                             break;
                           }
                           
                           case GLFW_RELEASE:
                           {
                             On_key_release key_released(key);
                             Glfw_window::get_event_callback_function()(key_released);
                             break;
                           }
                           
                           default:
                           {
                             On_any_key_event any_key;
                             Glfw_window::get_event_callback_function()(any_key);
                             break;
                           }
                         }
                       });
    glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                               []([[maybe_unused]] GLFWwindow *window_, int32_t button, int32_t action,
                                  [[maybe_unused]] int32_t mods)
                               {
                                 switch (action)
                                 {
                                   case GLFW_PRESS:
                                   {
                                     On_mouse_button_press mouse_btn_pressed(button);
                                     Glfw_window::get_event_callback_function()(mouse_btn_pressed);
                                     break;
                                   }
                                   
                                   case GLFW_REPEAT:
                                   {
                                     On_mouse_button_hold mouse_btn_held(button);
                                     Glfw_window::get_event_callback_function()(mouse_btn_held);
                                     break;
                                   }
                                   
                                   case GLFW_RELEASE:
                                   {
                                     On_mouse_button_release mouse_btn_released(button);
                                     Glfw_window::get_event_callback_function()(mouse_btn_released);
                                     break;
                                   }
                                   
                                   default: break;
                                 }
                               });
//    glfwSetCursorPosCallback(static_cast<GLFWwindow *>(this->get_native_window()),
//
//                                       [](GLFWwindow *window_, double x, double y)
//                                       {
//                                         Window &this_window_instance = *(Window *) glfwGetWindowUserPointer(window_);
//                                         On_mouse_movement mouse_movement_event(
//                                             Vector_2f(static_cast<float>(x), static_cast<float>(y)));
//                                         Window::get_event_callback_function()(mouse_movement_event);
//                                       });
    glfwSetWindowSizeCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                              []([[maybe_unused]] GLFWwindow *window_, int32_t width_, int32_t height_)
                              {
                                On_window_resize window_resized(static_cast<float>(width_),
                                                                static_cast<float>(height_));
                                Glfw_window::get_event_callback_function()(window_resized);
                              });
    glfwSetScrollCallback(static_cast<GLFWwindow *>(this->get_native_window()),
                          []([[maybe_unused]] GLFWwindow *window_, double x_offset, double y_offset)
                          {
                            On_mouse_wheel_scroll wheel_input(
                              Vector_2f(static_cast<float>(x_offset),
                                        static_cast<float>(y_offset)));
                            Glfw_window::get_event_callback_function()(wheel_input);
                          });
  }
  
  void Glfw_window::unbind_api_callback(const Event &event)
  {
    // If any of the subcategories of keyboard is requested to be unbound, we are forced to unbind this single callback to undo all.
    if (event.get_category_flags() & Event_category::EVENT_CATEGORY_KEYBOARD)
    {
      glfwSetKeyCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr);
      return;
    }
    
    // If any of the subcategories of mouse button is requested to be unbound, we are forced to unbind this single callback to undo all.
    if (event.get_category_flags() & Event_category::EVENT_CATEGORY_MOUSE_BUTTON)
    {
      glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr);
      return;
    }
    
    switch (event.get_event_type())
    {
      case Event_type::On_mouse_movement:
      {
        glfwSetCursorPosCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr);
        break;
      }
      case Event_type::On_window_resize:
      {
        glfwSetWindowSizeCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr);
        break;
      }
      case Event_type::On_mouse_wheel_scroll:
      {
        glfwSetScrollCallback(static_cast<GLFWwindow *>(this->get_native_window()), nullptr);
        break;
      }
      default:return;
    }
  }
  
  bool Glfw_window::is_minimized() const
  {
    return glfwGetWindowAttrib(static_cast<GLFWwindow *>(this->get_native_window()), GLFW_ICONIFIED);
  }
  
  // Let the window open as long as the close flag (gathered by glfwPollEvents) is not set to true.
  bool Glfw_window::is_closing()
  {
    if (this->glfw_init)
    {
      return
        glfwWindowShouldClose(static_cast<GLFWwindow *>(get_native_window())) != 0 || false |
                                                                                      this->request_closing;
    }
    return false;
  }
  
  // Show the window created.
  void Glfw_window::hide()
  {
    glfwHideWindow(static_cast<GLFWwindow *>(this->get_native_window())); // Hide our window.
  }
  
  void Glfw_window::toggle_fullscreen()
  {
    if (!Glfw_window::is_fullscreen())
    {
      alert(WAVE_LOG_INFO, "[WINDOW] --> Set window position to : (%d, %d)",
            this->get_window_pos().get_x(), this->get_window_pos().get_y());
      
      this->set_native_monitor(glfwGetPrimaryMonitor());
      const GLFWvidmode *mode = glfwGetVideoMode(static_cast<GLFWmonitor *>(get_native_monitor()));
      
      glfwSetWindowMonitor(static_cast<GLFWwindow *>(this->get_native_window()),
                           static_cast<GLFWmonitor *>(get_native_monitor()),
                           0, 0,
                           mode->width, mode->height, mode->refreshRate);
      this->set_fullscreen(true);
      this->set_width(static_cast<float>(mode->width));
      this->set_height(static_cast<float>(mode->height));
    } else
    {
      int width_ = 0, height_ = 0;
      glfwGetWindowSize(static_cast<GLFWwindow *>(this->get_native_window()), &width_, &height_);
      glfwSetWindowMonitor(static_cast<GLFWwindow *>(this->get_native_window()), nullptr,
                           static_cast<int32_t>(this->get_window_pos().get_x()),
                           static_cast<int32_t>(this->get_window_pos().get_y()),
                           width_, height_,
                           this->get_refresh_rate());  // Set maximum update rate possible.
      this->set_fullscreen(false);
      this->set_width(static_cast<float>(width_));
      this->set_height(static_cast<float>(height_));
    }
    Gl_renderer::set_viewport(static_cast<float>(this->get_width()),
                              static_cast<float>(this->get_height()));
  }
  
  // Signal that the window is closing.
  void Glfw_window::close()
  {
    glfwSetWindowShouldClose(static_cast<GLFWwindow *>(get_native_window()), GLFW_TRUE);
    this->request_closing = true;
  }
  
  void Glfw_window::shutdown()
  {
    if (this->glfw_init)
    {
      WAVE_LOG_INSTRUCTION("GLFW window", DEFAULT, "Closing window", this->close())
      
      glfwDestroyWindow(static_cast<GLFWwindow *>(get_native_window()));
      glfwTerminate();
    }
    this->glfw_init = false;
  }
  
  Context_api_e Glfw_window::get_context() const
  {
    return this->context_api;
  }
  
  std::string Glfw_window::get_context_version() const
  {
    return glfwGetVersionString();
  }
  
  void *Glfw_window::get_native_window() const
  {
    return this->window;
  }
  
  void *Glfw_window::get_native_monitor() const
  {
    return this->monitor;
  }
  
  const char *Glfw_window::get_title() const
  {
    return this->window_properties.title;
  }
  
  int Glfw_window::get_framebuffer_width() const
  {
    if (!this->glfw_init) return 0;
    int width;
    glfwGetFramebufferSize((GLFWwindow *) this->window, &width, nullptr);
    return width;
  }
  
  int Glfw_window::get_framebuffer_height() const
  {
    if (!this->glfw_init) return 0;
    int height;
    glfwGetFramebufferSize((GLFWwindow *) this->window, nullptr, &height);
    return height;
  }
  
  int Glfw_window::get_width() const
  {
    return this->window_properties.width;
  }
  
  int Glfw_window::get_height() const
  {
    return this->window_properties.height;
  }
  
  const Vector_2f &Glfw_window::get_aspect_ratio() const
  {
    return this->aspect_ratio;
  }
  
  int32_t Glfw_window::get_refresh_rate() const
  {
    return this->window_properties.refresh_rate;
  }
  
  bool Glfw_window::is_fullscreen() const
  {
    return this->fullscreen;
  }
  
  bool Glfw_window::is_vsync() const
  {
    return this->window_properties.vsync;
  }
  
  const Vector_2f &Glfw_window::get_window_pos() const
  {
    return this->position_on_screen;
  }
  
  float Glfw_window::get_x_scale() const
  {
    return this->x_scale;
  }
  
  float Glfw_window::get_y_scale() const
  {
    return this->y_scale;
  }
  
  int32_t Glfw_window::get_max_refresh_rate() const
  {
    return this->max_refresh_rate;
  }
  
  int32_t Glfw_window::get_samples() const
  {
    return this->window_properties.sample_rate;
  }
  
  const std::function<void(Event &event)> &Glfw_window::get_event_callback_function()
  {
    return Glfw_window::event_callback_function;
  }
  
  void Glfw_window::set_event_callback_function(const std::function<void(Event &)> &callback_function)
  {
    Glfw_window::event_callback_function = callback_function;
  }
  
  void Glfw_window::set_native_window(void *window_)
  {
    this->window = window_;
  }
  
  void Glfw_window::set_native_monitor(void *monitor_)
  {
    this->monitor = monitor_;
  }
  
  void Glfw_window::set_title(const char *title_)
  {
    if (get_native_window())
    {
      glfwSetWindowTitle(static_cast<GLFWwindow *>(get_native_window()), title_);
    }
  }
  
  bool Glfw_window::glfw_is_init() const
  {
    return this->glfw_init;
  }
  
  bool Glfw_window::is_focused() const
  {
    bool focus;
    (focus = glfwGetWindowAttrib(static_cast<GLFWwindow *>(get_native_window()), GLFW_FOCUSED));
    return focus;
  }
  
  bool Glfw_window::is_hovered() const
  {
    bool hover;
    (hover = glfwGetWindowAttrib(static_cast<GLFWwindow *>(get_native_window()), GLFW_HOVERED));
    return hover;
  }
  
  void Glfw_window::set_glfw_init_status(bool status)
  {
    this->glfw_init = status;
  }
  
  void Glfw_window::set_width(float width_)
  {
    this->window_properties.width = width_;
  }
  
  void Glfw_window::set_height(float height_)
  {
    this->window_properties.height = height_;
  }
  
  void Glfw_window::set_aspect_ratio(const Vector_2f &aspect_ratio_)
  {
    this->aspect_ratio = aspect_ratio_;
    (glfwSetWindowAspectRatio(static_cast<GLFWwindow *>(get_native_window()),
                              static_cast<int32_t>(aspect_ratio_.get_x()),
                              static_cast<int32_t>(aspect_ratio_.get_y())));
  }
  
  void Glfw_window::resize(float width_, float height_)
  {
    Glfw_window::set_width(width_);
    Glfw_window::set_height(height_);
    Glfw_window::set_aspect_ratio({width_,
                                   height_});
    Gl_renderer::set_viewport(width_, height_);
  }
  
  void Glfw_window::set_fullscreen(bool fullscreen_state)
  {
    this->fullscreen = fullscreen_state;
  }
  
  void Glfw_window::set_refresh_rate(int32_t refresh_rate_)
  {
    if (refresh_rate_ == WAVE_VALUE_DONT_CARE)
    {
      this->window_properties.refresh_rate = 0;
      return;
    }
    if (refresh_rate_ > 0) this->window_properties.refresh_rate = refresh_rate_;
  }
  
  void Glfw_window::set_max_refresh_rate(int32_t refresh_rate_)
  {
    this->max_refresh_rate = refresh_rate_;
  }
  
  void Glfw_window::set_vsync(bool vsync_)
  {
    if (!vsync_) Glfw_window::set_refresh_rate(WAVE_VALUE_DONT_CARE);
    else
    {
      const GLFWvidmode *mode =
        glfwGetVideoMode((GLFWmonitor *) this->get_native_monitor()); // Get video specs of monitor.
      Glfw_window::set_refresh_rate(mode->refreshRate);
    }
    this->window_properties.vsync = vsync_;
  }
  
  void Glfw_window::set_window_pos(float x_pos_, float y_pos_)
  {
    if (x_pos_ > this->window_properties.width || y_pos_ > this->window_properties.height)
    {
      this->position_on_screen = {this->window_properties.width / 2,
                                  this->window_properties.height / 2};
      return;
    }
    this->position_on_screen = {x_pos_,
                                y_pos_};
  }
  
  void Glfw_window::set_x_scale(float x_scale_)
  {
    this->x_scale = x_scale_;
  }
  
  void Glfw_window::set_y_scale(float y_scale_)
  {
    this->y_scale = y_scale_;
  }
  
  bool Window::operator==(const Wave::Window &window_)
  {
    if (this == &window_) return true;
    return false;
  }
  
  bool Window::operator!=(const Wave::Window &window_)
  {
    if (this == &window_) return false;
    return true;
  }
  
  bool Glfw_window::operator!=(const Glfw_window &window_)
  {
    if (this == &window_) return false;
    return static_cast<GLFWwindow *>(get_native_window()) != static_cast<GLFWwindow *>(window_.get_native_window());
  }
  
  bool Glfw_window::operator==(const Glfw_window &window_)
  {
    if (this == &window_) return true;
    return static_cast<GLFWwindow *>(get_native_window()) == static_cast<GLFWwindow *>(window_.get_native_window());
  }
  
  void Glfw_window::glfw_error_callback(int error_code, const char *description)
  {
    char buffer[FILENAME_MAX * 4]{0};
    if (glfwGetCurrentContext())
    {
      if (snprintf(buffer, sizeof(buffer), "%s,\n%48sCode --> %d", description, DEFAULT, error_code) < 0)
      {
        On_context_error glfw_error({"Glfw error",
                                     "Fatal",
                                     description,
                                     error_code}, Context_api_e::Glfw);
        return get_event_callback_function()(glfw_error);
      }
      On_context_error glfw_error({"Glfw error",
                                   "Fatal",
                                   buffer,
                                   error_code}, Context_api_e::Glfw);
      return get_event_callback_function()(glfw_error);
    }
  }
} // Wave