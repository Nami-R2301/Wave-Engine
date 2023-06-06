//
// Created by nami on 24/04/23.
//

#include <imgui_internal.h>
#include <Layer/text_layer.h>

namespace Wave
{
  static Vector_2f s_pixel_size_previous_value, s_initial_text_box_offset, s_text_box_offset;
  static Color s_text_color_previous_value;
  
  Text_layer::Text_layer(const std::vector<std::shared_ptr<Text_box>> &text_boxes_,
                         const std::vector<std::shared_ptr<Shader>> &shaders_,
                         const Vector_2f *viewport_size_,
                         bool imgui_render)
  {
    this->imgui_enabled = imgui_render;
    this->text_boxes = text_boxes_;
    this->shaders = shaders_;
    this->viewport_size = viewport_size_;
    this->projection = Orthographic_camera(this->viewport_size->get_x(),
                                           this->viewport_size->get_y(), -1.0f, 1.0f);
  }
  
  Text_layer::~Text_layer()
  {
    Text_layer::on_detach();
  }
  
  void Text_layer::on_attach()
  {
    for (auto &text_box: this->text_boxes)
    {
      this->shaders[1]->send_gpu();
      this->shaders[1]->bind();
      this->shaders[1]->set_uniform("u_projection",
                                    &(this->projection.get_projection_matrix().get_matrix()[0][0]),
                                    false);
      s_text_color_previous_value = text_box->get_text_uniform_color();
      Renderer::send_text(*text_box, *this->shaders[1]);
    }
  }
  
  void Text_layer::on_detach()
  {
    for (const std::shared_ptr<Shader> &shader: this->shaders) shader->unbind();
  }
  
  void Text_layer::on_event([[maybe_unused]] Event &event)
  {
  }
  
  void Text_layer::on_update([[maybe_unused]] float time_step)
  {
  }
  
  void Text_layer::on_render()
  {
    this->shaders[1]->bind();
    this->shaders[1]->set_uniform("u_projection",
                                  &(this->projection.get_projection_matrix().get_matrix()[0][0]),
                                  false);
  }
  
  void Text_layer::on_ui_render(float time_step)
  {
    if (this->imgui_enabled)
    {
      auto &io = ImGui::GetIO();
      io.DeltaTime = time_step;
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
            
            s_text_box_offset = this->text_boxes.back()->get_text_offset();
            s_pixel_size_previous_value = this->text_boxes.back()->get_pixel_size();
            
            ImGui::ColorEdit4("##Text uniform color", &s_text_color_previous_value[0]);
            ImGui::Text("Text box size");
            ImGui::SliderFloat2("##Text box size", &this->text_boxes.back()->get_text_box_size()[0], 0,
                                this->viewport_size->get_x(),
                                "%.2f");
            ImGui::Text("Text pixel size");
            ImGui::SliderFloat("##Text pixel size", &s_pixel_size_previous_value[1], 0.0f, 500.0f,
                               "%.2f");
            ImGui::Text("Text box position");
            ImGui::SliderFloat("X", &s_text_box_offset[0],
                               0.0f,
                               (this->viewport_size->get_x() - this->text_boxes.back()->get_text_length()),
                               "%.2f");
            ImGui::SliderFloat("Y", &s_text_box_offset[1],
                               0.0f,
                               (this->viewport_size->get_y() - this->text_boxes.back()->get_pixel_size().get_y()),
                               "%.2f");
            ImGui::Text("Text length --> %.2f", this->text_boxes.back()->get_text_length());
            
            this->text_boxes.back()->set_text_offset(s_text_box_offset);
            this->text_boxes.back()->set_pixel_size(s_pixel_size_previous_value);
            this->text_boxes.back()->set_text_uniform_color(s_text_color_previous_value);
            
            if (s_text_box_offset != this->text_boxes.back()->get_text_offset() ||
                s_text_color_previous_value != this->text_boxes.back()->get_text_uniform_color() ||
                s_pixel_size_previous_value != this->text_boxes.back()->get_pixel_size())
            {
              this->text_boxes.back()->set_pixel_size(s_pixel_size_previous_value);
              this->text_boxes.back()->set_text_offset(s_text_box_offset);
              Renderer::send_text(*this->text_boxes.back(), *this->shaders[1], 0);
            }
            
            s_text_color_previous_value = this->text_boxes.back()->get_text_uniform_color();
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