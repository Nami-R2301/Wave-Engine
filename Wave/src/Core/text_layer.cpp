//
// Created by nami on 24/04/23.
//

#include <Core/text_layer.h>

namespace Wave
{
  
  Text_layer::Text_layer(const std::vector<std::shared_ptr<Text>> &strings_,
                         const std::vector<std::shared_ptr<Shader>>& text_shaders_)
  {
    this->strings = strings_;
    this->text_shaders = text_shaders_;
    this->transform = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);
  }
  
  Text_layer::~Text_layer()
  {
    Text_layer::on_detach();
  }
  
  void Text_layer::on_attach()
  {
    Gl_renderer::set_clear_color(Color(0.05, 1.0f, true));
    this->vao_list.emplace_back(Gl_renderer::load_text());
  }
  
  void Text_layer::on_detach()
  {
    for (const std::shared_ptr<Shader> &shader : this->text_shaders) shader->unbind();
  }
  
  void Text_layer::on_update(float time_step)
  {
    //  Update uniforms.
    
    this->text_shaders[0]->bind();
    this->text_shaders[0]->set_uniform("u_transform", this->transform);
    this->text_shaders[0]->set_uniform("u_text_color", this->strings[0]->get_format().color);
    
    Gl_renderer::draw_text(this->strings[0], this->vao_list.back());
  }
  
  void Text_layer::on_event(Event &event)
  {
  
  }
  
  void Text_layer::on_imgui_render(float time_step)
  {
  
  }
}