//
// Created by nami on 23/04/23.
//

#include <editor_layer.h>

namespace Wave
{
  
  bool Editor_layer::system_panel_dock_open = true;
  
  Editor_layer::Editor_layer(const std::shared_ptr<Wave::Camera> &demo_perspective_camera_,
                             const std::vector<std::shared_ptr<Wave::Shader>> &shaders_,
                             const std::vector<std::shared_ptr<Wave::Object>> &demo_objects_,
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
    this->objects[0]->add_texture(Resource_loader::load_texture_source("../Wave/res/Textures/tiles.png",
                                                                       {Texture_type::Texture_2D,
                                                                        0, 0, 0, 1, 1}));
    
    // Setup objects in scene.
    this->objects[0]->translate(10, -10, 20);
    this->objects[0]->rotate(90, -90, 0);
    
    // Setup framebuffer shader.
    this->framebuffer_viewport_data.framebuffer_viewport_shader = Shader::create("Framebuffer Editor Viewport",
                                                                                 Resource_loader::load_shader_source(
                                                                                   "../Wave-Editor/res/Shaders/viewport_framebuffer_ms.vert").c_str(),
                                                                                 Resource_loader::load_shader_source(
                                                                                   "../Wave-Editor/res/Shaders/viewport_framebuffer_ms.frag").c_str());
    this->shaders[0]->bind();
    this->shaders[0]->set_uniform("u_has_texture", true);
    this->shaders[0]->set_uniform("u_sampler", 1);
    Renderer::draw_object(this->objects[0], this->shaders[0]);
    this->shaders[0]->unbind();
  }
  
  void Editor_layer::on_detach()
  {
  }
  
  void Editor_layer::on_event([[maybe_unused]] Event &event)
  {
    this->camera->on_event(event);
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
  }
  
  void Editor_layer::on_render()
  {
    ImGuiIO io = ImGui::GetIO();
    
    ImGuiViewport *viewport_ = ImGui::GetMainViewport();
    
    /* Important to set next viewport window, because otherwise a window node will clip out of the main viewport when
     * dragged outside the main window, which is annoying when trying to re-dock a node by dragging it to the very edge,
     * and it's separating from the dockSpace instead.
    */
    
    // Render System panel UI.
    ImGui::SetNextWindowViewport(viewport_->ID);
    
    if (Editor_layer::system_panel_dock_open)
    {
      if (ImGui::Begin("System Info", &(Editor_layer::system_panel_dock_open), ImGuiWindowFlags_None))
      {
        ImGui::Text("Application performance :\t%.3f ms/frame (%d FPS)", 1000.0f * Engine::get_time_step(),
                    static_cast<int>(Engine::get_engine_framerate()));
        auto framebuffer_viewport_gl = dynamic_cast<Gl_framebuffer *>(this->framebuffer_viewport_data.viewport.get());
        ImGui::Text("Framebuffer size :\t(%.2f, %.2f)", framebuffer_viewport_gl->get_options().width,
                    framebuffer_viewport_gl->get_options().height);
      }
      ImGui::End();  // Stats
    }
    
    // Render scene UI.
    ImGui::SetNextWindowViewport(viewport_->ID);
    
    io.Fonts->Fonts[1]->Scale = this->scene_panel.get_font_scale();
    this->scene_panel.on_render();
    io.Fonts->Fonts[1]->Scale = 1.0f;  // Reset scale for other components.
    
    // Render Viewport framebuffer UI.
    ImGui::SetNextWindowViewport(viewport_->ID);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    ImGuiDockNode *viewport_dock_node;
    viewport_dock_node = ImGui::DockBuilderGetNode(ImGui_layer::viewport_panel_dock_id);
    if (viewport_dock_node) viewport_dock_node->HostWindow->DrawList->Flags |= ImDrawListFlags_AntiAliasedLines;
    
    if (this->framebuffer_viewport_data.viewport->get_options().samples == 1)
    {
      // Traditional mono-sampled framebuffer.
      uint64_t texture_id = this->framebuffer_viewport_data.viewport->get_color_attachment();
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
    } else if (viewport_dock_node->Windows.Size > 0)
    {
      // Add custom callback to draw texture multi-sampled framebuffer onto the docked window.
      viewport_dock_node->HostWindow->DrawList->AddCallback(draw_viewport_quad, &this->framebuffer_viewport_data);
    } else
    {
      // Add the custom callback when the window is undocked and being popped out of the node container.
      viewport_dock_node->Windows.Data[0]->DrawList->AddCallback(draw_viewport_quad,
                                                                 &this->framebuffer_viewport_data);
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
    framebuffer_data->framebuffer_viewport_shader->unbind();
    
    // Unbind all buffers.
    CHECK_GL_CALL(glActiveTexture(GL_TEXTURE1));
    CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    CHECK_GL_CALL(glActiveTexture(GL_TEXTURE2));
    CHECK_GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    
    // Reset Imgui OpenGL buffers for next draw commands.
    CHECK_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_ibo));
    CHECK_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, last_vbo));
    CHECK_GL_CALL(glBindVertexArray(last_vao));
    CHECK_GL_CALL(glUseProgram(lastProgram));
  }
}