//
// Created by nami on 29/03/23.
//

#include <example_layer.h>

static float s_imgui_app_performance_stat = 0.0f;
static float s_imgui_app_performance_timer = 0.0f;

Example_scene_3D::Example_scene_3D(std::vector<Wave::Entity> &entities_,
                                   bool imgui_render) : imgui_render(imgui_render)
{
  this->layer_name = "3D Sandbox App";
  
  
  for (auto &entity: entities_)
  {
    if (entity.has_component<std::shared_ptr<Wave::Object>>())
      this->objects.emplace_back(entity.get_component<std::shared_ptr<Wave::Object>>());
    
    if (entity.has_component<std::shared_ptr<Wave::Camera>>())
      this->camera = entity.get_component<std::shared_ptr<Wave::Camera>>();
  }
}

void Example_scene_3D::on_attach()
{
//  this->objects[0]->add_texture(Wave::Texture_2D::create("../Wave/res/Textures/tiles.png"));
//  this->objects[0]->get_textures().back()->set_texture_slot(1);
  
  // Setup objects transformations.
  this->objects[0]->translate(0, 0, 5);
  this->objects[0]->rotate(0, 0, 0);
  
  this->objects[1]->translate(10, -10, 20);
  this->objects[1]->rotate(90, -90, 0);
  
  this->objects[2]->translate(-3.5, -2, 6);
  this->objects[2]->rotate(45, 0, 0);
  
  Wave::Point_light point_light = Wave::Point_light(Wave::Color(0xFFFFFFFF), 0.1f, 0.4f,
                                                    Wave::Vector_3f(0.0f, 0.0f, 0.0f),
                                                    0.3f, 0.2f, 0.1f);
  this->objects[0]->calculate_average_normals();
  this->objects[1]->calculate_average_normals();
  this->objects[2]->calculate_average_normals();
  
  this->objects[0]->calculate_effect_by_light(point_light);
  
  // Lastly, finalize by sending and enqueuing the object for rendering at a later stage (on_render()).
  this->objects[0]->send_gpu(1);
  this->objects[1]->send_gpu(1);
  this->objects[2]->send_gpu(1);
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
  if (this->imgui_render)
  {
    ImGuiIO io = ImGui::GetIO();
    io.DeltaTime = time_step;
    ImGuiViewport *viewport_ = ImGui::GetMainViewport();
    
    /* Important to set next viewport window, because otherwise a window node will clip out of the main viewport when
     * dragged outside the main window, which is annoying when trying to re-dock a node by dragging it to the very edge,
     * and it's separating from the dockSpace instead.
    */
    
    // Render System panel UI.
    ImGui::SetNextWindowViewport(viewport_->ID);
    
    if (Wave::ImGui_layer::show_system_panel)
    {
      if (ImGui::Begin("System Info", &(Wave::ImGui_layer::show_system_panel), ImGuiWindowFlags_None))
      {
        // Slow down the refresh rate of the text output by timing it with each second.
        if (s_imgui_app_performance_timer >= 1.0f)
        {
          ImGui::Text("Application performance :\t%.3f ms/frame (%d FPS)",
                      Wave::Engine::get_time_step() * 1000.0f,
                      (int) (1.0f / s_imgui_app_performance_stat));
          s_imgui_app_performance_timer = 0;
        } else
        {
          ImGui::Text("Application performance :\t%.3f ms/frame (%d FPS)",
                      s_imgui_app_performance_stat * 1000.0f,
                      (int) (1.0f / s_imgui_app_performance_stat));
        }
        
        ImGui::Text("Framebuffer resolution :\t(%.2f, %.2f)", 1920.0f, 1080.0f);
        ImGui::Text("UI font size :\t%.2f",
                    Wave::ImGui_layer::imgui_data.font_size * Wave::ImGui_layer::imgui_data.font_scale);
      }
      ImGui::End();  // System Info.
      auto node = ImGui::DockBuilderGetNode(Wave::ImGui_layer::events_panel_dock_id);
      if (node)
      {
        ImGui::DockNodeBeginAmendTabBar(node);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
        
        if (ImGui::BeginTabItem("Renderer Info", &Wave::ImGui_layer::show_system_panel, ImGuiTabItemFlags_Trailing))
        {
          auto renderer_stats = Wave::Renderer::get_stats();
          ImGui::Text("Renderer 3D");
          ImGui::Text("\t\tRenderer stats (per frame) :\n"
                      "\t\t\t\tDraw calls = %ld\n"
                      "\t\t\t\tTotal number of shaders sent = %ld"
                      "\t\t\t\tTotal number of vertex array buffers sent = %ld"
                      "\t\t\t\tTotal number of vertices sent = %ld"
                      "\t\t\t\tTotal number of indices sent = %ld\n"
                      "\t\t\t\tTotal number of textures sent = %ld"
                      "\t\t\t\tTotal number of objects sent = %ld"
                      "\t\t\t\t\t\t\t\t\t  Total number of text characters sent = %ld",
                      renderer_stats.draw_call_count,
                      renderer_stats.shaders_count,
                      renderer_stats.vao_count,
                      renderer_stats.vertices_drawn_count,
                      renderer_stats.indices_drawn_count,
                      renderer_stats.textures_drawn_count,
                      renderer_stats.object_count,
                      renderer_stats.text_glyph_count);
          ImGui::EndTabItem();
        }
        ImGui::PopStyleVar();
        ImGui::DockNodeEndAmendTabBar();
      }
    }
  }
}
