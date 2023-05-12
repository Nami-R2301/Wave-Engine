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
  this->shaders[0]->bind();
  this->object->add_texture(std::make_shared<Wave::Texture>("../Wave/res/Textures/tiles.png"));
  
  unsigned int u_camera_block = glGetUniformBlockIndex(this->shaders[0]->get_id(), "u_camera");
  glUniformBlockBinding(this->shaders[0]->get_id(), u_camera_block, 3);
  Wave::Renderer::load_object(this->object.get());
  
  // Setup objects in scene.
  this->object->translate(10, -10, 20);
  this->object->rotate(90, -90, 0);
  
  this->shaders[0]->set_uniform("u_model", &this->object->get_model_matrix().get_matrix()[0][0]);
  this->shaders[0]->set_uniform("u_has_texture", true);
  
  this->shaders[0]->unbind();
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
  
  this->camera->update_view_matrix();
  
  //  Update objects.
  this->shaders[0]->bind();
  Wave::Renderer::begin_scene(*this->camera);
  Wave::Renderer::draw_loaded_objects(1);
  this->shaders[0]->unbind();
}

void Example_scene_3D::on_event(Wave::Event &event)
{
  this->camera->on_event(event);
}

void Example_scene_3D::on_ui_render([[maybe_unused]] float time_step)
{
}
