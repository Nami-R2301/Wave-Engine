//
// Created by nami on 23/04/23.
//

#include <editor_layer.h>

namespace Wave
{
  Scene_ui_panel Editor_layer::scene_panel = {nullptr, {}};
  static float s_imgui_app_performance_stat = 0.0f;
  static float s_imgui_app_performance_timer = 0.0f;
  
  Editor_layer::Editor_layer(const std::shared_ptr<Scene> &active_scene_,
                             const std::vector<Entity> &entities_,
                             const std::shared_ptr<Framebuffer> &viewport_) : entities(entities_)
  {
    Editor_layer::scene_panel = Scene_ui_panel(active_scene_, {});
    this->layer_name = "Editor layer";
    this->framebuffer_viewport_data.viewport = viewport_;
    // Setup framebuffer shader.
    this->framebuffer_viewport_data.framebuffer_viewport_shader = Shader::create("Framebuffer Editor Viewport",
                                                                                 Resource_loader::load_shader_source(
                                                                                   "../Wave-Editor/res/Shaders/viewport_framebuffer_ms.vert"),
                                                                                 Resource_loader::load_shader_source(
                                                                                   "../Wave-Editor/res/Shaders/viewport_framebuffer_ms.frag"));
    for (auto &entity: this->entities)
    {
      if (entity.has_component<std::shared_ptr<Object>>())
        this->objects.emplace_back(entity.get_component<std::shared_ptr<Object>>());
      
      if (entity.has_component<std::shared_ptr<Camera>>())
        this->camera = entity.get_component<std::shared_ptr<Camera>>();
    }
  }
  
  void Editor_layer::on_attach()
  {
    this->camera->set_position(0, 0, 0.0f);
    // Setup objects transformations.
    this->objects[0]->translate(-5, 2, 5);
    this->objects[0]->rotate(90, 0, 0);
    
    this->objects[1]->translate(10, -10, 20);
    this->objects[1]->rotate(90, -90, 0);
    
    this->objects[2]->translate(10, 0, 10);
    this->objects[2]->rotate(45, 0, 0);
    
    this->objects[3]->scale(0.1f, 0.1f, 0.1f);
    
    // Setup how object behaves with lighting.
//    Point_light point_light = Point_light(Color(0xFFFFFFFF), 1.0f, 1.0f,
//                                          Math::Vector_3f(0, 0, 0),
//                                          0.3f, 0.2f, 0.1f);
    Point_light point_light_2 = Point_light(Color(0xFFFFFFFF), 0.0f, 0.5f,
                                            this->camera->get_position(),
                                            0.3f, 0.2f, 0.1f);

//    this->objects[2]->add_texture(Resource_loader::load_texture_source("../Wave/res/Textures/tiles.png"));
    
    this->objects[0]->calculate_effect_by_light(point_light_2);
    this->objects[2]->calculate_effect_by_light(point_light_2);
  }
  
  void Editor_layer::on_detach()
  {
    this->framebuffer_viewport_data.framebuffer_viewport_shader->free_gpu(1);
  }
  
  void Editor_layer::on_event([[maybe_unused]] Event &event)
  {
    this->camera->on_event(event);
  }
  
  void Editor_layer::on_update(float time_step)
  {
    if (s_imgui_app_performance_timer == 0) s_imgui_app_performance_stat = Engine::get_time_step();
    s_imgui_app_performance_timer += time_step;  // Update imgui performance counter.
    this->camera->on_update(time_step);
//    this->objects[1]->rotate(time_step * 10, 0, 0);
//    this->objects[1]->send_gpu(1);
    
    this->objects[0]->rotate((cosf((float) glfwGetTime())), 0,
                             (sinf((float) glfwGetTime())));
//    this->objects[2]->rotate((cosf((float) glfwGetTime())), 0,
//                             (sinf((float) glfwGetTime())));
    
    this->objects[0]->translate((cosf((float) glfwGetTime()) * 20), 0,
                                (sinf((float) glfwGetTime()) * 20) + 10);
    this->objects[0]->send_gpu(1);
    Point_light point_light_2 = Point_light(Color(0xFFFFFFFF), 0.0f, 0.5f,
                                            this->camera->get_position(),
                                            0.3f, 0.2f, 0.1f);
    this->objects[2]->calculate_effect_by_light(point_light_2);
  }
  
  void Editor_layer::on_render()
  {
  }
  
  void Editor_layer::on_ui_render(float time_step)
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
    
