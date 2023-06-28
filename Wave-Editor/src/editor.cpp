//
// Created by nami on 23/04/23.
//

#include <editor.h>
#include <editor_layer.h>
#include <entrypoint.h>

namespace Wave
{
  
  /* This represents the imgui title bar height for the viewport docked window, used to determine the actual content
   * height of the framebuffer for the viewport by subtracting it from the full height.
   *
   * Note that the actual framebuffer boundaries do take into account the title bar height, due to how the content size
   * is calculated --> (viewport y cursor position += title bar height); (viewport full height - position height).
   */
  static float s_viewport_title_bar_height = 0.0f;
  static float s_scene_hierarchy_panel_width = 0.0f;
  static ImGuiWindow *s_viewport_window = nullptr;
  
  Editor::Editor() : Engine(Renderer_api::OpenGL, Context_api_e::Glfw,
                            Engine::App_type::Editor)
  {
    // Add Cameras
    this->active_scene = std::make_shared<Scene>();
    this->entities.emplace_back(this->active_scene->create_entity("Editor Camera"));
    
    this->entities.back().add_component<std::shared_ptr<Camera>>(
      std::make_shared<Editor_camera>(Engine::get_main_window()->get_width(),
                                      Engine::get_main_window()->get_height(),
                                      90.0f, 0.1f, 1000.0f));
    
    // Add objects
    this->entities.emplace_back(this->active_scene->create_entity("Object : Sphere"));
    auto sphere = Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/sphere.obj"), nullptr,
                                 (int32_t) (uint32_t) this->entities.back());
    this->entities.back().add_component<std::shared_ptr<Object>>(sphere);
    
