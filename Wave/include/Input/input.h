//
// Created by nami on 11/12/22.
//

#pragma once

#include <Core/engine.h>

namespace Wave
{
  
  #define NUM_KEY_CODES  349  // 349 - first 32 chars.
  #define NUM_MOUSE_BUTTONS  8
  
  class Input
  {
  public:
    // KEY QUERY FUNCTIONS.
    static void set_key_state(int key_code, bool state);
    static bool is_key_pressed(int key_code);
    static bool is_key_held(int key_code);
    static bool is_key_released(int key_code);
    static bool is_key_pair_pressed(int first_key, int second_key);
    static bool is_key_pair_held(int first_key, int second_key);
    static std::string get_key_name(int key_code_);
    
    // MOUSE BUTTON QUERY FUNCTIONS.
    static bool is_mouse_button_pressed(int mouse_button);
    static bool is_mouse_button_held(int mouse_button);
    static bool is_mouse_button_released(int mouse_button);
    static void set_mouse_button_state(int mouse_button, bool state);
    
    // MOUSE MOVEMENT STATIC FUNCTIONS.
    static Vector_2f get_mouse_cursor_position();
    static bool is_mouse_cursor_within(const Object_3D &object, const Transform &transform);
    static int get_mouse_cursor_attribute();
    static void set_mouse_cursor_attribute(int value);
    static void set_mouse_cursor_position(const Vector_2f &position);
    
    // MOUSE STATE FUNCTIONS.
    [[nodiscard]] static bool is_mouse_locked();
    static void set_mouse_locked(bool value);
  private:
    static bool key_states[NUM_KEY_CODES];
    static bool mouse_button_states[NUM_MOUSE_BUTTONS];
    static bool mouse_locked;
  };
}