    if (ImGui_layer::show_system_panel)
    {
      if (ImGui::Begin("System Info", &(ImGui_layer::show_system_panel), ImGuiWindowFlags_None))
      {
        // Slow down the refresh rate of the text output by timing it with each second.
        if (s_imgui_app_performance_timer >= 1.0f)
        {
          ImGui::Text("Application performance :\t%.3f ms/frame (%d FPS)",
                      Engine::get_time_step() * 1000.0f,
                      (int) (1.0f / s_imgui_app_performance_stat));
          s_imgui_app_performance_timer = 0;
        } else
        {
          ImGui::Text("Application performance :\t%.3f ms/frame (%d FPS)",
                      s_imgui_app_performance_stat * 1000.0f,
                      (int) (1.0f / s_imgui_app_performance_stat));
        }
        auto framebuffer_viewport_gl = dynamic_cast<Gl_framebuffer *>(this->framebuffer_viewport_data.viewport.get());
        ImGui::Text("Viewport resolution :\t(%d, %d)",
                    framebuffer_viewport_gl->get_options().width,
                    framebuffer_viewport_gl->get_options().height);
        
        ImGui::Text("Framebuffer resolution :\t(%d, %d)", Engine::get_main_window()->get_width(),
                    Engine::get_main_window()->get_height());
        
        ImGui::Text("UI font size :\t%.2f", ImGui_layer::imgui_data.font_size * ImGui_layer::imgui_data.font_scale);

//        ImGui::Text("Clicked entity UUID : %d", this->hovered_entity.get_uuid());
      }
      ImGui::End();  // System Info.
      auto node = ImGui::DockBuilderGetNode(ImGui_layer::events_panel_dock_id);
      if (node)
      {
        ImGui::DockNodeBeginAmendTabBar(node);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
        
        if (ImGui::BeginTabItem("Renderer Info", &ImGui_layer::show_system_panel, ImGuiTabItemFlags_Trailing))
        {
          auto renderer_stats = Renderer::get_stats();
          ImGui::Text("Renderer 3D");
          ImGui::SetCursorPosX(ImGui::GetCursorPosX() + Editor_layer::scene_panel.get_size_boundaries().get_z());
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
    
    // Render scene UI.
    ImGui::SetNextWindowViewport(viewport_->ID);
    
    io.Fonts->Fonts[1]->Scale = Editor_layer::scene_panel.get_font_scale();
    Editor_layer::scene_panel.on_ui_render();
    io.Fonts->Fonts[1]->Scale = 1.0f;  // Reset scale for other components.
    
    // Render Viewport framebuffer UI.
    ImGui::SetNextWindowViewport(viewport_->ID);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("Viewport"))
    {
      auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
      auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
      auto viewportOffset = ImGui::GetWindowPos();
      Math::Vector_2f min_bounds = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
      Math::Vector_2f max_bounds = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};
      
      // Gizmos
      Entity selectedEntity = Editor_layer::scene_panel.get_selected_entity();
      if (selectedEntity)
      {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        
        ImGuizmo::SetRect(min_bounds.get_x(), min_bounds.get_y(),
                          max_bounds.get_x() - min_bounds.get_x(),
                          max_bounds.get_y() - min_bounds.get_y());
        
        // Camera
        
        // Runtime camera from entity
        // auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
        // const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
        // const glm::mat4& cameraProjection = camera.GetProjection();
        // glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
        
        // Editor camera
        const Math::Matrix_4f &cameraProjection = this->camera->get_projection_matrix();
        Math::Matrix_4f cameraView = this->camera->get_view_matrix();
        
        // Entity transform
        if (selectedEntity.has_component<std::shared_ptr<Object>>())
        {
          auto &tc = selectedEntity.get_component<std::shared_ptr<Object>>();
          Math::Matrix_4f model_matrix = tc->get_model_matrix();
          const Math::Transform &transform = tc->get_model_transform();
          this->guizmo_type = ImGuizmo::OPERATION::TRANSLATE;
          
          // Snapping
          bool snap = Input::is_key_held(WAVE_KEY_LEFT_ALT);
          if (snap) this->guizmo_type = ImGuizmo::OPERATION::ROTATE;
          float snapValue = 0.5f; // Snap to 0.5m for translation/scale
          // Snap to 45 degrees for rotation
          if (this->guizmo_type == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;
          
          float snapValues[3] = {snapValue, snapValue, snapValue};
          
          ImGuizmo::Manipulate(&cameraView.get_matrix()[0][0], &cameraProjection.get_matrix()[0][0],
                               (ImGuizmo::OPERATION) this->guizmo_type, ImGuizmo::WORLD,
                               &model_matrix.get_matrix()[0][0],
                               nullptr, snap ? snapValues : nullptr);
          
          if (ImGuizmo::IsUsing())
          {
            Math::Vector_3f translation, rotation, scale;
            Wave::Math::Matrix_4f::decompose_matrix(model_matrix, translation, rotation, scale);
            if (this->guizmo_type == ImGuizmo::OPERATION::ROTATE &&
                rotation != transform.get_rotation())
              rotation += transform.get_rotation();
            else rotation = transform.get_rotation();
            
            tc->set_model_transform(Math::Transform(translation, rotation, scale));
            tc->send_gpu(1);
          }
        }
      }
      ImGuiDockNode *viewport_dock_node;
      viewport_dock_node = ImGui::DockBuilderGetNode(ImGui_layer::viewport_panel_dock_id);
      if (viewport_dock_node) viewport_dock_node->HostWindow->DrawList->Flags |= ImDrawListFlags_AntiAliasedLines;
      
      if (this->framebuffer_viewport_data.viewport->get_options().samples == 1)
      {
        // Traditional mono-sampled framebuffer.
        uint64_t texture_id = this->framebuffer_viewport_data.viewport->get_color_attachments()[0].attachment_texture->get_id();
        ImVec2 viewport_size = ImGui::GetContentRegionAvail();
        ImGui::Image(reinterpret_cast<void *>(texture_id),
                     ImVec2(viewport_size.x, viewport_size.y),
                     ImVec2(0, 1), ImVec2(1, 0));
        
        /* For some reason, if the window is being dragged outside the dock container, it will cease to call the framebuffer.
         * callback provided. After countless tests, it seems that the only quick workaround for this is to add the custom
         * callback via the Window list instead of the host window due to Window list decreasing in size when the window node
         * is undocked.
         *
         * NOTE : If we only pass the callback via the Window list, the window won't execute the custom callback
         * once it's docked again, so, we must cover both cases for a smoother and more robust user experience.
         *
        */
      } else if (viewport_dock_node && viewport_dock_node->Windows.Size > 0)
      {
        // Add custom callback to draw texture multi-sampled framebuffer onto the docked window.
        viewport_dock_node->HostWindow->DrawList->AddCallback(draw_viewport_quad, &this->framebuffer_viewport_data);
      } else if (viewport_dock_node && viewport_dock_node->Windows.Data)
      {
        // Add the custom callback when the window is undocked and being popped out of the node container.
        viewport_dock_node->Windows.Data[0]->DrawList->AddCallback(draw_viewport_quad,
                                                                   &this->framebuffer_viewport_data);
      } else
      {
        ImGui::GetWindowDrawList()->AddCallback(draw_viewport_quad,
                                                &this->framebuffer_viewport_data);
      }
    }
    ImGui::End(); // Viewport.
    ImGui::PopStyleVar();  // Window padding.
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
    
    framebuffer_data->framebuffer_viewport_shader->bind();
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_projection", &viewportProjection[0][0],
                                                               false);
    
    // Bind framebuffer attachments.
    for (const auto &color_attachment: framebuffer_viewport_gl->get_color_attachments())
    {
      // Bind color attachments.
      color_attachment.attachment_texture->bind(WAVE_VALUE_DONT_CARE);
    }
    
    if (framebuffer_viewport_gl->get_depth_attachment().attachment_texture)
      // Bind Depth attachment.
      framebuffer_viewport_gl->get_depth_attachment().attachment_texture->bind(WAVE_VALUE_DONT_CARE);
    
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_color_attachment_sampler",
                                                               framebuffer_viewport_gl->get_color_attachments()[0].attachment_texture->get_texture_slot());
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_viewport_width", (int) viewport->Size.x);
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_viewport_height", (int) viewport->Size.y);
    framebuffer_data->framebuffer_viewport_shader->set_uniform("u_max_samples",
                                                               (int) framebuffer_data->viewport->get_options().samples);
    
    // Draw viewport quad
    framebuffer_viewport_gl->data.vao->bind();
    framebuffer_viewport_gl->data.vao->get_index_buffer()->bind();
    
    CHECK_GL_CALL(glDrawElements(GL_TRIANGLES, framebuffer_viewport_gl->data.vao->get_index_buffer()->get_count(),
                                 GL_UNSIGNED_INT, nullptr));
    
    framebuffer_viewport_gl->data.vao->unbind();
    framebuffer_viewport_gl->data.vao->get_index_buffer()->unbind();
    framebuffer_data->framebuffer_viewport_shader->unbind();
    
    // Reset Imgui OpenGL buffers for next draw commands.
    CHECK_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_ibo));
    CHECK_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, last_vbo));
    CHECK_GL_CALL(glBindVertexArray(last_vao));
    CHECK_GL_CALL(glUseProgram(lastProgram));
  }
}