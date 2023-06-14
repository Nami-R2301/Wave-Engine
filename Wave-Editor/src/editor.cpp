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
    auto sphere = Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/sphere.obj"));
    auto awp = Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/awp.obj"),
                              sphere->get_shader());
    auto cube = Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/cube.obj"),
                               sphere->get_shader());
    
    this->entities.emplace_back(this->active_scene->create_entity("Object : Sphere"));
    this->entities.back().add_component<std::shared_ptr<Object>>(sphere);
    
    this->entities.emplace_back(this->active_scene->create_entity("Object : Awp"));
    this->entities.back().add_component<std::shared_ptr<Object>>(awp);
    
    this->entities.emplace_back(this->active_scene->create_entity("Object : Cube"));
    this->entities.back().add_component<std::shared_ptr<Object>>(cube);
    
    // Add framebuffers.
    
    // Setup default framebuffer's attachments.
    Framebuffer_attachment_s color_attachment = {Framebuffer_target_e::Color_attachment, 0, 2};
    Framebuffer_attachment_s depth_attachment = {Framebuffer_target_e::Depth_stencil_attachment, 0, 3};
    
    Framebuffer_options_s options = {Engine::get_main_window()->get_width(),
                                     Engine::get_main_window()->get_height(),
                                     {color_attachment, depth_attachment},
                                     Engine::get_main_window()->get_samples()};
    
    this->viewport_resolution = {options.width, options.height};
    this->viewport_framebuffer = Framebuffer::create(options);
    
    // Add text strings
    auto text_box = Text_box::create(Vector_2f(0.0f, 50.0f), "~ Wave Engine ~");
    text_box->append_text(" By : Nami Reghbati ");
    
    this->entities.emplace_back(this->active_scene->create_entity("Text Box : Title"));
    this->entities.back().add_component<std::shared_ptr<Text_box>>(text_box);
  }
  
  void Editor::on_init()
  {
    Engine::on_init();
    
    push_overlay(new ImGui_layer());
    push_layer(new Editor_layer(this->active_scene, this->entities, this->viewport_framebuffer));
    push_layer(new Text_layer(this->entities, &this->viewport_resolution, false));
    
    // Lastly, finalize by sending and enqueuing the object for rendering at a later stage (on_render()).
    this->viewport_framebuffer->send_gpu(1);
    this->active_scene->send_gpu();
  }
  
  void Editor::on_destroy()
  {
    for (auto &shader: this->demo_shaders) shader->free_gpu(1);
    this->viewport_framebuffer->free_gpu(1);
    
    Engine::shutdown();
    Engine::on_destroy();
  }
  
  void Editor::on_event(Event &event)
  {
    switch (event.get_event_type())
    {
      case Wave::Event_type::None:return;
      case Wave::Event_type::On_framebuffer_resize:
      {
        on_viewport_resize(dynamic_cast<On_framebuffer_resize &>(event));
        break;
      }
      case Wave::Event_type::On_mouse_wheel_scroll:
      {
        auto view = this->active_scene->get_all_entities_with<Editor_camera>();
        for (const auto &entity: view) view.get<Editor_camera>(entity).on_event(event);
        break;
      }
      default: break;
    }
    Engine::on_event(event);
  }
  
  bool Editor::on_viewport_resize(On_framebuffer_resize &resize)
  {
    auto framebuffer_resize = dynamic_cast<On_framebuffer_resize &>(resize);
    this->viewport_framebuffer_boundaries = Vector_4f(framebuffer_resize.get_position_x(),
                                                      framebuffer_resize.get_position_y(),
                                                      framebuffer_resize.get_width(),
                                                      framebuffer_resize.get_height() + s_viewport_title_bar_height);
    
    this->viewport_framebuffer->resize(framebuffer_resize.get_width(),
                                       framebuffer_resize.get_height(),
                                       &this->viewport_framebuffer_boundaries);
    return false;
  }
  
  void Editor::on_update(float time_step)
  {
    Engine::on_update(time_step);
  }
  
  void Editor::on_game_render()
  {
    ImGui_layer::begin();
    auto viewport_undocked = ImGui::FindWindowByName("Viewport");
    if (viewport_undocked)  // If viewport is detached from DockSpace.
    {
      ImVec2 size = viewport_undocked->SizeFull;
      s_viewport_title_bar_height = viewport_undocked->TitleBarHeight();
      
      // Redraw framebuffer on resize.
      if (size.x > 0.0f && size.y - s_viewport_title_bar_height > 0.0f &&
          (size.x != this->viewport_framebuffer->get_options().width ||
           size.y - s_viewport_title_bar_height != this->viewport_framebuffer->get_options().height
           || viewport_undocked->Pos.x != this->viewport_framebuffer_boundaries.get_x() ||
           viewport_undocked->Pos.y != this->viewport_framebuffer_boundaries.get_y()))
      {
        On_framebuffer_resize framebuffer_resized(viewport_undocked->Pos.x,
                                                  viewport_undocked->Pos.y,
                                                  size.x, size.y - s_viewport_title_bar_height);
        Window::get_event_callback_function()(framebuffer_resized);
      }
    }
    
    this->viewport_framebuffer->bind();
    Renderer::begin(this->active_scene->get_entity("Editor Camera").get_component<std::shared_ptr<Camera>>());
    Renderer::set_clear_color(this->background_clear_color);
    Renderer::clear_bg();
    Engine::on_game_render();
    Renderer::end();
    this->viewport_framebuffer->unbind();
  }
  
  void Editor::on_ui_render(float time_step)
  {
    Engine::on_ui_render(time_step);
    ImGui_layer::end();
  }
  
  Engine *create_app()
  {
    return new Editor();
  }
}
