//
// Created by nami on 23/04/23.
//

#include <editor.h>
#include <editor_layer.h>
#include <entrypoint.h>

namespace Wave
{
  
  Editor::Editor() : Engine(Renderer_api::Opengl)
  {
    // Add Cameras
    this->demo_perspective_camera = create_shared_pointer<Editor_camera>(Engine::get_main_window()->get_width(),
                                                                         Engine::get_main_window()->get_height(),
                                                                         90.0f, 0.1f, 1000.0f);
    
    // Add shaders
    this->demo_shaders.emplace_back(Wave::Shader::create("Text",
                                                         Wave::Res_loader_3D::load_shader_source(
                                                             "../Wave/res/Shaders/text-glyph.vert").c_str(),
                                                         Wave::Res_loader_3D::load_shader_source(
                                                             "../Wave/res/Shaders/text-glyph.frag").c_str()));
    this->demo_shaders.emplace_back(Shader::create("Default",
                                                   Res_loader_3D::load_shader_source(
                                                       "../Wave/res/Shaders/default.vert").c_str(),
                                                   Res_loader_3D::load_shader_source(
                                                       "../Wave/res/Shaders/default.frag").c_str()));
    
    
    // Add objects
    this->demo_objects.emplace_back(
        create_shared_pointer<Object_3D>(Res_loader_3D("../Wave/res/Models/awp.obj").load_3D_mesh()));
    this->demo_objects.emplace_back(
        create_shared_pointer<Object_3D>(Res_loader_3D("../Wave/res/Models/cube.obj").load_3D_mesh()));
    
    // Add text strings
    Wave::Text_format format = {25.0f,
                                1080.0f - 38.0f,
                                1.0f,
                                26.0f,
                                Wave::Text_style::REGULAR,
                                Wave::Color(1.0f, 0.0f, 0.0f, 1.0f, true)};
    this->demo_texts.emplace_back(Text::create("../Wave/res/Fonts/Comfortaa/Comfortaa-Bold.ttf",
                                               "Wave Engine ~",
                                               format));
    
    // Setup default viewport framebuffer specs.
    Framebuffer_options fbSpec;
    fbSpec.width = 1920;
    fbSpec.height = 1080;
    this->viewport_size = {1920.0f,
                           1080.0f};
    this->viewport = Framebuffer::create(fbSpec);
    
    push_layer(new Editor_layer(this->demo_perspective_camera,
                                this->demo_shaders,
                                this->demo_objects,
                                this->viewport));
    push_layer(new Text_layer(this->demo_texts, this->demo_shaders));
    push_overlay(new ImGui_layer());
  }
  
  void Editor::init()
  {
  
  }
  
  void Editor::on_update(float time_step)
  {
    if (!Engine::get_main_window()->is_minimized()) ImGui_layer::begin();
    
    auto id = ImGui::FindWindowByName("Viewport");
    if (id)
    {
      ImVec2 size = id->Size;
      // Redraw framebuffer on resize.
      if (this->viewport_size.get_x() > 0.0f && this->viewport_size.get_y() > 0.0f &&
          (size.x != this->viewport_size.get_x() || size.y != this->viewport_size.get_y()))
      {
        this->viewport_boundaries = Vector_4f(id->Pos.x, id->Pos.y, size.x, size.y);
        this->viewport->resize(size.x,size.y,&this->viewport_boundaries);
        this->viewport_size = Vector_2f(size.x, size.y);
      }
    }
    
    this->viewport->bind();
    Gl_renderer::clear_bg();
    Engine::on_update(time_step);
    this->viewport->unbind();
    
    if (!Engine::get_main_window()->is_minimized())
    {
      ImGui_layer::end();
    }
  }
  
  void Editor::on_event(Event &event)
  {
    Engine::on_event(event);
    this->demo_perspective_camera->on_event(event);
  }
  
  bool Editor::window_closed_callback(On_window_close &window_closed_event)
  {
    window_closed_event.print(Print_type::Warn);
    Engine::main_window->close();
    return true;
  }
  
  bool Editor::window_resize_callback(On_window_resize &window_resized_event)
  {
    Engine::window_resize_callback(window_resized_event);
    this->demo_perspective_camera->on_window_resize(window_resized_event);
    return true;
  }
  
  Engine *create_app()
  {
    return new Editor();
  }
}