//
// Created by nami on 24/04/23.
//

#include <imgui_internal.h>
#include <Core/text_layer.h>

namespace Wave
{
  
  static Orthographic_camera custom_projection;
  
  Text_layer::Text_layer(const std::vector<std::shared_ptr<Text>> &strings_,
                         const std::vector<std::shared_ptr<Shader>> &shaders_,
                         const Vector_2f &viewport_size_,
                         bool imgui_render)
  {
    this->imgui_enabled = imgui_render;
    this->strings = strings_;
    this->shaders = shaders_;
    this->viewport_size = viewport_size_;
    custom_projection = Orthographic_camera(this->viewport_size.get_x(), this->viewport_size.get_y(), 0.1f, 1000.0f);
    this->projection = glm::ortho(0.0f, this->viewport_size.get_x(), 0.0f, this->viewport_size.get_y());
  }
  
  Text_layer::~Text_layer()
  {
    Text_layer::on_detach();
  }
  
  void Text_layer::on_attach()
  {
    this->shaders[1]->bind();
    this->shaders[1]->set_uniform("u_sampler", 0);
    this->shaders[1]->set_uniform("u_projection", this->projection);
    this->shaders[1]->set_uniform("u_text_color", this->strings[0]->get_format().color);
    Renderer::draw_text(this->strings[0], this->shaders[1]);
    this->shaders[1]->unbind();
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
        this->projection = glm::ortho(0.0f, resize_event.get_width(), 0.0f, resize_event.get_height());
        this->strings[0]->set_offset_y(this->viewport_size.get_y() - 25.0f);
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
        if (ImGui::TreeNodeEx("Text", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
        {
          ImGui::Text("String : %s", this->strings.back()->get_string().c_str());
          if (ImGui::TreeNodeEx("Projection", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
          {
            ImGui::PushFont(regular);
            if (ImGui::Begin("Entity Info"))
            {
              ImGui::Text("Projection matrix :\n%s", custom_projection.get_projection_matrix().to_string().c_str());
            }
            ImGui::End();  // Entity Info.
            ImGui::PopFont();
            ImGui::TreePop();
          }
          ImGui::TreePop();  // Text.
        }
        ImGui::PopFont();
        ImGui::PopStyleVar();  // Window padding.
      }
      ImGui::End();  // Scene.
    }
  }
}