//
// Created by nami on 2023-02-23.
//

#pragma once

#include "Math/vector.h"
#include "backends/imgui_impl_glfw.h"
#include <Events/event_dispatcher.h>
#include <Renderer/color.h>
#include <functional>

namespace Wave
{
  
  enum class Context_api
  {
    GLFW,
    GLUT,
    WIN32
  };
  
  typedef struct Api_info
  {
    int version_major;
    int version_minor;
    int version_revision;
  } Api_info;
  
  typedef struct Monitor_properties
  {
    const char *title = "Main Window";
    float width = 0;
    float height = 0;
    int32_t red_bits = 0;
    int32_t blue_bits = 0;
    int32_t green_bits = 0;
    uint32_t refresh_rate = 30.0f;
  } Monitor_properties;
  
  class Window
  {
  public:
    Window() = default;
    virtual ~Window() = default;
    
    virtual void setup_api() = 0;
    virtual void setup_monitor() = 0;
    virtual void create_window() = 0;
    virtual void on_update(float time_step) = 0;
    virtual void bind_api_callbacks() = 0;
    virtual void unbind_api_callback(const Event &event) = 0;
    virtual void toggle_fullscreen() = 0;
    virtual void close() = 0;
    virtual void shutdown() = 0;
    virtual void hide() = 0;
    virtual bool is_closing() = 0;
    
    [[nodiscard]] virtual Context_api get_context() const = 0;
    [[nodiscard]] virtual void *get_native_window() const = 0;
    [[nodiscard]] virtual void *get_native_monitor() const = 0;
    [[nodiscard]] virtual const char *get_title() const = 0;
    [[nodiscard]] virtual bool is_fullscreen() const = 0;
    [[nodiscard]] virtual const Vector_2f &get_window_pos() const = 0;
    [[nodiscard]] virtual float get_width() const = 0;
    [[nodiscard]] virtual float get_height() const = 0;
    [[nodiscard]] virtual uint32_t get_refresh_rate() const = 0;
    [[nodiscard]] virtual uint32_t get_max_refresh_rate() const = 0;
    [[nodiscard]] virtual bool is_vsync() const = 0;
    [[nodiscard]] virtual bool is_minimized() const = 0;
    [[nodiscard]] virtual bool is_focused() const = 0;
    [[nodiscard]] virtual bool is_hovered() const = 0;
    
    [[nodiscard]] static const std::function<void(Event &event)> &get_event_callback_function();
    static void set_event_callback_function(const std::function<void(Event &)> &callback_function);
    
    virtual void set_native_window(void *window_) = 0;
    virtual void set_native_monitor(void *monitor_) = 0;
    virtual void set_title(const char *title) = 0;
    virtual void set_width(float width) = 0;
    virtual void set_height(float height) = 0;
    virtual void set_fullscreen(bool fullscreen_state) = 0;
    virtual void set_refresh_rate(uint32_t refresh_rate) = 0;
    virtual void set_max_refresh_rate(uint32_t refresh_rate) = 0;
    virtual void set_vsync(bool vsync) = 0;
    virtual void set_window_pos(float x_pos_, float y_pos_) = 0;
    virtual void set_x_scale(float x_scale_) = 0;
    virtual void set_y_scale(float y_scale_) = 0;
    
    [[nodiscard]] virtual float get_x_scale() const = 0;
    [[nodiscard]] virtual float get_y_scale() const = 0;
    [[nodiscard]] virtual Color &get_bg_color() = 0;
    
    virtual bool operator ==(const Window &window);
    virtual bool operator !=(const Window &window);
  
  private:
    static std::function<void(Event &)> event_callback_function;
  };
  
  class Gl_window : public Window
  {
  public:
    Gl_window();
    explicit Gl_window(Context_api api_chosen);
    ~Gl_window() override;
    
    void setup_api() override;
    void setup_monitor() override;
    void create_window() override;
    void on_update(float time_step) override;
    void bind_api_callbacks() override;
    void unbind_api_callback(const Event &event) override;
    void toggle_fullscreen() override;
    // Let the window open as long as the close flag (gathered by glfwPollEvents) is not set to true.
    bool is_closing() override;
    void close() override;  // Free resources and close the window.
    void shutdown() override;
    void hide() override;  // Show the window created.
    
    // Return a glfwWindow instance in case there's a need to manipulate the window directly.
    [[nodiscard]] bool glfw_is_init() const;
    [[nodiscard]] Context_api get_context() const override;
    [[nodiscard]] void *get_native_window() const override;
    [[nodiscard]] void *get_native_monitor() const override;
    [[nodiscard]] const char *get_title() const override;
    [[nodiscard]] bool is_fullscreen() const override;
    [[nodiscard]] float get_width() const override;
    [[nodiscard]] float get_height() const override;
    [[nodiscard]] uint32_t get_refresh_rate() const override;
    [[nodiscard]] uint32_t get_max_refresh_rate() const override;
    [[nodiscard]] bool is_vsync() const override;
    [[nodiscard]] bool is_minimized() const override;
    [[nodiscard]] bool is_focused() const override;
    [[nodiscard]] bool is_hovered() const override;
    [[nodiscard]] float get_x_scale() const override;
    [[nodiscard]] float get_y_scale() const override;
    [[nodiscard]] Color &get_bg_color() override;
    [[nodiscard]] const Vector_2f &get_window_pos() const override;
    
    static void glfw_error_callback(const char *function_name, const char *source_file, size_t line_number);
    
    void set_glfw_init_status(bool status);
    void set_title(const char *title_) override;
    void set_native_window(void *window_) override;
    void set_native_monitor(void *monitor_) override;
    void set_width(float width) override;
    void set_height(float height) override;
    void set_fullscreen(bool fullscreen_state) override;
    void set_refresh_rate(uint32_t refresh_rate) override;
    void set_max_refresh_rate(uint32_t refresh_rate) override;
    void set_vsync(bool vsync) override;
    void set_window_pos(float x_pos_, float y_pos_) override;
    void set_x_scale(float x_scale_) override;
    void set_y_scale(float y_scale_) override;
    
    bool operator !=(const Gl_window &window_);
    bool operator ==(const Gl_window &window_);
  private:
    bool glfw_init = false;  // Flag to check glfw prepare_mesh status.
    Context_api context_api = Context_api::GLFW;
    void *window = nullptr;
    void *monitor = nullptr;
    Monitor_properties monitor_properties {nullptr};
    bool vsync = false;
    uint32_t max_refresh_rate = 30.0f;  // Set max possible fps value for primary display, useful for frame limiting.
    
    // Default Window attributes (Center).
    Vector_2f position_on_screen;
    
    float x_scale = 1.0f;
    float y_scale = 1.0f;
    Color bg_color = Color(0.03f, 1.0f, true);  // Default background color (gray).
    bool fullscreen = false;
    bool request_closing = false;
    Api_info api_info {};
    static std::function<void(Event &)> event_callback_function;
  };
}