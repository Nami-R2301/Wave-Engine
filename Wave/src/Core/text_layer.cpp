//
// Created by nami on 24/04/23.
//

#include "Events/app_event.h"
#include <Core/text_layer.h>

namespace Wave
{
  
  Text_layer::Text_layer(const std::vector<std::shared_ptr<Text>> &strings_,
                         const std::vector<std::shared_ptr<Shader>> &text_shaders_,
                         const Vector_2f &viewport_size_)
  {
    this->strings = strings_;
    this->text_shaders = text_shaders_;
    this->viewport_size = viewport_size_;
    this->transform = glm::ortho(0.0f, this->viewport_size.get_x(), 0.0f, this->viewport_size.get_y());
  }
  
  Text_layer::~Text_layer()
  {
    Text_layer::on_detach();
  }
  
  void Text_layer::on_attach()
  {
    Gl_renderer::set_clear_color(Color(0.05, 1.0f, true));
    this->vao_list.emplace_back(Gl_renderer::load_text());
    this->text_shaders[0]->bind();
    this->text_shaders[0]->set_uniform("u_sampler", 1);
    this->text_shaders[0]->unbind();
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
    this->text_shaders[0]->unbind();
  }
  
  void Text_layer::on_event(Event &event)
  {
    switch (event.get_event_type())
    {
      case Wave::Event_type::None:return;
      case Wave::Event_type::On_window_resize:
      {
        auto resize_event = dynamic_cast<On_window_resize &>(event);
        this->transform = glm::ortho(0.0f, resize_event.get_width(), 0.0f, resize_event.get_height());
        this->strings[0]->set_offset_y(this->viewport_size.get_y() - 25.0f);
        break;
      }
      default:break;
    }
  }
  
  void Text_layer::on_ui_render(float time_step)
  {
  
  }
}