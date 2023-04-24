//
// Created by nami on 23/04/23.
//

#include "Events/key_event.h"
#include <editor_layer.h>
#include <imGUI/imgui.h>

namespace Wave
{
  
  Editor_layer::Editor_layer(const std::shared_ptr<Wave::Camera> &demo_perspective_camera_,
                             const std::vector<std::shared_ptr<Wave::Shader>> &shaders_,
                             const std::shared_ptr<Wave::Object_3D> &demo_object_)
  {
    this->layer_name = "Editor layer";
    this->camera = demo_perspective_camera_;
    this->shaders = shaders_;
    this->object = demo_object_;
  }
  
  void Editor_layer::on_attach()
  {
    
    Wave::Gl_renderer::set_clear_color(Wave::Color(0.03f, 1.0f, true));
    
    // Setup objects in scene.
    Wave::Gl_renderer::load_object(this->object.get());
    this->object->translate(10, -10, 20);
    this->object->rotate(90, -90, 0);
    
    Framebuffer_options fbSpec;
    fbSpec.width = 1920;
    fbSpec.height = 1080;
    this->viewport_coords = {1920.0f,
                             1080.0f};
    this->framebuffer = Framebuffer::create(fbSpec);
  }
  
  void Editor_layer::on_detach()
  {
    for (const auto &shader: this->shaders) shader->unbind();
  }
  
  void Editor_layer::on_update(float time_step)
  {
    Framebuffer_options spec = this->framebuffer->get_options();
    // Redraw framebuffer on resize.
    if (this->viewport_coords.get_x() > 0.0f && this->viewport_coords.get_y() > 0.0f &&
        (spec.width != this->viewport_coords.get_x() || spec.height != this->viewport_coords.get_y()))
    {
      this->framebuffer->resize(this->viewport_coords.get_x(), this->viewport_coords.get_y());
    }
    
    this->framebuffer->bind();
    Gl_renderer::clear_bg();
    //  Update uniforms.
    this->shaders[0]->bind();
    this->shaders[0]->set_uniform("u_transform",
                                  &Wave::Transform::get_transform_matrix(this->object->get_model_matrix(),
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
    this->framebuffer->unbind();
  }
  
  void Editor_layer::on_event(Event &event)
  {
  
  }
  
  void Editor_layer::on_imgui_render(float time_step)
  {
    ImGuiIO &io = ImGui::GetIO();
    auto bold = io.Fonts->Fonts[1];
    io.DeltaTime = time_step;
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    
    if (opt_fullscreen)
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    
    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Wave-Engine", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();  // Window Padding.
    ImGui::PopStyleVar();  // Window Border size.
    ImGui::PopStyleVar();  // Window Rounding.
    
    // DockSpace
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowMinSize.x = 250.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        ImGui::MenuItem("Open Project...", "Ctrl+O");
        ImGui::Separator();
        ImGui::MenuItem("New Scene", "Ctrl+N");
        ImGui::MenuItem("Save Scene", "Ctrl+S");
        ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S");
        ImGui::Separator();
        
        if (ImGui::MenuItem("Exit"))
        {
          Engine::get_app()->shutdown();
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
    
    if (ImGui::Begin("Scene"))
    {
      if (ImGui::TreeNodeEx("Clear color", ImGuiTreeNodeFlags_SpanFullWidth))
      {
        ImGui::PushFont(bold);
        ImGui::ColorEdit3("", &Engine::get_main_window()->get_bg_color()[0]);
        ImGui::PopFont();
        ImGui::TreePop();
      }
    }
    ImGui::End();  // Scene Hierarchy
    
    if (ImGui::Begin("Stats"))
    {
      ImGui::Text("Application performance :\t%.3f ms/frame (%d FPS)", 1000.0f * time_step,
                  static_cast<int>(Engine::get_engine_framerate()));
    }
    ImGui::End();  // Stats
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0,
                                                             0});
    ImGui::Begin("Viewport");
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    
    uint64_t textureID = this->framebuffer->get_color_attachment();
    ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2 {viewport_panel_size.x,
                                                              viewport_panel_size.y},
                 ImVec2 {0,
                         1}, ImVec2 {1,
                                     0});
    ImGui::End();  // Viewport
    ImGui::PopStyleVar();
    ImGui::End(); // Wave-Engine.
  }
}