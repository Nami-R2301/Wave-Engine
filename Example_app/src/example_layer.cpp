//
// Created by nami on 29/03/23.
//

#include <example_layer.h>

Example_scene_3D::Example_scene_3D(const std::shared_ptr<Wave::Perspective_camera> &demo_perspective_camera_,
                                   const std::vector<std::shared_ptr<Wave::Shader>> &shaders_,
                                   const std::shared_ptr<Wave::Object_3D> &demo_object)
{
  this->layer_name = "3D Sandbox App";
  this->demo_perspective_camera = demo_perspective_camera_;
  this->shaders = shaders_;
  this->object = demo_object;
}

void Example_scene_3D::on_attach()
{
  Wave::Gl_renderer::set_clear_color(Wave::Color(0.03f, 1.0f, true));
  
  // Setup objects in scene.
  Wave::Gl_renderer::load_object(this->object.get());
  this->object->translate(10, -10, 20);
  this->object->rotate(90, -90, 0);
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
                                                                       this->demo_perspective_camera->get_view_matrix(),
                                                                       this->demo_perspective_camera->get_projection_matrix()).get_matrix()[0][0]);
  float velocity = 10.0f;
  if (Wave::Input::is_key_held(WAVE_KEY_W))
    this->demo_perspective_camera->move(this->demo_perspective_camera->get_up(), velocity * time_step);
  if (Wave::Input::is_key_held(WAVE_KEY_A))
    this->demo_perspective_camera->move(this->demo_perspective_camera->get_left(), velocity * time_step);
  if (Wave::Input::is_key_held(WAVE_KEY_S))
    this->demo_perspective_camera->move(this->demo_perspective_camera->get_up(), -velocity * time_step);
  if (Wave::Input::is_key_held(WAVE_KEY_D))
    this->demo_perspective_camera->move(this->demo_perspective_camera->get_right(), velocity * time_step);
  
  Wave::Gl_renderer::draw_loaded_objects(1);
}

void Example_scene_3D::on_event(Wave::Event &e)
{
  Layer::on_event(e);
}
