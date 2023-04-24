//
// Created by nami on 23/04/23.
//

#include <editor.h>
#include <editor_layer.h>

#include <entrypoint.h>

namespace Wave
{
  
  Editor::Editor() : Engine(Renderer_api::Opengl)
  {
    // Add Cameras
    this->demo_perspective_camera = Wave::create_shared_pointer<Wave::Editor_camera>(90.0f, 0.1f, 1000.0f);
    
    // Add shaders
    this->demo_shaders.emplace_back(Wave::Shader::create("Default",
                                                         Wave::Res_loader_3D::load_shader_source(
                                                             "default.vert").c_str(),
                                                         Wave::Res_loader_3D::load_shader_source(
                                                             "default.frag").c_str()));
    
    
    // Add objects
    this->demo_objects.emplace_back(Wave::create_shared_pointer<Wave::Object_3D>(Wave::Res_loader_3D("awp.obj").load_3D_mesh()));
    this->demo_objects.emplace_back(Wave::create_shared_pointer<Wave::Object_3D>(Wave::Res_loader_3D("cube.obj").load_3D_mesh()));
    
    push_layer(new Editor_layer(this->demo_perspective_camera,
                                this->demo_shaders,
                                this->demo_objects[0]));
    push_overlay(new Wave::ImGui_layer());
  }
  
  void Editor::init()
  {
  }
  
  void Editor::on_update(float time_step)
  {
    if (!Engine::get_main_window()->is_minimized()) ImGui_layer::begin();
    Engine::on_update(time_step);
    if (!Engine::get_main_window()->is_minimized()) ImGui_layer::end();
  }
  
  void Editor::on_event(Event &event)
  {
    Engine::on_event(event);
  }
  
  bool Editor::window_closed_callback(On_window_close &window_closed_event)
  {
    window_closed_event.print(Wave::Print_type::Warn);
    Engine::main_window->close();
    return true;
  }
  
  bool Editor::window_resize_callback(On_window_resize &window_resized_event)
  {
    Engine::window_resize_callback(window_resized_event);
    this->demo_perspective_camera->on_window_resize(window_resized_event);
    return true;
  }
}

Wave::Engine *Wave::create_app()
{
  return new Editor();
}