    this->entities.emplace_back(this->active_scene->create_entity("Object : Awp"));
    auto awp = Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/awp.obj"),
                              sphere->get_shader(), (int32_t) (uint32_t) this->entities.back());
    this->entities.back().add_component<std::shared_ptr<Object>>(awp);
    
    this->entities.emplace_back(this->active_scene->create_entity("Object : Cube"));
    auto cube = Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/cube.obj"),
                               nullptr, (int32_t) (uint32_t) this->entities.back());
    cube->enable_flat_shading();
    this->entities.back().add_component<std::shared_ptr<Object>>(cube);
    
    this->entities.emplace_back(this->active_scene->create_entity("Point Light : light bulb"));
    auto light_bulb = Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/Lamp.obj"),
                                     nullptr, (int32_t) (uint32_t) this->entities.back());
    this->entities.back().add_component<std::shared_ptr<Object>>(light_bulb);
    
    // Add framebuffers.
    
    // Setup default framebuffer's attachments.
    Framebuffer_attachment_s color_attachment = {Framebuffer_target_e::Color_attachment, 0, 2};
    
    // Add id attachment to framebuffer for mouse picking.
    Framebuffer_attachment_s ms_id_attachment = {Framebuffer_target_e::ID_attachment, 1, 3};
    Framebuffer_attachment_s non_ms_id_attachment = {Framebuffer_target_e::ID_attachment, 1, 5};
    
    Framebuffer_attachment_s depth_attachment = {Framebuffer_target_e::Depth_stencil_attachment, 0, 4};
    
    Framebuffer_options_s ms_options = {Engine::get_main_window()->get_framebuffer_width(),
                                        Engine::get_main_window()->get_framebuffer_height(),
                                        {color_attachment, ms_id_attachment, depth_attachment},
                                        Engine::get_main_window()->get_samples()};
    
    Framebuffer_options_s non_ms_options = {Engine::get_main_window()->get_framebuffer_width(),
                                            Engine::get_main_window()->get_framebuffer_height(),
                                            {non_ms_id_attachment},
                                            1};
    
    this->viewport_resolution = {(float) ms_options.width, (float) ms_options.height};
    this->viewport_ms_framebuffer = Framebuffer::create(ms_options);
    this->viewport_non_ms_framebuffer = Framebuffer::create(non_ms_options);
    
    // Add text strings
    this->entities.emplace_back(this->active_scene->create_entity("Text Box : Title"));
    auto text_box = Text_box::create(Math::Vector_2f(0.0f, 50.0f), "~ Wave Engine ~", nullptr,
                                     (int32_t) (uint32_t) this->entities.back());
    text_box->append_text(" By : Nami Reghbati ");
    
    this->entities.back().add_component<std::shared_ptr<Text_box>>(text_box);
    
    this->entities.emplace_back(this->active_scene->create_entity("Text Box : Menu Title"));
    auto text_box_2 = Text_box::create(Math::Vector_2f(0.0f, 50.0f), "Game Title", nullptr,
                                       (int32_t) (uint32_t) this->entities.back());
    text_box_2->set_text_offset_x(text_box->get_text_length() + 50.0f);
    
    this->entities.back().add_component<std::shared_ptr<Text_box>>(text_box_2);
  }
  
  void Editor::on_init()
  {
    Engine::on_init();
    
    this->viewport_ms_framebuffer->bind();
    this->viewport_non_ms_framebuffer->bind();
    
    push_overlay(new ImGui_layer());
    push_layer(new Editor_layer(this->active_scene, this->entities, this->viewport_ms_framebuffer));
    push_layer(new Text_layer(this->entities, &this->viewport_resolution, true));
  }
  
  void Editor::on_destroy()
  {
    for (auto &shader: this->demo_shaders) if (Renderer::is_running()) shader->free_gpu(1);
    if (Renderer::is_running()) this->viewport_ms_framebuffer->free_gpu(1);
    delete this->selected_entity;
    
    Engine::shutdown();
    Engine::on_destroy();
  }
  
  void Editor::on_event(Event_system::Event &event)
  {
    Engine::on_event(event);
    switch (event.get_event_type())
    {
      case Wave::Event_system::Event_type::None:return;
      case Wave::Event_system::Event_type::On_framebuffer_resize:
      {
        on_viewport_resize(dynamic_cast<Event_system::On_framebuffer_resize &>(event));
        break;
      }
      case Wave::Event_system::Event_type::On_mouse_button_press:
      {
        /********************** MOUSE PICKING **********************/
        
        Math::Vector_2f position = Input::get_mouse_cursor_position();
        
        Math::Vector_2f viewport_size = Math::Vector_2f(this->viewport_framebuffer_boundaries[2],
                                                        this->viewport_framebuffer_boundaries[3]);
        // Set cursor offset for viewport window.
        position.set_x(position.get_x() - s_scene_hierarchy_panel_width);
        // Flip y-axis to start at the bottom left of the viewport, in order to match it with texture coordinates,
        // since the ID attachment texture is not flipped.
        position.set_y((viewport_size.get_y() + s_viewport_title_bar_height) - position.get_y());
        
        // If mouse is within the viewport window.
        if ((int) position.get_x() >= 0 && (int) position.get_y() >= 0 &&
            (int) position.get_x() <= (int) viewport_size.get_x() &&
            (int) position.get_y() <= (int) viewport_size.get_y() - (int) s_viewport_title_bar_height)
        {
          // Setup default framebuffer and copy the ID attachment texture info from the original framebuffer, since
          // we cannot just read pixels from a multi-sampled framebuffer.
          this->viewport_ms_framebuffer->blit_color_attachments((int32_t) this->viewport_non_ms_framebuffer->get_id(),
                                                                {this->viewport_ms_framebuffer->get_color_attachments()[1]});
          
          int pixelData = this->viewport_non_ms_framebuffer->read_pixel(1, (int32_t) position.get_x(),
                                                                        (int32_t) position.get_y());
          
          if (pixelData != -1)
          {
            delete this->selected_entity;
            this->selected_entity = new Entity((entt::entity) pixelData, this->active_scene.get());
            Editor_layer::scene_panel.set_selected_entity(*this->selected_entity);
          } else
          {
            delete this->selected_entity;
            this->selected_entity = nullptr;
          }
        }
        break;
      }
      case Wave::Event_system::Event_type::On_mouse_wheel_scroll:
      {
        auto view = this->active_scene->get_all_entities_with<std::shared_ptr<Camera>>();
        for (const auto &entity: view) view.get<std::shared_ptr<Camera>>(entity)->on_event(event);
        break;
      }
      default: break;
    }
  }
  
  bool Editor::on_viewport_resize(Event_system::On_framebuffer_resize &resize)
  {
    this->viewport_framebuffer_boundaries = Math::Vector_4f(resize.get_position_x(),
                                                            resize.get_position_y(),
                                                            resize.get_width(),
                                                            resize.get_height() + s_viewport_title_bar_height);
    
    this->viewport_ms_framebuffer->resize((int32_t) resize.get_width(),
                                          (int32_t) resize.get_height(),
                                          &this->viewport_framebuffer_boundaries);
    
    this->viewport_non_ms_framebuffer->resize((int32_t) resize.get_width(),
                                              (int32_t) resize.get_height(),
                                              &this->viewport_framebuffer_boundaries);
    return false;
  }
  
  void Editor::on_update(float time_step)
  {
    Engine::on_update(time_step);
    if (this->active_scene->get_camera("Editor Camera") != nullptr)
      this->active_scene->on_update_editor(time_step, *this->active_scene->get_camera("Editor Camera"));
  }
  
  void Editor::on_game_render()
  {
    if (s_viewport_window)
    {
      ImVec2 size = s_viewport_window->SizeFull;
      s_viewport_title_bar_height = s_viewport_window->TitleBarHeight();
      
      // Redraw framebuffer on resize.
      if (size.x > 0.0f && size.y - s_viewport_title_bar_height > 0.0f &&
          (size.x != (float) this->viewport_ms_framebuffer->get_options().width ||
           size.y - s_viewport_title_bar_height != (float) this->viewport_ms_framebuffer->get_options().height
           || s_viewport_window->Pos.x != this->viewport_framebuffer_boundaries.get_x() ||
           s_viewport_window->Pos.y != this->viewport_framebuffer_boundaries.get_y()))
      {
        Event_system::On_framebuffer_resize framebuffer_resized(s_viewport_window->Pos.x,
                                                                s_viewport_window->Pos.y,
                                                                size.x, size.y - s_viewport_title_bar_height);
        Window::get_event_callback_function()(framebuffer_resized);
      }
    }
    this->viewport_ms_framebuffer->bind();
    Renderer::set_clear_color(this->background_clear_color);
    Renderer::clear_bg();
    this->viewport_ms_framebuffer->clear_attachment(1, -1);
    Renderer::begin(*this->active_scene->get_camera("Editor Camera"));
    Engine::on_game_render();
    Renderer::end();
    this->viewport_ms_framebuffer->unbind();
  }
  
  void Editor::on_ui_render(float time_step)
  {
    ImGui_layer::begin();
    s_viewport_window = ImGui::FindWindowByName("Viewport");
    auto scene_panel = ImGui::FindWindowByName("Scene Editor");
    scene_panel && ImGui_layer::show_scene_panel ? s_scene_hierarchy_panel_width = scene_panel->Size.x :
      s_scene_hierarchy_panel_width = 0.0f;
    Engine::on_ui_render(time_step);
    
    ImGui::Begin("System Info");
    ImGui::Text("Selected Entity --> %s", this->selected_entity ? this->selected_entity->get_name().c_str() : "None");
    ImGui::End();
    
    ImGui_layer::end();
  }
  
  Engine *create_app()
  {
    return new Editor();
  }
}
