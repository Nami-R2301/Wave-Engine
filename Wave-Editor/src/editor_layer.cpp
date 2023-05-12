//
// Created by nami on 23/04/23.
//

#include <editor_layer.h>

namespace Wave
{
  
  Color Editor_layer::framebuffer_color = Color(78.0f, 255.0f, false);  // Defaults to grey.
  
  Editor_layer::Editor_layer(const std::shared_ptr<Wave::Editor_camera> &demo_perspective_camera_,
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
    // Setup object shaders.
    this->objects[0]->add_texture(std::make_shared<Texture>("../Wave/res/Textures/tiles.png"));
    this->shaders[1]->bind();
    
    unsigned int u_camera_block = glGetUniformBlockIndex(this->shaders[1]->get_id(), "u_camera");
    CHECK_GL_CALL(glUniformBlockBinding(this->shaders[1]->get_id(), u_camera_block, 3));
    
    this->shaders[1]->set_uniform("u_has_texture", true);
    this->shaders[1]->set_uniform("u_sampler", 0);
    
    // Setup objects in scene.
    Wave::Renderer::load_object(this->objects[0].get());
    this->objects[0]->translate(10, -10, 20);
    this->objects[0]->rotate(90, -90, 0);
    
    this->shaders[1]->set_uniform("u_model", &this->objects[0]->get_model_matrix().get_matrix()[0][0]);
    this->shaders[1]->unbind();
    
    // Setup framebuffer shader.
    this->framebuffer_viewport_data.framebuffer_viewport_shader = Shader::create("Framebuffer Editor Viewport",
                                                                                 Res_loader_3D::load_shader_source(
                                                                                   "../Wave-Editor/res/Shaders/viewport_framebuffer_ms.vert").c_str(),
                                                                                 Res_loader_3D::load_shader_source(
                                                                                   "../Wave-Editor/res/Shaders/viewport_framebuffer_ms.frag").c_str());
  }
  
  void Editor_layer::on_detach()
  {
    for (const auto &shader: this->shaders) shader->unbind();  // Unbind before destruction.
  }
  
  void Editor_layer::on_update(float time_step)
  {
    this->camera->on_update(time_step);  // Update camera.
    
    // Update window.
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
    
    //  Update objects.
    this->shaders[1]->bind();
    Renderer::begin_scene(*this->camera);
    Renderer::draw_loaded_objects(1);
    this->shaders[1]->unbind();
  }
  
