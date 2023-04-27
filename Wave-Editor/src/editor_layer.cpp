//
// Created by nami on 23/04/23.
//

#include <editor_layer.h>
#include <imGUI/imgui.h>

namespace Wave
{
  
  Editor_layer::Editor_layer(const std::shared_ptr<Wave::Camera> &demo_perspective_camera_,
                             const std::vector<std::shared_ptr<Wave::Shader>> &shaders_,
                             const std::vector<std::shared_ptr<Wave::Object_3D>> &demo_objects_,
                             const std::shared_ptr<Framebuffer> &viewport_)
  {
    this->layer_name = "Editor layer";
    this->camera = demo_perspective_camera_;
    this->shaders = shaders_;
    this->objects = demo_objects_;
    this->viewport = viewport_;
  }
  
  void Editor_layer::on_attach()
  {
    Wave::Gl_renderer::set_clear_color(Wave::Color(0.03f, 1.0f, true));
    
    // Init object shaders.
    this->objects[0]->add_texture(Texture("../Wave/res/Textures/tiles.png"));
    this->shaders[1]->bind();
    this->shaders[1]->set_uniform("u_has_texture", true);
    this->shaders[1]->set_uniform("u_sampler", 0);
    this->shaders[1]->unbind();
    // Setup objects in scene.
    Wave::Gl_renderer::load_object(this->objects[0].get());
    this->objects[0]->translate(10, -10, 20);
    this->objects[0]->rotate(90, -90, 0);
  }
  
  void Editor_layer::on_detach()
  {
    for (const auto &shader: this->shaders) shader->unbind();
  }
  
  void Editor_layer::on_update(float time_step)
  {
    //  Update objects.
    this->shaders[1]->bind();
    this->shaders[1]->set_uniform("u_transform",
                                  &Wave::Transform::get_transform_matrix(this->objects[0]->get_model_matrix(),
                                                                         this->camera->get_view_matrix(),
                                                                         this->camera->get_projection_matrix()).get_matrix()[0][0]);
    
    // Synchronous tasks.
    float velocity = 10.0f;
    if (Wave::Input::is_key_held(WAVE_KEY_W))
    {
      this->camera->move(this->camera->get_up(), velocity * time_step);
    }
    if (Wave::Input::is_key_held(WAVE_KEY_A))
    {
      this->camera->move(this->camera->get_left(), velocity * time_step);
    }
    if (Wave::Input::is_key_held(WAVE_KEY_S))
    {
      this->camera->move(this->camera->get_up(), -velocity * time_step);
    }
    if (Wave::Input::is_key_held(WAVE_KEY_D))
    {
      this->camera->move(this->camera->get_right(), velocity * time_step);
    }
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_ENTER))
    {
      Wave::Display_settings::toggle_fullscreen(Wave::Engine::get_main_window());
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_V))
    {
      Wave::Display_settings::set_vsync(Wave::Engine::get_main_window(),
                                        !Wave::Engine::get_main_window()->is_vsync());
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_6))
    {
      Wave::Display_settings::set_refresh_rate(Wave::Engine::get_main_window(), 60);
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_3))
    {
      Wave::Display_settings::set_refresh_rate(Wave::Engine::get_main_window(), 30);
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_1))
    {
      Wave::Display_settings::set_refresh_rate(Wave::Engine::get_main_window(), 1);
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_9))
    {
      Wave::Display_settings::set_refresh_rate(Wave::Engine::get_main_window(), 144);
    }
    
    if (Wave::Input::is_key_pair_pressed(WAVE_KEY_LEFT_ALT, WAVE_KEY_F4))
    {
      Wave::alert(WAVE_WARN, "[SETTING] --> Force shutdown");
      Wave::Engine::get_main_window()->close();
    }
    
    Wave::Gl_renderer::draw_loaded_objects(1);
    this->shaders[1]->unbind();
  }
  
  void Editor_layer::on_event(Event &event)
  {
  
  }
  
  void Editor_layer::on_ui_render(float time_step)
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    
    auto viewports = ImGui::GetContentRegionAvail();
    uint64_t textureID = this->viewport->get_color_attachment();
    ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2 {viewports.x,
                                                              viewports.y},
                 ImVec2 {0,
                         1}, ImVec2 {1,
                                     0});
    ImGui::End();  // Viewport
    ImGui::PopStyleVar();
  }
}