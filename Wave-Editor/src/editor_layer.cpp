//
// Created by nami on 23/04/23.
//

#include <editor_layer.h>
#include <imGUI/imgui.h>
#include <imGUI/imgui_internal.h>

namespace Wave
{
  
  static Vector_3f camera_slider_position = Vector_3f(0);
  
  Editor_layer::Editor_layer(const std::shared_ptr<Wave::Camera> &demo_perspective_camera_,
                             const std::vector<std::shared_ptr<Wave::Shader>> &shaders_,
                             const std::vector<std::shared_ptr<Wave::Object_3D>> &demo_objects_,
                             const std::shared_ptr<Framebuffer> &viewport_)
  {
    this->layer_name = "Editor layer";
    this->camera = demo_perspective_camera_;
    this->shaders = shaders_;
    this->objects = demo_objects_;
    this->framebuffer_viewport_data.viewport = viewport_;
  }
  
  void Editor_layer::on_attach()
  {
    Wave::Gl_renderer::set_clear_color(Wave::Color(0.05f, 1.0f, true));
    
    // Setup object shaders.
    this->objects[0]->add_texture(Texture("../Wave/res/Textures/tiles.png"));
    this->shaders[1]->bind();
    this->shaders[1]->set_uniform("u_has_texture", true);
    this->shaders[1]->set_uniform("u_sampler", 0);
    this->shaders[1]->unbind();
    
    // Setup framebuffer shader.
    this->framebuffer_viewport_data.framebuffer_viewport_shader = Shader::create("Framebuffer Editor Viewport",
                                                                                 Res_loader_3D::load_shader_source(
                                                                                     "../Wave-Editor/res/Shaders/viewport_framebuffer.vert").c_str(),
                                                                                 Res_loader_3D::load_shader_source(
                                                                                     "../Wave-Editor/res/Shaders/viewport_framebuffer.frag").c_str());
    
    // Setup objects in scene.
    Wave::Gl_renderer::load_object(this->objects[0].get());
    this->objects[0]->translate(10, -10, 20);
//    this->objects[1]->translate(0, 0, -10);
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
    ImGuiIO &io = ImGui::GetIO();
    auto bold = io.Fonts->Fonts[1];
    io.DeltaTime = time_step;
    // Note: Switch this to true to enable dockspace
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    
    if (opt_fullscreen)
    {
      ImGuiViewport *viewport_ = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport_->Pos);
      ImGui::SetNextWindowSize(viewport_->Size);
      ImGui::SetNextWindowViewport(viewport_->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      Editor_layer::window_flags |=
          ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
          ImGuiWindowFlags_NoMove;
      Editor_layer::window_flags |=
          ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDecoration;
    }
    
    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    {
      Editor_layer::window_flags |= ImGuiWindowFlags_NoBackground;
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Wave-Engine", &(Editor_layer::dockSpace_open), Editor_layer::window_flags);
    ImGui::PopStyleVar();  // Window Padding.
    
    // DockSpace
    
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable && !Editor_layer::dockSpace_id)
    {
      Editor_layer::dockSpace_id = ImGui::GetID("Wave-Engine-DockSpace");
      ImGui::DockBuilderRemoveNode(Editor_layer::dockSpace_id); // Clear out existing layout
      ImGui::DockBuilderAddNode(Editor_layer::dockSpace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
      ImGui::DockBuilderSetNodeSize(Editor_layer::dockSpace_id, ImVec2(Engine::get_main_window()->get_width(),
                                                                       Engine::get_main_window()->get_height()));
      
      ImGuiID dock_main_id = Editor_layer::dockSpace_id;
      Editor_layer::scene_panel_dock_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.15f, nullptr,
                                                                      &dock_main_id);
      Editor_layer::stats_panel_dock_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.1f, nullptr,
                                                                      &dock_main_id);
      
      ImGui::DockBuilderDockWindow("Viewport", dock_main_id);
      ImGui::DockBuilderDockWindow("Scene", Editor_layer::scene_panel_dock_id);
      ImGui::DockBuilderDockWindow("Events", Editor_layer::stats_panel_dock_id);
      ImGui::DockBuilderFinish(dock_main_id);
      this->viewport_panel_dock_id = dock_main_id;
    }
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowMinSize.x = 50.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      Editor_layer::dockSpace_id = ImGui::GetID("Wave-Engine-DockSpace");
      ImGui::DockSpace(Editor_layer::dockSpace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
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
          Engine::shutdown();
        }
        ImGui::EndMenu();
      }
    }
    ImGui::EndMenuBar();
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 10.0f));
    if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_None))
    {
      if (ImGui::TreeNodeEx("Clear color", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
      {
        ImGui::PushFont(bold);
        ImGui::ColorEdit3("", &Engine::get_main_window()->get_bg_color()[0]);
        ImGui::PopFont();
        ImGui::TreePop();
      }
      ImGui::Separator();
      if (ImGui::TreeNodeEx("Main Camera (Perspective)",
                            ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
      {
        float min_x = -10.0f, max_x = 10.0f, min_y = -10.0f, max_y = 10.0f, min_depth = -100.0f, max_depth = 100.0f;
        ImGui::PushFont(bold);
        ImGui::SliderScalar("X", ImGuiDataType_Float, &camera_slider_position[0], &min_x, &max_x);
        this->camera->set_position(camera_slider_position[0],
                                   camera_slider_position[1],
                                   camera_slider_position[2]);
        ImGui::SliderScalar("Y", ImGuiDataType_Float, &camera_slider_position[1], &min_y, &max_y);
        this->camera->set_position(camera_slider_position[0],
                                   camera_slider_position[1],
                                   camera_slider_position[2]);
        ImGui::SliderScalar("Z", ImGuiDataType_Float, &camera_slider_position[2], &min_depth, &max_depth);
        this->camera->set_position(camera_slider_position[0],
                                   camera_slider_position[1],
                                   camera_slider_position[2]);
        
        ImGui::PopFont();
        ImGui::TreePop();
      }
    }
    ImGui::End();  // Scene hierarchy
    ImGui::PopStyleVar();  // Window padding
    
    if (ImGui::Begin("Events"))
    {
      ImGui::Text("Application performance :\t%.3f ms/frame (%d FPS)", 1000.0f * time_step,
                  static_cast<int>(Engine::get_engine_framerate()));
      auto framebuffer_viewport_gl = dynamic_cast<Gl_framebuffer *>(this->framebuffer_viewport_data.viewport.get());
      ImGui::Text("Framebuffer size :\t(%.2f, %.2f)", framebuffer_viewport_gl->get_options().width,
                  framebuffer_viewport_gl->get_options().height);
    }
    ImGui::End();  // Stats
    
    if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse))
    {
      ImGuiDockNode *viewport_dock_node;
      viewport_dock_node = ImGui::DockBuilderGetNode(this->viewport_panel_dock_id);
      viewport_dock_node->HostWindow->DrawList->AddCallback(draw_viewport_quad,
                                                            &this->framebuffer_viewport_data);
      //  If viewport dockNode has no user callback (Window undocked).
      if (!viewport_dock_node->HostWindow->DrawList->CmdBuffer.Data->UserCallback)
      {
        ImGui::GetCurrentWindow()->DrawList->AddCallback(draw_viewport_quad, &this->framebuffer_viewport_data);
      }
    }
    ImGui::End();  // Viewport
    ImGui::End(); // Wave-Engine.
    ImGui::PopStyleVar();  // Window Border size.
    ImGui::PopStyleVar();  // Window Rounding.
  }
  
  void Editor_layer::draw_viewport_quad([[maybe_unused]] const ImDrawList *parentList, const ImDrawCmd *cmd)
  {
    auto framebuffer_data = (Framebuffer_draw_data *) cmd->UserCallbackData;
    auto framebuffer_viewport_gl = dynamic_cast<Gl_framebuffer *>(framebuffer_data->viewport.get());
    
    ImGuiWindow *viewport = ImGui::FindWindowByName("Viewport");
    
    // Remember important last states
    GLint lastProgram = 0, last_vao = 0, last_vbo = 0, last_ibo = 0;
    gl_call(glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram));
    gl_call(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vao));
    gl_call(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_vbo));
    gl_call(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_ibo));
    
    gl_call(glEnable(GL_SAMPLE_SHADING));
    
    framebuffer_data->framebuffer_viewport_shader->bind();
    
    ImDrawData *drawData = ImGui::GetDrawData();
    float left = drawData->DisplayPos.x;
    float right = drawData->DisplayPos.x + drawData->DisplaySize.x;
    float top = drawData->DisplayPos.y;
    float bottom = drawData->DisplayPos.y + drawData->DisplaySize.y;
    
    const float viewportProjection[4][4] = {
        {2.0f / (right - left),           0.0f,                            0.0f,  0.0f},
        {0.0f,                            2.0f / (top - bottom),           0.0f,  0.0f},
        {0.0f,                            0.0f,                            -1.0f, 0.0f},
        {(right + left) / (left - right), (top + bottom) / (bottom - top), 0.0f,  1.0f},
    };
    
    glUniformMatrix4fv(glGetUniformLocation(framebuffer_data->framebuffer_viewport_shader->get_id(), "u_projection"),
                       1, GL_FALSE, &viewportProjection[0][0]);
