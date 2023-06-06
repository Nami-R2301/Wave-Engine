//
// Created by nami on 29/03/23.
//

#include <example_layer.h>

Example_scene_3D::Example_scene_3D(const std::shared_ptr<Wave::Camera> &demo_camera_,
                                   const std::vector<std::shared_ptr<Wave::Shader>> &shaders_,
                                   const std::vector<std::shared_ptr<Wave::Object>> &demo_object)
{
  this->layer_name = "3D Sandbox App";
  this->camera = demo_camera_;
  this->shaders = shaders_;
  this->objects = demo_object;
}

void Example_scene_3D::on_attach()
{
  this->objects[0]->add_texture(Wave::Texture_2D::create("../Wave/res/Textures/tiles.png"));
  this->objects[0]->get_textures().back()->set_texture_slot(1);
  
  // Setup objects in scene.
  this->objects[0]->translate(10, -10, 20);
  this->objects[0]->rotate(90, -90, 0);
  
  this->objects[0]->send_gpu();
  Wave::Renderer::send_object(*this->objects[0], *this->shaders[0]);
}

void Example_scene_3D::on_detach()
{
  for (const auto &shader: this->shaders) shader->unbind();
}

void Example_scene_3D::on_update(float time_step)
{
  // Update camera.
  float velocity = 10.0f;
  if (Wave::Input::is_key_held(WAVE_KEY_W))
    this->camera->move(this->camera->get_up(), velocity * time_step);
  if (Wave::Input::is_key_held(WAVE_KEY_A))
    this->camera->move(this->camera->get_left(), velocity * time_step);
  if (Wave::Input::is_key_held(WAVE_KEY_S))
    this->camera->move(this->camera->get_up(), -velocity * time_step);
  if (Wave::Input::is_key_held(WAVE_KEY_D))
    this->camera->move(this->camera->get_right(), velocity * time_step);
  
  this->camera->on_update(time_step);
}

void Example_scene_3D::on_event(Wave::Event &event)
{
  this->camera->on_event(event);
}

void Example_scene_3D::on_render()
{
}

void Example_scene_3D::on_ui_render(float time_step)
{
}
