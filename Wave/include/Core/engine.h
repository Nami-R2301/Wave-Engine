#pragma once

#include <Renderer/renderer.h>

#include <ImGUI/imGUI_layer.h>
#include <Core/settings.h>
#include <Layer/layer_stack.h>

#include <Window/window.h>
#include <Input/input.h>

#include <Events/key_event.h>
#include <Events/mouse_event.h>
#include <Events/app_event.h>

namespace Wave
{
  
  class Engine : public Loadable, public Destroyable
  {
    public:
    enum class App_type
    {
      Runtime = 0,
      Editor = 1,
      Testing = 2
    };
    public:
    Engine();
    explicit Engine(Renderer_api renderer_api_choice, Context_api_e context_api_choice, App_type executable_type_);
    ~Engine() override = default;
    
    [[nodiscard]] static Engine *get_app();
    [[nodiscard]] static Window *get_main_window();  // For glfw purposes, keep this static.
    [[nodiscard]] static float get_time_step();
    [[nodiscard]] static long get_frame_drawn_counter();
    [[nodiscard]] static float get_engine_framerate();
    [[nodiscard]] static bool get_running_state();
    [[nodiscard]] static int32_t get_exit_status();
    
    static void set_engine_framerate(float window_framerate);
    static void set_frame_drawn_counter(long counter);
    static void set_running_state(bool new_state);
    static void set_exit_status(int32_t code);
    
    INTERFACE_LOADABLE
    INTERFACE_DESTROYABLE
    
    virtual void on_event(Event &event);
    virtual void on_update(float time_step);
    virtual void on_render();
    void run();
    
    void push_overlay(Layer *layer);
    void pop_overlay(Layer *layer);
    
    void push_layer(Layer *layer);
    void pop_layer(Layer *layer);
    
    // DEFAULT EVENT CALLBACKS.
    virtual bool any_key_callback(On_any_key_event &any_key);
    virtual bool key_press_callback(On_key_press &key_press);
    virtual bool key_hold_callback(On_key_hold &key_hold);
    virtual bool key_release_callback(On_key_release &key_release);
    
    virtual bool mouse_button_press_callback(On_mouse_button_press &mouse_button_press);
    virtual bool mouse_button_hold_callback(On_mouse_button_hold &mouse_button_hold);
    virtual bool mouse_button_release_callback(On_mouse_button_release &mouse_button_release);
    virtual bool mouse_wheel_callback(On_mouse_wheel_scroll &mouse_wheel_input);
    virtual bool mouse_movement_callback(On_mouse_movement &mouse_cursor_event);
    
    virtual bool window_closed_callback(On_window_close &window_closed_event);
    virtual bool window_resize_callback(On_window_resize &resize_event);
    
    static void shutdown();
    [[nodiscard]] static bool has_crashed();
    static void wait(float start_time, float end_time);
    private:
    bool loaded = false;
    static Engine *instance;
    static App_type executable_type;
    static bool running_state;
    static float time_step;
    static long frame_drawn_counter;
    static float engine_framerate;
    static Engine_time current_time;
    static int32_t exit_code;  // Document the program_id exit code for maintenance.
    [[maybe_unused]] static Vector_2f last_mouse_position;  // Useful for determining mouse position delta between the last frame and the current one.
    static std::unique_ptr<Window> main_window;  // Default to OpenGL implementation.
    Layer_stack layer_stack;  // List containing all of our app layers.
    private:
    std::future<void> demo_futures;  // For async job handling.
  };
  
  Engine *create_app();
}
