//
// Created by nami on 8/26/22.
//

#include <example_app.h>
#include <example_layer.h>
// NEED TO BE INCLUDED IN MAIN APP ONLY
#include <entrypoint.h>

static Wave::Color previous_background_color;

Example_app::Example_app() : Wave::Engine(Wave::Renderer_api::OpenGL, Wave::Context_api_e::Glfw,
                                          Wave::Engine::App_type::Runtime)
{
  // Add Cameras
  this->active_scene = std::make_shared<Wave::Scene>();
  this->entities.emplace_back(this->active_scene->create_entity("Editor Camera"));
  
  this->entities.back().add_component<std::shared_ptr<Wave::Camera>>(
    std::make_shared<Wave::Editor_camera>(Engine::get_main_window()->get_width(),
                                          Engine::get_main_window()->get_height(),
                                          90.0f, 0.1f, 1000.0f));
  
  // Add objects
  this->entities.emplace_back(this->active_scene->create_entity("Object : Sphere"));
  auto sphere = Wave::Object::create(Wave::Resource_loader::load_object_3D_source("../Wave/res/Models/sphere.obj"),
                                     nullptr,
                                     (int32_t) (uint32_t) this->entities.back());
  this->entities.back().add_component<std::shared_ptr<Wave::Object>>(sphere);
  
  this->entities.emplace_back(this->active_scene->create_entity("Object : Awp"));
  auto awp = Wave::Object::create(Wave::Resource_loader::load_object_3D_source("../Wave/res/Models/awp.obj"),
                                  sphere->get_shader(), (int32_t) (uint32_t) this->entities.back());
  this->entities.back().add_component<std::shared_ptr<Wave::Object>>(awp);
  
  // Add text strings
  auto text_box = Wave::Text_box::create(Wave::Math::Vector_2f(0.0f, 50.0f), "~ Wave Engine ~");
  text_box->append_text(" By : Nami Reghbati ");
  
  this->entities.emplace_back(this->active_scene->create_entity("Text Box : Title"));
  this->entities.back().add_component<std::shared_ptr<Wave::Text_box>>(text_box);
  
  this->viewport = {(float) Wave::Engine::get_main_window()->get_width(),
                    (float) Wave::Engine::get_main_window()->get_height()};
}

void Example_app::on_init()
{
  Wave::Engine::on_init();
  Wave::Renderer::set_clear_color(Wave::Color(78.0f, 255.0f, false));
  
  push_layer(new Example_scene_3D(this->entities));
  push_layer(new Wave::Text_layer(this->entities, &this->viewport, false));
  
  // Lastly, finalize by sending and enqueuing the object for rendering at a later stage (on_render()).
  this->active_scene->send_gpu();
}

void Example_app::on_event(Wave::Event &event)
{
  Wave::Engine::on_event(event);
}

void Example_app::on_update(float time_step)
{
  auto camera = this->active_scene->get_entity("Editor Camera").get_component<std::shared_ptr<Wave::Camera>>();
  camera->on_update(time_step);  // Update camera.
  if (this->background_clear_color != previous_background_color)
  {
    Wave::Renderer::set_clear_color(this->background_clear_color);
    previous_background_color = this->background_clear_color;
  }
  Wave::Engine::on_update(time_step);
}

void Example_app::on_game_render()
{
  Wave::Renderer::begin(this->active_scene->get_entity("Editor Camera").get_component<std::shared_ptr<Wave::Camera>>());
  Wave::Renderer::clear_bg();
  Wave::Engine::on_game_render();
  Wave::Renderer::end();
}

bool Example_app::window_closed_callback([[maybe_unused]] Wave::On_window_close &window_closed_event)
{
  window_closed_event.print(Wave::Print_type::Warn);
  Wave::Engine::get_main_window()->close();
  return true;
}

bool Example_app::mouse_movement_callback([[maybe_unused]] Wave::On_mouse_movement &mouse_cursor_event)
{
//  float sensitivity = 1.0f;
//  auto current_mouse_position = mouse_cursor_event.get_mouse_cursor_position();
//
//  if (Wave::Input::is_mouse_button_held(WAVE_MOUSE_BUTTON_LEFT))
//  {
//    auto camera = this->active_scene->get_entity("Editor Camera").get_component<std::shared_ptr<Wave::Camera>>();
//    float center_x = camera->get_center_position().get_x();
//    float center_y = camera->get_center_position().get_y();
//
//    float delta_x = current_mouse_position.get_x() - center_x;
//    float delta_y = current_mouse_position.get_y() - center_y;

//    float rotation_x = delta_x / (float) camera->get_width();
//    float rotation_y = delta_y > center_y ? (-delta_y / (float) camera->get_height()) / 2 :
//                       (delta_y / (float) camera->get_height()) / 2;

//    this->demo_objects[0]->rotate(rotation_x * sensitivity, rotation_y * sensitivity, 0);
//  }
  return true;
}

bool Example_app::mouse_wheel_callback(Wave::On_mouse_wheel_scroll &mouse_wheel_input)
{
  auto camera = this->active_scene->get_entity("Editor Camera").get_component<std::shared_ptr<Wave::Camera>>();
  camera->on_event(mouse_wheel_input);
  return false;
}

bool Example_app::window_resize_callback(Wave::On_window_resize &window_resized_event)
{
  auto camera = this->active_scene->get_entity("Editor Camera").get_component<std::shared_ptr<Wave::Camera>>();
  Wave::Engine::window_resize_callback(window_resized_event);
  camera->on_window_resize(window_resized_event);
  return true;
}

void Example_app::on_destroy()
{
  Wave::Engine::on_destroy();
}

Wave::Engine *Wave::create_app()
{
  return new Example_app();
}

