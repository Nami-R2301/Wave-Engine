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
    this->demo_perspective_camera = create_shared_pointer<Editor_camera>(90.0f, 0.1f, 1000.0f);
    
    // Add shaders
    this->demo_shaders.emplace_back(Shader::create("Default",
                                                   Res_loader_3D::load_shader_source(
                                                       "default.vert").c_str(),
                                                   Res_loader_3D::load_shader_source(
                                                       "default.frag").c_str()));
    
    
    // Add objects
    this->demo_objects.emplace_back(create_shared_pointer<Object_3D>(Res_loader_3D("awp.obj").load_3D_mesh()));
    this->demo_objects.emplace_back(create_shared_pointer<Object_3D>(Res_loader_3D("cube.obj").load_3D_mesh()));
    
    // Set texture.
    this->demo_objects[0]->add_texture(Texture("../Wave/res/Textures/tiles.png"));
    this->demo_shaders[0]->bind();
    this->demo_shaders[0]->set_uniform("u_has_texture", true);
    
    push_layer(new Editor_layer(this->demo_perspective_camera,
                                this->demo_shaders,
                                this->demo_objects[0]));
    push_overlay(new ImGui_layer());
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
    window_closed_event.print(Print_type::Warn);
    Engine::main_window->close();
    return true;
  }
  
  bool Editor::window_resize_callback(On_window_resize &window_resized_event)
  {
    Engine::window_resize_callback(window_resized_event);
    this->demo_perspective_camera->on_window_resize(window_resized_event);
    return true;
  }
  
  Engine *create_app()
  {
    return new Editor();
  }
}