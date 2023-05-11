//
// Created by nami on 05/05/23.
//

#pragma once

#include <Window/window.h>

#if defined (GLFW3_STATIC)

namespace Wave
{
  
  class Glfw_window : public Window
  {
    public:
    Glfw_window();
    explicit Glfw_window(Context_api api_chosen);
    ~Glfw_window() override;
    
    void setup_api() const override;
    void setup_monitor() override;
    void create_window() override;
    void poll_api_events() override;
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
    [[nodiscard]] const Vector_2f &get_aspect_ratio() const override;
    [[nodiscard]] int32_t get_refresh_rate() const override;
    [[nodiscard]] int32_t get_max_refresh_rate() const override;
    [[nodiscard]] bool is_vsync() const override;
    [[nodiscard]] bool is_minimized() const override;
    [[nodiscard]] bool is_focused() const override;
    [[nodiscard]] bool is_hovered() const override;
    [[nodiscard]] float get_x_scale() const override;
    [[nodiscard]] float get_y_scale() const override;
    [[nodiscard]] const Vector_2f &get_window_pos() const override;
    
    static void glfw_error_callback(int error_code, const char *description);
    
    void set_glfw_init_status(bool status);
    void set_title(const char *title_) override;
    void set_native_window(void *window_) override;
    void set_native_monitor(void *monitor_) override;
    void set_width(float width) override;
    void set_height(float height) override;
    void set_aspect_ratio(const Vector_2f &aspect_ratio_) override;
    void resize(float width_, float height_) override;
    void set_fullscreen(bool fullscreen_state) override;
    void set_refresh_rate(int32_t refresh_rate) override;
    void set_max_refresh_rate(int32_t refresh_rate) override;
    void set_vsync(bool vsync) override;
    void set_window_pos(float x_pos_, float y_pos_) override;
    void set_x_scale(float x_scale_) override;
    void set_y_scale(float y_scale_) override;
    
    bool operator!=(const Glfw_window &window_);
    bool operator==(const Glfw_window &window_);
    private:
    bool glfw_init = false;  // Flag to check glfw prepare_mesh status.
    Context_api context_api = Context_api::Glfw;
    void *window = nullptr;
    void *monitor = nullptr;
    Monitor_properties monitor_properties{nullptr};
    bool vsync = false;
    int32_t max_refresh_rate = -1;  // Set max possible fps value for primary display, useful for frame limiting.
    
    // Default Window attributes (Center).
    Vector_2f position_on_screen;
    
    float x_scale = 1.0f;
    float y_scale = 1.0f;
    Vector_2f aspect_ratio = {16.0f,
                              9.0f};
    bool fullscreen = false;
    bool request_closing = false;
    Api_info api_info{};
  };
} // Wave

#else

#error "Cannot include GLFW context API, since it is not included! Did you forget to add GLFW_STATIC to your compile definitions?"

#endif
