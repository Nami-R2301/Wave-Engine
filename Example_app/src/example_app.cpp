//
// Created by nami on 8/26/22.
//

#include <example_app.h>
#include <example_layer.h>
// NEED TO BE INCLUDED IN MAIN APP ONLY
#include <entrypoint.h>

Example_app::Example_app() : Wave::Engine(Wave::Renderer_api::Opengl, Wave::Context_api::Glfw)
{
  // Add Cameras
  this->demo_perspective_camera = std::make_shared<Wave::Editor_camera>(
    Engine::get_main_window()->get_width(),
    Engine::get_main_window()->get_height(),
    90.0f, 0.1f, 1000.0f);
  
  
  // Add shaders
  this->demo_object_shaders.emplace_back(Wave::Shader::create("Default",
                                                              Wave::Res_loader_3D::load_shader_source(
                                                                "../Wave/res/Shaders/default_3D.vert").c_str(),
                                                              Wave::Res_loader_3D::load_shader_source(
                                                                "../Wave/res/Shaders/default_3D.frag").c_str()));
  this->demo_text_shaders.emplace_back(Wave::Shader::create("Text",
                                                            Wave::Res_loader_3D::load_shader_source(
                                                              "../Wave/res/Shaders/text-glyph.vert").c_str(),
                                                            Wave::Res_loader_3D::load_shader_source(
                                                              "../Wave/res/Shaders/text-glyph.frag").c_str()));
  
  
  // Add objects
  this->demo_objects.emplace_back(
    std::make_shared<Wave::Object_3D>(Wave::Res_loader_3D("../Wave/res/Models/awp.obj").load_3D_mesh()));
  this->demo_objects.emplace_back(
    std::make_shared<Wave::Object_3D>(Wave::Res_loader_3D("../Wave/res/Models/cube.obj").load_3D_mesh()));
  
  // Add text strings
  Wave::Text_format format = {25.0f,
                              Engine::get_main_window()->get_height() - 38.0f,
                              1.0f,
                              26.0f,
                              Wave::Text_style::REGULAR,
                              Wave::Color(1.0f, 0.0f, 0.0f, 1.0f, true)};
  this->demo_text.emplace_back(std::make_shared<Wave::Gl_text>("../Wave/res/Fonts/Comfortaa/Comfortaa-Bold.ttf",
                                                               "Wave Engine ~",
                                                               format));
  
  push_layer(new Example_scene_3D(this->demo_perspective_camera, this->demo_object_shaders, this->demo_objects[0]));
  push_layer(new Wave::Text_layer(this->demo_text, this->demo_text_shaders,
                                  Wave::Vector_2f(Engine::get_main_window()->get_width(),
                                                  Engine::get_main_window()->get_height())));
}

void Example_app::init()
{
  Wave::Renderer::set_clear_color(Wave::Color(78.0f, 255.0f, false));
}

void Example_app::on_update(float time_step)
{
  Wave::Gl_renderer::clear_bg();
  Engine::on_update(time_step);
}

void Example_app::on_event(Wave::Event &event)
{
  Engine::on_event(event);
  
  Wave::Event_dispatcher event_dispatcher(event);
  
  switch(event.get_event_type())
  {
    case Wave::Event_type::On_key_event:
    {
      event_dispatcher.dispatch<Wave::On_any_key_event>(BIND_EVENT_FUNCTION(any_key_callback));
      break;
    }
    case Wave::Event_type::On_mouse_movement:
    {
      event_dispatcher.dispatch<Wave::On_mouse_movement>(BIND_EVENT_FUNCTION(mouse_movement_callback));
      break;
    }
    case Wave::Event_type::On_mouse_button_press:
    {
      event_dispatcher.dispatch<Wave::On_mouse_button_press>(BIND_EVENT_FUNCTION(mouse_button_press_callback));
      break;
    }
    case Wave::Event_type::On_mouse_button_hold:
    {
      event_dispatcher.dispatch<Wave::On_mouse_button_hold>(BIND_EVENT_FUNCTION(mouse_button_hold_callback));
      break;
    }
    case Wave::Event_type::On_mouse_button_release:
    {
      event_dispatcher.dispatch<Wave::On_mouse_button_release>(BIND_EVENT_FUNCTION(mouse_button_release_callback));
      break;
    }
    case Wave::Event_type::On_mouse_wheel_scroll:
    {
      event_dispatcher.dispatch<Wave::On_mouse_wheel_scroll>(BIND_EVENT_FUNCTION(mouse_wheel_callback));
      break;
    }
    default:break;
  }
}

bool Example_app::window_closed_callback([[maybe_unused]] Wave::On_window_close &window_closed_event)
{
  window_closed_event.print(Wave::Print_type::Warn);
  Engine::get_main_window()->close();
  return true;
}

bool Example_app::mouse_movement_callback(Wave::On_mouse_movement &mouse_cursor_event)
{
  float sensitivity = 1.0f;
  auto current_mouse_position = mouse_cursor_event.get_mouse_cursor_position();
  
  if (Wave::Input::is_mouse_button_held(WAVE_MOUSE_BUTTON_LEFT))
  {
    float center_x = this->demo_perspective_camera->get_center_position().get_x();
    float center_y = this->demo_perspective_camera->get_center_position().get_y();
    
    float delta_x = current_mouse_position.get_x() - center_x;
    float delta_y = current_mouse_position.get_y() - center_y;
    
    float rotation_x = delta_x / (float) this->demo_perspective_camera->get_width();
    float rotation_y = delta_y > center_y ? (-delta_y / (float) this->demo_perspective_camera->get_height()) / 2 :
                       (delta_y / (float) this->demo_perspective_camera->get_height()) / 2;
    
    this->demo_objects[0]->rotate(rotation_x * sensitivity, rotation_y * sensitivity, 0);
  }
  return true;
}

bool Example_app::mouse_wheel_callback(Wave::On_mouse_wheel_scroll &mouse_wheel_input)
{
  this->demo_perspective_camera->on_event(mouse_wheel_input);
  return false;
}

bool Example_app::window_resize_callback(Wave::On_window_resize &window_resized_event)
{
  Engine::window_resize_callback(window_resized_event);
  this->demo_perspective_camera->on_window_resize(window_resized_event);
  return true;
}

Wave::Engine *Wave::create_app()
{
  return new Example_app();
}

