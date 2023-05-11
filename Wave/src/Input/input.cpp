//
// Created by nami on 11/12/22.
//

#include <Input/input.h>
#include <Core/engine.h>

namespace Wave
{
  
  bool Input::key_states[NUM_KEY_CODES];
  bool Input::mouse_button_states[NUM_MOUSE_BUTTONS];
  bool Input::mouse_locked = false;
  
  bool Input::is_key_pressed(int32_t key_code)
  {
    bool previous_key_state = Input::key_states[key_code];
    (Input::set_key_state(key_code,
                          glfwGetKey(static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()),
                                     key_code)));
    
    return previous_key_state != WAVE_PRESS && Input::key_states[key_code] == WAVE_PRESS;
  }
  
  bool Input::is_key_held(int32_t key_code)
  {
    bool previous_key_state = Input::key_states[key_code];  // Save previous key State.
    (
      Input::set_key_state(key_code,
                           glfwGetKey(static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()),
                                      key_code)));
    
    return previous_key_state == WAVE_PRESS && Input::key_states[key_code] != WAVE_RELEASE;
  }
  
  bool Input::is_key_released(int key_code)
  {
    bool previous_key_state = Input::key_states[key_code];  // Save previous key State.
    (
      Input::set_key_state(key_code,
                           glfwGetKey(static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()),
                                      key_code)));
    
    return previous_key_state == WAVE_PRESS && Input::key_states[key_code] == WAVE_RELEASE;
  }
  
  void Input::set_key_state(int key_code, bool state)
  {
    if (key_code < NUM_KEY_CODES) Input::key_states[key_code] = state;
  }
  
  bool Input::is_key_pair_pressed(int second_key, int first_key)
  {
    return
      // Pressed down first key while holding down second key.
      (Input::is_key_pressed(first_key) && Input::is_key_held(second_key)) ||
      // Pressed down second key while holding down first key.
      (Input::is_key_pressed(second_key) && Input::is_key_held(first_key));
  }
  
  bool Input::is_key_pair_held(int first_key, int second_key)
  {
    return Input::is_key_held(first_key) && Input::is_key_held(second_key); // Held down both keys.
  }
  
  std::string Input::get_key_name(int key_code_)
  {
    switch (key_code_)
    {
      case 32:return {"SPACE"};
      case 256:return {"ESCAPE"};
      case 257:return {"ENTER"};
      case 342:return {"LEFT-ALT"};
      default:char buffer[5];
        snprintf(buffer, 5, "%c", (char) key_code_);
        return {buffer};
    }
  }
  
  /************************ MOUSE INPUTS *************************/
  
  void Input::set_mouse_button_state(int mouse_button, bool state)
  {
    if (mouse_button < NUM_MOUSE_BUTTONS) Input::mouse_button_states[mouse_button] = state;
  }
  
  bool Input::is_mouse_button_pressed(int mouse_button)
  {
    // Mouse button was not pressed prior.
    bool previous_mouse_btn_state = mouse_button_states[mouse_button];  // Save previous mouse button State.
    (
      Input::set_mouse_button_state(mouse_button, glfwGetMouseButton(
        static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()),
        mouse_button)));
    
    return previous_mouse_btn_state == WAVE_RELEASE && Input::mouse_button_states[mouse_button] == WAVE_PRESS;
  }
  
  bool Input::is_mouse_button_held(int mouse_button)
  {
    bool previous_mouse_btn_state = Input::mouse_button_states[mouse_button];  // Save previous mouse button State.
    (
      Input::set_mouse_button_state(mouse_button, glfwGetMouseButton(
        static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()),
        mouse_button)));
    
    return previous_mouse_btn_state == WAVE_PRESS && Input::mouse_button_states[mouse_button] == WAVE_PRESS;
  }
  
  bool Input::is_mouse_button_released(int mouse_button)
  {
    bool previous_mouse_btn_state = Input::mouse_button_states[mouse_button];  // Save previous mouse button State.
    (
      Input::set_mouse_button_state(mouse_button, glfwGetMouseButton(
        static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()),
        mouse_button)));
    
    return previous_mouse_btn_state == WAVE_PRESS && Input::mouse_button_states[mouse_button] == WAVE_RELEASE;
  }
  
  void Input::set_mouse_cursor_position(const Vector_2f &position)
  {
    (
      glfwSetCursorPos(static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()), position.get_x(),
                       position.get_y()));
  }
  
  Vector_2f Input::get_mouse_cursor_position()
  {
    double position_x = 0.0f, position_y = 0.0f;
    glfwGetCursorPos(static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()), &position_x,
                     &position_y);
    return {(float) position_x,
            (float) position_y};
  }
  
  int Input::get_mouse_cursor_attribute()
  {
    int result =
      glfwGetInputMode(static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()),
                       GLFW_CURSOR);
    return result;
  }
  
  bool Input::is_mouse_locked()
  {
    return Input::mouse_locked;
  }
  
  void Input::set_mouse_locked(bool value)
  {
    Input::mouse_locked = value;
  }
  
  void Input::set_mouse_cursor_attribute(int value)
  {
    glfwSetInputMode(static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()), GLFW_CURSOR,
                     value);
  }

//  bool Input::is_mouse_cursor_within(const Object_3D &object, const Transform &transform)
//  {
//    double position_x = 0.0f, position_y = 0.0f;
//    (glfwGetCursorPos(static_cast<GLFWwindow *>(Engine::get_main_window()->get_native_window()), &position_x,
//                               &position_y));
//
//    for (size_t i = 0; i < object.get_vertices().size(); ++i)
//    {
//      Matrix_4f transform_matrix = transform.get_projection_matrix() *
//                                   Transform::get_view_matrix(Engine::get_cameras()[0]) *
//                                   transform.get_model_matrix();
//      Quaternion result_vector_4f(object.get_vertices()[i].get_position().get_x(),
//                                  object.get_vertices()[i].get_position().get_y(),
//                                  object.get_vertices()[i].get_position().get_z(),
//                                  0);
//
//      result_vector_4f = (transform_matrix * result_vector_4f);
//      Vector_3f result_vector_3f = Vector_3f(result_vector_4f.get_x(),
//                                             result_vector_4f.get_y(),
//                                             result_vector_4f.get_z());
//
//      alert(WAVE_INFO, "Current cursor position : (%lf, %lf)",
//            (position_x / Engine::get_main_window()->get_width()),
//            (position_y / Engine::get_main_window()->get_height()));
//      alert(WAVE_INFO, "Current transform matrix : %s", transform_matrix.to_string().c_str());
//      if (result_vector_3f.absolute().get_x() < (position_x / Engine::get_main_window()->get_width()) ||
//          result_vector_3f.absolute().get_y() < (position_y / Engine::get_main_window()->get_height()))
//        return false;
//    }
//    return true;
//  }
}