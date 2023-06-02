//
// Created by nami on 23/04/23.
//

#include <editor.h>
#include <editor_layer.h>
#include <entrypoint.h>

namespace Wave
{
  
  
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
      Object::create(Resource_loader::load_object_3D_source("../Wave/res/Models/awp.obj")));
    
    // Add framebuffer.
    int32_t max_samples = Engine::get_main_window()->get_samples();
    // Setup default viewport framebuffer specs.
    Framebuffer_options fbSpec;
    fbSpec.width = Engine::get_main_window()->get_width();  // Fullscreen size.
    fbSpec.height = Engine::get_main_window()->get_height();  // Fullscreen size.
    fbSpec.samples = max_samples / 4 <= 4 ? max_samples : max_samples / 4;
    this->viewport_resolution = {fbSpec.width,
                                 fbSpec.height};
    this->viewport_framebuffer = Framebuffer::create(fbSpec);
    
    // Add text strings
    this->demo_texts.emplace_back(Text_box::create(Vector_2f(0.0f, 50.0f), "~ Wave Engine ~"));
    this->demo_texts[0]->set_text_offset_y(this->viewport_resolution.get_y() -
                                           this->demo_texts[0]->get_pixel_size().get_y());
    this->demo_texts[0]->set_text_color('~', Color(0xFF0000FF));
    this->demo_texts.emplace_back(Text_box::create(Vector_2f(0.0f, 50.0f), "Namgame222"));
    this->demo_texts[1]->set_text_offset_x(this->demo_texts[0]->get_text_length());
    this->demo_texts[1]->set_text_offset_y(this->viewport_resolution.get_y() -
                                           this->demo_texts[0]->get_pixel_size().get_y());
    this->demo_texts[1]->set_text_color(Color(0x0000FFFF));
  }
  
  void Editor::build()
  {
    Engine::build();
    
    push_layer(new Editor_layer(this->editor_camera,
                                this->demo_shaders,
                                this->demo_objects,
                                this->viewport_framebuffer));
    push_layer(new Text_layer(this->demo_texts, this->demo_shaders, this->viewport_resolution,
                              true));
    push_layer(new ImGui_layer());
  }
  
  void Editor::unbuild()
  {
    for (auto &shader: this->demo_shaders) shader->unbuild();
    for (auto &text_box: this->demo_texts) text_box->unbuild();
    for (auto &object: this->demo_objects) object->unbuild();
    this->viewport_framebuffer->unbuild();
    
    Engine::unbuild();
  }
  
  void Editor::on_update(float time_step)
  {
    Engine::on_update(time_step);
  }
  
  void Editor::on_render()
  {
    if (!Engine::get_main_window()->is_minimized()) ImGui_layer::begin();
    
    auto viewport_undocked = ImGui::FindWindowByName("Viewport");
    if (viewport_undocked)  // If viewport is detached from DockSpace.
    {
      ImVec2 size = viewport_undocked->Size;
      float title_tab_height = viewport_undocked->TitleBarHeight();
      
      // Redraw framebuffer on resize.
      if (size.x > 0.0f && size.y - title_tab_height > 0.0f &&
          (size.x != this->viewport_framebuffer->get_options().width ||
           size.y - title_tab_height != this->viewport_framebuffer->get_options().height
           || viewport_undocked->Pos.x != this->viewport_framebuffer_boundaries.get_x() ||
           viewport_undocked->Pos.y != this->viewport_framebuffer_boundaries.get_y()))
      {
        this->viewport_framebuffer_boundaries = Vector_4f(viewport_undocked->Pos.x, viewport_undocked->Pos.y, size.x,
                                                          size.y);
        this->viewport_framebuffer->resize(size.x, size.y - title_tab_height, &this->viewport_framebuffer_boundaries);
      }
    }
    
    this->viewport_framebuffer->bind();
    Renderer::begin(this->editor_camera);
    Renderer::set_clear_color(this->background_clear_color);
    Renderer::clear_bg();
    Engine::on_render();
    Renderer::end();
    this->viewport_framebuffer->unbind();
    
    if (!Engine::get_main_window()->is_minimized()) ImGui_layer::end();
  }
  
  Engine *create_app()
  {
    return new Editor();
  }
}