//    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_projection", &viewportProjection[0][0]);
    
    // Bind framebuffer textures
    gl_call(glActiveTexture(GL_TEXTURE0));
    gl_call(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebuffer_viewport_gl->get_color_attachment()));
    gl_call(glActiveTexture(GL_TEXTURE1));
    gl_call(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebuffer_viewport_gl->get_depth_attachment()));
    
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_color_attachment_sampler", 0);
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_viewport_width", (int) viewport->Size.x);
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_viewport_height", (int) viewport->Size.y);
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_max_samples", 8);
    
    // Draw viewport quad
    framebuffer_viewport_gl->data.vao->bind();
    framebuffer_viewport_gl->data.vao->get_vertex_buffers().back()->bind();
    framebuffer_viewport_gl->data.vao->get_index_buffer()->bind();
    
    gl_call(glDrawElements(GL_TRIANGLES, framebuffer_viewport_gl->data.vao->get_index_buffer()->get_count(),
                           GL_UNSIGNED_INT, nullptr));
    
    framebuffer_viewport_gl->data.vao->unbind();
    
    // Unbind all buffers.
    gl_call(glActiveTexture(GL_TEXTURE0));
    gl_call(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    gl_call(glActiveTexture(GL_TEXTURE1));
    gl_call(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    framebuffer_data->framebuffer_viewport_shader->unbind();
    
    // Reset Imgui OpenGL buffers for next draw commands.
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_ibo));
    gl_call(glBindBuffer(GL_ARRAY_BUFFER, last_vbo));
    gl_call(glBindVertexArray(last_vao));
    gl_call(glUseProgram(lastProgram));
  }
}