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
    this->editor_camera = std::make_shared<Editor_camera>(Engine::get_main_window()->get_width(),
                                                          Engine::get_main_window()->get_height(),
                                                          90.0f, 0.1f, 1000.0f);
    
    // Add shaders
    this->demo_shaders.emplace_back(Wave::Shader::create("Object",
                                                         Wave::Resource_loader::load_shader_source(
                                                           "../Wave/res/Shaders/default_3D.vert"),
                                                         Wave::Resource_loader::load_shader_source(
                                                           "../Wave/res/Shaders/default_3D.frag")));
    this->demo_shaders.emplace_back(Wave::Shader::create("Text_box",
                                                         Wave::Resource_loader::load_shader_source(
                                                           "../Wave/res/Shaders/text-glyph.vert"),
                                                         Wave::Resource_loader::load_shader_source(
                                                           "../Wave/res/Shaders/text-glyph.frag")));
    this->demo_shaders.emplace_back(Wave::Shader::create("Text_box",
                                                         Wave::Resource_loader::load_shader_source(
                                                           "../Wave/res/Shaders/text-glyph.vert"),
                                                         Wave::Resource_loader::load_shader_source(
                                                           "../Wave/res/Shaders/text-glyph.frag")));
    
    // Add objects
    this->demo_objects.emplace_back(
      Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/cube.obj")));
    
    // Add framebuffer.
    int32_t max_samples = Engine::get_main_window()->get_samples();
    // Setup default viewport framebuffer specs.
    Framebuffer_options fbSpec;
    fbSpec.width = Engine::get_main_window()->get_width();  // Windowed docked size.
    fbSpec.height = Engine::get_main_window()->get_height();  // Windowed docked size.
    fbSpec.samples = max_samples / 4 <= 4 ? max_samples : max_samples / 4;
    this->viewport_resolution = {fbSpec.width,
                                 fbSpec.height};
    this->viewport_framebuffer = Framebuffer::create(fbSpec);
    
    // Add text strings
    this->demo_texts.emplace_back(Text_box::create(Vector_2f(0.0f, 50.0f), "~ Wave Engine ~"));
    this->demo_texts[0]->set_text_offset_y((this->viewport_resolution.get_y() -
                                            this->demo_texts[0]->get_pixel_size().get_y()));
    this->demo_texts[0]->set_text_color('~', Color(0xFF0000FF));
    this->demo_texts[0]->append_text(" Nami Reghbati");
  }
  
  void Editor::on_init()
  {
    Engine::on_init();
    
    push_layer(new Editor_layer(this->editor_camera,
                                this->demo_shaders,
                                this->demo_objects,
                                this->viewport_framebuffer));
    push_layer(new Text_layer(this->demo_texts, this->demo_shaders, &this->viewport_resolution,
                              true));
    push_layer(new ImGui_layer());
  }
  
  void Editor::on_destroy()
  {
    for (auto &shader: this->demo_shaders) shader->free_gpu();
    for (auto &text_box: this->demo_texts) text_box->free_gpu();
    for (auto &object: this->demo_objects) object->free_gpu();
    this->viewport_framebuffer->free_gpu();
    
    Engine::on_destroy();
  }
  
  void Editor::on_event(Event &event)
  {
    switch (event.get_event_type())
    {
      case Wave::Event_type::None:return;
      case Wave::Event_type::On_framebuffer_resize:
      {
        Event_dispatcher viewport_dispatcher(event);
        viewport_dispatcher.dispatch<On_framebuffer_resize>(BIND_EVENT_FUNCTION(on_viewport_resize));
        break;
      }
      case Wave::Event_type::On_mouse_wheel_scroll:
      {
        this->editor_camera->on_event(event);
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
    this->viewport_framebuffer->bind();
    Renderer::begin(this->editor_camera);
    Renderer::set_clear_color(this->background_clear_color);
    Renderer::clear_bg();
    Engine::on_game_render();
    Renderer::end();
    this->viewport_framebuffer->unbind();
  }
  
  void Editor::on_ui_render(float time_step)
  {
    if (!Engine::get_main_window()->is_minimized())
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
      Engine::on_ui_render(time_step);
      ImGui_layer::end();
    }
  }
  
  Engine *create_app()
  {
    return new Editor();
  }
}
