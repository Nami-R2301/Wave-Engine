#pragma once

#include "ImGUI/imGUI_layer.h"
#include <Core/core_macros.h>
#include <Core/settings.h>
#include <Core/layer_stack.h>

#include <Window/window.h>
#include <Renderer/renderer.h>
#include <Input/input.h>

#include <Events/key_event.h>
#include <Events/mouse_event.h>
#include <Events/app_event.h>

namespace Wave
{
  
  class Engine
  {
  public:
    Engine();
    explicit Engine(Renderer_api choice);
    virtual ~Engine();

    [[nodiscard]] static Engine *get_app();
    [[nodiscard]] static Window *get_main_window();  // For glfw purposes, keep this static.
    [[nodiscard]] long get_frame_drawn_counter() const;
    [[nodiscard]] static float get_engine_framerate();
    [[nodiscard]] bool get_running_state() const;
    [[nodiscard]] uint32_t get_exit_status() const;
    
    static void set_engine_framerate(float window_framerate);
    void set_frame_drawn_counter(long counter);
    void set_running_state(bool new_state);
    void set_exit_status(uint32_t code);
    
    virtual void init();
    virtual void on_event(Event &event);
    virtual void on_update(float time_step);
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
    
    void shutdown();
    [[nodiscard]] bool has_crashed() const;
    static void wait(float start_time, float end_time);
  private:
    static Engine *instance;
    bool running_state = false;
    bool is_minimized = false;
    long frame_drawn_counter = 0;
    static float engine_framerate;
    Engine_time current_time;
    uint32_t exit_code = PROCESSING;  // Document the program_id exit code for maintenance.
  protected:
    Vector_2f last_mouse_position;  // Useful for determining mouse position delta between the last frame and the current one.
    static std::unique_ptr<Window> main_window;  // Default to OpenGL implementation.
    Layer_stack layer_stack;  // List containing all of our app layers.
    ImGui_layer *imgui_layer = nullptr;
  private:
    std::future<void> demo_futures;  // For async job handling.
  };
  
  Engine *create_app();
}