  void Editor_layer::on_event([[maybe_unused]] Event &event)
  {
    this->camera->on_event(event);
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
      Editor_layer::scene_panel_dock_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr,
                                                                      &dock_main_id);
      Editor_layer::stats_panel_dock_id = ImGui::DockBuilderSplitNode(Editor_layer::scene_panel_dock_id, ImGuiDir_Down,
                                                                      0.4f, nullptr,
                                                                      &(Editor_layer::scene_panel_dock_id));
      Editor_layer::events_panel_dock_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.1f, nullptr,
                                                                       &dock_main_id);
      
      ImGui::DockBuilderDockWindow("Viewport", dock_main_id);
      ImGui::DockBuilderDockWindow("Scene", Editor_layer::scene_panel_dock_id);
      ImGui::DockBuilderDockWindow("System Info", Editor_layer::events_panel_dock_id);
      ImGui::DockBuilderDockWindow("Entity Info", Editor_layer::stats_panel_dock_id);
      ImGui::DockBuilderFinish(dock_main_id);
      this->viewport_panel_dock_id = dock_main_id;
    }
    
    dockspace_flags |= ImDrawListFlags_AntiAliasedLinesUseTex;
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
    if (ImGui::Begin("Scene", &(Editor_layer::dockSpace_open), ImGuiWindowFlags_None))
    {
      ImGui::PushFont(bold);
      if (ImGui::TreeNodeEx("Background", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
      {
        ImGui::ColorEdit3("Color", &Editor_layer::framebuffer_color[0]);
        ImGui::TreePop();
      }
      ImGui::Separator();
      if (ImGui::TreeNodeEx("Editor camera",
                            ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
      {
        float min_x = -100.0f, max_x = 100.0f, min_y = -100.0f, max_y = 100.0f, min_depth = -100.0f, max_depth = 100.0f;
        ImGui::SliderScalar("X", ImGuiDataType_Float, &this->camera->get_position()[0], &min_x, &max_x);
        ImGui::SliderScalar("Y", ImGuiDataType_Float, &this->camera->get_position()[1], &min_y, &max_y);
        ImGui::SliderScalar("Z", ImGuiDataType_Float, &this->camera->get_position()[2], &min_depth, &max_depth);
        
        this->camera->set_position(this->camera->get_position());
        
        ImGui::TreePop();
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 20.0f));
        ImGui::PushFont(io.FontDefault);
        if (ImGui::Begin("Entity Info", &(Editor_layer::dockSpace_open), ImGuiWindowFlags_None))
        {
          ImGui::Text("Properties : %s", this->camera->to_string().c_str());
        }
        ImGui::PopFont();
        ImGui::PopStyleVar();
        ImGui::End();
      }
      ImGui::PopFont();
    }
    ImGui::End();  // Scene hierarchy
    ImGui::PopStyleVar();  // Window padding
    
    if (ImGui::Begin("System Info", &(Editor_layer::dockSpace_open), ImGuiWindowFlags_None))
    {
      ImGui::Text("Application performance :\t%.3f ms/frame (%d FPS)", 1000.0f * time_step,
                  static_cast<int>(Engine::get_engine_framerate()));
      auto framebuffer_viewport_gl = dynamic_cast<Gl_framebuffer *>(this->framebuffer_viewport_data.viewport.get());
      ImGui::Text("Framebuffer size :\t(%.2f, %.2f)", framebuffer_viewport_gl->get_options().width,
                  framebuffer_viewport_gl->get_options().height);
    }
    ImGui::End();  // Stats
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    ImGuiDockNode *viewport_dock_node;
    viewport_dock_node = ImGui::DockBuilderGetNode(this->viewport_panel_dock_id);
    viewport_dock_node->HostWindow->DrawList->Flags |= ImDrawListFlags_AntiAliasedLines;
    
    if (this->framebuffer_viewport_data.viewport->get_options().samples == 1)
    {
      // Traditional mono-sampled framebuffer.
      uint64_t texture_id = this->framebuffer_viewport_data.viewport->get_color_attachment();
      ImVec2 viewport_size = ImGui::GetContentRegionAvail();
      ImGui::Image(reinterpret_cast<void *>(texture_id),
                   ImVec2(viewport_size.x, viewport_size.y),
                   ImVec2(0, 1), ImVec2(1, 0));
    } else
    {
      viewport_dock_node->HostWindow->DrawList->AddCallback(draw_viewport_quad,
                                                            &this->framebuffer_viewport_data);
    }
    ImGui::End();  // Viewport
    ImGui::PopStyleVar();  // Window padding.
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
    GLint lastProgram = 0, last_vao = 0, last_vbo = 0, last_ibo = 0, depth_blend_function = 0;
    CHECK_GL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram));
    CHECK_GL_CALL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vao));
    CHECK_GL_CALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_vbo));
    CHECK_GL_CALL(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_ibo));
    CHECK_GL_CALL(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &depth_blend_function));
    
    CHECK_GL_CALL(glEnable(GL_SAMPLE_SHADING));
    
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
    CHECK_GL_CALL(glActiveTexture(GL_TEXTURE1));
    CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebuffer_viewport_gl->get_color_attachment()));
    CHECK_GL_CALL(glActiveTexture(GL_TEXTURE2));
    CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebuffer_viewport_gl->get_depth_attachment()));
    
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_color_attachment_sampler", 1);
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_viewport_width", (int) viewport->Size.x);
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_viewport_height", (int) viewport->Size.y);
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_max_samples",
                                                               (int) framebuffer_data->viewport->get_options().samples);
    
    // Draw viewport quad
    framebuffer_viewport_gl->data.vao->bind();
    framebuffer_viewport_gl->data.vao->get_vertex_buffers().back()->bind();
    framebuffer_viewport_gl->data.vao->get_index_buffer()->bind();
    
    CHECK_GL_CALL(glDrawElements(GL_TRIANGLES, framebuffer_viewport_gl->data.vao->get_index_buffer()->get_count(),
                                 GL_UNSIGNED_INT, nullptr));
    
    framebuffer_viewport_gl->data.vao->unbind();
    framebuffer_viewport_gl->data.vao->get_vertex_buffers().back()->unbind();
    framebuffer_viewport_gl->data.vao->get_index_buffer()->unbind();
    
    CHECK_GL_CALL(glDisable(GL_SAMPLE_SHADING));
    
    // Unbind all buffers.
    CHECK_GL_CALL(glActiveTexture(GL_TEXTURE1));
    CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    CHECK_GL_CALL(glActiveTexture(GL_TEXTURE2));
    CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    framebuffer_data->framebuffer_viewport_shader->unbind();
    
    // Reset Imgui OpenGL buffers for next draw commands.
    CHECK_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_ibo));
    CHECK_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, last_vbo));
    CHECK_GL_CALL(glBindVertexArray(last_vao));
    CHECK_GL_CALL(glUseProgram(lastProgram));
  }
}