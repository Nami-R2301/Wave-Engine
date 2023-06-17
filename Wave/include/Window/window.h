//
// Created by nami on 2023-02-23.
//

#pragma once

#if defined (GLFW3_STATIC)

#include <GLFW/glfw3.h>

#endif

#include <Events/event.h>
#include <Renderer/color.h>
#include <functional>

namespace Wave
{
  
  enum class Context_api_e
  {
    Glfw,
    Glut,
    Win32
  };
  
  typedef struct Api_info_s
  {
    int version_major;
    int version_minor;
    int version_revision;
  } Api_info;
  
  typedef struct Window_properties_s
  {
    const char *title = nullptr;
    float width = WAVE_VALUE_DONT_CARE;
    float height = WAVE_VALUE_DONT_CARE;
    bool vsync = true;
    int32_t red_bits = WAVE_VALUE_DONT_CARE;
    int32_t blue_bits = WAVE_VALUE_DONT_CARE;
    int32_t green_bits = WAVE_VALUE_DONT_CARE;
    int32_t refresh_rate = WAVE_VALUE_DONT_CARE;
    int32_t sample_rate = WAVE_VALUE_DONT_CARE;
  } Window_properties_s;
  
  class Window
  {
    public:
    Window() = default;
    virtual ~Window() = default;
    
    static std::unique_ptr<Window> create(Context_api_e api_chosen, Window_properties_s options);
    
    virtual void setup_api() const = 0;
    virtual void setup_monitor() = 0;
    virtual void create_window() = 0;
    virtual void poll_api_events() = 0;
    virtual void on_render() = 0;
    virtual void bind_api_callbacks() = 0;
    virtual void unbind_api_callback(const Event &event) = 0;
    virtual void toggle_fullscreen() = 0;
    virtual void close() = 0;
    virtual void shutdown() = 0;
    virtual void hide() = 0;
    virtual bool is_closing() = 0;
    
    [[nodiscard]] virtual Context_api_e get_context() const = 0;
    [[nodiscard]] virtual std::string get_context_version() const = 0;
    [[nodiscard]] virtual void *get_native_window() const = 0;
    [[nodiscard]] virtual void *get_native_monitor() const = 0;
    [[nodiscard]] virtual const char *get_title() const = 0;
    [[nodiscard]] virtual bool is_fullscreen() const = 0;
    [[nodiscard]] virtual const Vector_2f &get_window_pos() const = 0;
    [[nodiscard]] virtual int get_framebuffer_width() const = 0;
    [[nodiscard]] virtual int get_framebuffer_height() const = 0;
    [[nodiscard]] virtual int get_width() const = 0;
    [[nodiscard]] virtual int get_height() const = 0;
    [[nodiscard]] virtual const Vector_2f &get_aspect_ratio() const = 0;
    [[nodiscard]] virtual int32_t get_refresh_rate() const = 0;
    [[nodiscard]] virtual int32_t get_max_refresh_rate() const = 0;
    [[nodiscard]] virtual int32_t get_samples() const = 0;
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
    virtual void set_aspect_ratio(const Vector_2f &aspect_ratio_) = 0;
    virtual void resize(float width_, float height_) = 0;
    virtual void set_fullscreen(bool fullscreen_state) = 0;
    virtual void set_refresh_rate(int32_t refresh_rate) = 0;
    virtual void set_max_refresh_rate(int32_t refresh_rate) = 0;
    virtual void set_vsync(bool vsync) = 0;
    virtual void set_window_pos(float x_pos_, float y_pos_) = 0;
    virtual void set_x_scale(float x_scale_) = 0;
    virtual void set_y_scale(float y_scale_) = 0;
    
    [[nodiscard]] virtual float get_x_scale() const = 0;
    [[nodiscard]] virtual float get_y_scale() const = 0;
    
    virtual bool operator==(const Window &window);
    virtual bool operator!=(const Window &window);
  };
}