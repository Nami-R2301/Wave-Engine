//
// Created by nami on 2023-02-23.
//

#pragma once

#include <Events/event_dispatcher.h>
#include <Renderer/color.h>

namespace Wave
{
  
  enum class Context_api
  {
    OPENGL,
    VULKAN,
    DIRECTX12
  };
  
  typedef struct Api_info
  {
    int version_major;
    int version_minor;
    int version_revision;
  } Api_info;
  
  typedef struct Window_pos
  {
    uint32_t x_pos;
    uint32_t y_pos;
  } Window_pos;
  
  typedef struct Monitor_properties
  {
    const char *title = "Main Window";
    uint32_t width = 0;
    uint32_t height = 0;
    int32_t red_bits = 0;
    int32_t blue_bits = 0;
    int32_t green_bits = 0;
    uint32_t refresh_rate = 30;
  } Monitor_properties;
  
  class Window
  {
  public:
    Window() = default;
    Window(const Window &window_);
    explicit Window(Context_api context_chosen);
    virtual ~Window() = default;
    
    virtual void setup_api();
    virtual void setup_monitor();
    virtual void create_window();
    virtual void on_update();
    virtual void bind_api_callbacks() = 0;
    virtual void unbind_api_callback(const Event &event) = 0;
    virtual void toggle_fullscreen();
    virtual bool is_closing();
    virtual void close();
    virtual void shutdown() = 0;
    virtual void hide();
    
    [[nodiscard]] Context_api get_context() const;
    [[nodiscard]] const std::function<void(Event &event)> &get_event_callback_function() const;
    [[nodiscard]] void *get_native_window() const;
    [[nodiscard]] void *get_monitor() const;
    [[nodiscard]] const char *get_title() const;
    [[nodiscard]] bool is_fullscreen() const;
    [[nodiscard]] uint32_t get_width() const;
    [[nodiscard]] uint32_t get_height() const;
    [[nodiscard]] uint32_t get_refresh_rate() const;
    [[nodiscard]] uint32_t get_max_refresh_rate() const;
    [[nodiscard]] virtual bool is_vsync() const;
    [[nodiscard]] virtual bool is_focused() const;
    [[nodiscard]] virtual bool is_hovered() const;
    [[nodiscard]] virtual Window_pos get_window_pos() const;
    
    void set_event_callback_function(const std::function<void(Event &)> &callback_function);
    void set_native_window(void *window_);
    void set_native_monitor(void *monitor_);
    virtual void set_title(const char *title);
    virtual void set_width(uint32_t width);
    virtual void set_height(uint32_t height);
    virtual void set_fullscreen(bool fullscreen_state);
    virtual void set_refresh_rate(uint32_t refresh_rate);
    virtual void set_max_refresh_rate(uint32_t refresh_rate);
    virtual void set_vsync(bool vsync);
    virtual void set_window_pos(uint32_t x_pos_, uint32_t y_pos_);
    virtual void set_x_scale(float x_scale_);
    virtual void set_y_scale(float y_scale_);
    
    [[nodiscard]] float get_x_scale() const;
    [[nodiscard]] float get_y_scale() const;
    [[nodiscard]] float *get_bg_color();
    
    virtual bool operator ==(const Window &window);
    virtual bool operator !=(const Window &window);
  protected:
    Context_api context_api = Context_api::OPENGL;
    void *window = nullptr;
    void *monitor = nullptr;
    Monitor_properties properties {};
    bool vsync = false;
    uint32_t max_refresh_rate = 30;  // Set max possible fps value for primary display, useful for frame limiting.
    
    // Default Window attributes (Center).
    Window_pos position_on_screen = {this->properties.width / 2,
                                     this->properties.height / 2};
    
    float x_scale = 1.0f;
    float y_scale = 1.0f;
    Color bg_color = Color(0.03f, 1.0f, true);  // Default background color (gray).
    Vector_4f c_color = {bg_color.get_red(),
                         bg_color.get_green(),
                         bg_color.get_blue(),
                         1.0f};
    bool fullscreen = false;
    bool request_closing = false;
    Api_info api_info {};
    std::function<void(Event &)> event_callback_function;
  };
  
  class Gl_window : public Window
  {
  public:
    Gl_window();
    ~Gl_window() override = default;
    
    void setup_api() override;
    void setup_monitor() override;
    void create_window() override;
    void on_update() override;
    void bind_api_callbacks() override;
    void unbind_api_callback(const Event &event) override;
    void toggle_fullscreen() override;
    // Let the window open as long as the close flag (gathered by glfwPollEvents) is not set to true.
    bool is_closing() override;
    void close() override;  // Free resources and close the window.
    void shutdown() override;
    void hide() override;  // Show the window created.
    static void glfw_error_callback(const char *function_name, const char *source_file, size_t line_number);
    
    // Return a glfwWindow instance in case there's a need to manipulate the window directly.
    [[nodiscard]] bool glfw_is_init() const;
    [[nodiscard]] void *get_gl_window() const;
    [[nodiscard]] void *get_gl_monitor() const;
    [[nodiscard]] bool is_focused() const override;
    [[nodiscard]] bool is_hovered() const override;
    
    void set_glfw_init_status(bool status);
    void set_gl_window(void *window);
    void set_gl_monitor(void *monitor);
    void set_title(const char *title_) override;
    
    bool operator !=(const Gl_window &window_);
    bool operator ==(const Gl_window &window_);
  private:
    bool glfw_init = false;  // Flag to check glfw prepare_mesh status.
  };
}