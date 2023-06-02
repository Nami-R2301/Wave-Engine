//
// Created by nami on 24/04/23.
//

#include <imgui_internal.h>
#include <Layer/text_layer.h>

namespace Wave
{
  
  static Color s_imgui_text_uniform_color = Color(1.0f, 1.0f, true);
  
  Text_layer::Text_layer(const std::vector<std::shared_ptr<Text_box>> &text_boxes_,
                         const std::vector<std::shared_ptr<Shader>> &shaders_,
                         const Vector_2f &viewport_size_,
                         bool imgui_render)
  {
    this->imgui_enabled = imgui_render;
    this->text_boxes = text_boxes_;
    this->shaders = shaders_;
    this->viewport_size = viewport_size_;
    this->projection = Orthographic_camera(this->viewport_size.get_x(), this->viewport_size.get_y(), -1.0f, 1.0f);
  }
  
  Text_layer::~Text_layer()
  {
    Text_layer::on_detach();
  }
  
  void Text_layer::on_attach()
  {
    for (auto &text_box: this->text_boxes)
    {
      this->shaders[1]->build();
      this->shaders[1]->bind();
      this->shaders[1]->set_uniform("u_projection",
                                    &(this->projection.get_projection_matrix().get_matrix()[0][0]),
                                    false);
      Renderer::send_text(*text_box, *this->shaders[1]);
    }
  }
  
  void Text_layer::on_detach()
  {
    for (const std::shared_ptr<Shader> &shader: this->shaders) shader->unbind();
  }
  
  void Text_layer::on_event(Event &event)
  {
    switch (event.get_event_type())
    {
      case Wave::Event_type::None:return;
      case Wave::Event_type::On_window_resize:
      {
        auto resize_event = dynamic_cast<On_window_resize &>(event);
        this->projection = Orthographic_camera(resize_event.get_width(), resize_event.get_height(), -1.0f, 1.0f);
        this->text_boxes[0]->set_text_offset_y(this->viewport_size.get_y() - 25.0f);
        break;
      }
      default:break;
    }
  }
  
  void Text_layer::on_update([[maybe_unused]] float time_step)
  {
  }
  
  void Text_layer::on_render()
  {
    on_imgui_render();
  }
  
  void Text_layer::on_imgui_render()
  {
    if (this->imgui_enabled)
    {
      auto &io = ImGui::GetIO();
      ImFont *bold = io.Fonts->Fonts[1];
      ImFont *regular = io.FontDefault;
      if (ImGui::Begin("Scene"))
      {
        ImGui::PushFont(bold);
        ImGui::Separator();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 20.0f));
        if (ImGui::TreeNodeEx("Text box", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
        {
          ImGui::Text("Text : %s", this->text_boxes.back()->get_text_string().c_str());
          ImGui::PushFont(regular);
          if (ImGui::Begin("Entity Info"))
          {
            ImGui::Text("Text uniform color");
            Vector_2f text_scale_previous_value = this->text_boxes.back()->get_text_scale();
            Vector_2f text_box_size_previous_value = this->text_boxes.back()->get_text_box_size();
            Color text_color_previous_value = s_imgui_text_uniform_color;
            ImGui::ColorEdit3("##Text uniform color", &s_imgui_text_uniform_color[0]);
            ImGui::Text("Text box size");
            ImGui::SliderFloat2("##Text box size", &this->text_boxes.back()->get_text_box_size()[0], 0,
                                this->viewport_size.get_x(),
                                "%.3f");
            ImGui::Text("Text pixel size");
            ImGui::SliderFloat2("##Text pixel size", &this->text_boxes.back()->get_text_scale()[0], 0.0f, 10.0f,
                                "%.3f");
            if (text_scale_previous_value != this->text_boxes.back()->get_text_scale()
                || text_box_size_previous_value != this->text_boxes.back()->get_text_box_size())
            {
              this->text_boxes.back()->build();
              Renderer::send_text(*this->text_boxes.back(), *this->shaders[1]);
            }
            
            if (text_color_previous_value != s_imgui_text_uniform_color)
            {
              this->text_boxes.back()->set_text_color(s_imgui_text_uniform_color);
              Renderer::send_text(*this->text_boxes.back(), *this->shaders[1]);
            }
          }
          ImGui::End();  // Entity Info.
          ImGui::PopFont();
          ImGui::TreePop();
        }
        ImGui::PopFont();
        ImGui::PopStyleVar();  // Window padding.
      }
      ImGui::End();  // Scene.
    }
  }
}