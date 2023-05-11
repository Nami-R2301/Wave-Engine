//
// Created by nami on 29/03/23.
//

#include <example_layer.h>

Example_scene_3D::Example_scene_3D(const std::shared_ptr<Wave::Camera> &demo_camera_,
                                   const std::vector<std::shared_ptr<Wave::Shader>> &shaders_,
                                   const std::shared_ptr<Wave::Object_3D> &demo_object)
{
  this->layer_name = "3D Sandbox App";
  this->camera = demo_camera_;
  this->shaders = shaders_;
  this->object = demo_object;
}

void Example_scene_3D::on_attach()
{
  Wave::Renderer::set_clear_color(Wave::Color(0.15f, 1.0f, true));
  
  
  // Setup objects in scene.
  Wave::Gl_renderer::load_object(this->object.get());
  this->object->translate(10, -10, 20);
  this->object->rotate(90, -90, 0);
  this->object->add_texture(Wave::Texture("../Wave/res/Textures/tiles.png"));
  this->shaders[0]->bind();
  this->shaders[0]->set_uniform("u_has_texture", true);
}

void Example_scene_3D::on_detach()
{
  for (const auto &shader: this->shaders) shader->unbind();
}

void Example_scene_3D::on_update(float time_step)
{
  //  Update uniforms.
  this->shaders[0]->bind();
  this->shaders[0]->set_uniform("u_transform",
                                &Wave::Transform::get_transform_matrix(this->object->get_model_matrix(),
                                                                       this->camera->get_view_matrix(),
                                                                       this->camera->get_projection_matrix()).get_matrix()[0][0]);
  float velocity = 10.0f;
  if (Wave::Input::is_key_held(WAVE_KEY_W))
    this->camera->move(this->camera->get_up(), velocity * time_step);
  if (Wave::Input::is_key_held(WAVE_KEY_A))
    this->camera->move(this->camera->get_left(), velocity * time_step);
  if (Wave::Input::is_key_held(WAVE_KEY_S))
    this->camera->move(this->camera->get_up(), -velocity * time_step);
  if (Wave::Input::is_key_held(WAVE_KEY_D))
    this->camera->move(this->camera->get_right(), velocity * time_step);
  
  Wave::Gl_renderer::draw_loaded_objects(1);
  this->shaders[0]->unbind();
}

void Example_scene_3D::on_event(Wave::Event &event)
{
  switch (event.get_event_type())
  {
    case Wave::Event_type::None:return;
    case Wave::Event_type::On_mouse_wheel_scroll:
    {
      this->camera->on_event(event);
      break;
    }
    default:break;
  }
}

void Example_scene_3D::on_ui_render(float time_step) {
//   Wave::ImGui_layer::begin();
//
//   ImGui::Begin("Test");
//   ImGui::Text("Testing!");
//   ImGui::End();
//   Wave::ImGui_layer::end();
}
