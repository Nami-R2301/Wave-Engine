//
// Created by nami on 24/04/23.
//

#include <Layer/text_layer.h>
#include <ImGUI/imGUI_layer.h>

namespace Wave
{
  static Vector_2f s_pixel_size_previous_value, s_text_box_offset;
  static Color s_text_color_previous_value;
  static bool s_info_visible = true;
  
  Text_layer::Text_layer(const std::vector<Entity> &entities_,
                         const Vector_2f *viewport_size_,
                         bool imgui_render) : entities(entities_),
                                              viewport_size(viewport_size_), imgui_enabled(imgui_render)
  {
    this->projection = Orthographic_camera(this->viewport_size->get_x(),
                                           this->viewport_size->get_y(), -1.0f, 1.0f);
    
    for (auto &entity: this->entities)
    {
      if (entity.has_component<std::shared_ptr<Text_box>>())
        this->text_boxes.emplace_back(entity.get_component<std::shared_ptr<Text_box>>());
    }
  }
  
  Text_layer::~Text_layer()
  {
    Text_layer::on_detach();
  }
  
  void Text_layer::on_attach()
  {
    // Setup text boxes' properties.
    for (const auto &text_box: this->text_boxes)
    {
      text_box->get_shader()->bind();
      text_box->get_shader()->set_uniform("u_projection",
                                          &(this->projection.get_projection_matrix().get_matrix()[0][0]),
                                          false);
      text_box->set_text_offset_y(this->viewport_size->get_y() - text_box->get_pixel_size().get_y());
      s_text_color_previous_value = text_box->get_text_uniform_color();
    }
  }
  
  void Text_layer::on_detach()
  {
    for (const auto &text_box: this->text_boxes) text_box->get_shader()->unbind();
  }
  
  void Text_layer::on_event([[maybe_unused]] Event &event)
  {
  }
  
  void Text_layer::on_update([[maybe_unused]] float time_step)
  {
  }
  
  void Text_layer::on_render()
  {
  }
  
  void Text_layer::on_ui_render(float time_step)
  {
    if (this->imgui_enabled)
    {
      auto &io = ImGui::GetIO();
      io.DeltaTime = time_step;
      if (ImGui_layer::show_scene_panel)
      {
        if (ImGui::Begin("Scene Editor", &ImGui_layer::show_scene_panel))
        {
          ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
          if (ImGui::TreeNodeEx("Text box", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_OpenOnArrow |
                                            ImGuiTreeNodeFlags_SpanAvailWidth |
                                            ImGuiTreeNodeFlags_FramePadding))
          {
            if (ImGui::Begin("Properties", &s_info_visible))
            {
              s_text_box_offset = this->text_boxes.back()->get_text_offset();
              s_pixel_size_previous_value = this->text_boxes.back()->get_pixel_size();
              
              ImGui::Text("Text : %s", this->text_boxes.back()->get_text_string().c_str());
              ImGui::Text("Uniform color");
              ImGui::SameLine();
              ImGui::ColorEdit4("##Uniform color", &s_text_color_previous_value[0]);
              
              ImGui::Text("Text pixel size");
              ImGui::SameLine();
              ImGui::DragFloat("##Text pixel size", &s_pixel_size_previous_value[1], 1.0f,
                               1.0f, 500.0f, "%.0f");
              
              if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen |
                                                 ImGuiTreeNodeFlags_OpenOnArrow |
                                                 ImGuiTreeNodeFlags_SpanFullWidth |
                                                 ImGuiTreeNodeFlags_FramePadding))
              {
                ImGui_layer::draw_property("Translation", s_text_box_offset, Vector_2f(0.0f),
                                           Vector_2f(this->viewport_size->get_x() -
                                                     this->text_boxes.back()->get_text_length(),
                                                     this->viewport_size->get_y() -
                                                     this->text_boxes.back()->get_pixel_size().get_y()),
                                           Vector_2f(5.0f),
                                           Vector_2f(0.0f), 120.0f);
                ImGui_layer::draw_property("Rotation", (Vector_2f & )
                this->text_boxes.back()->get_text_transform().get_rotation(),
                  Vector_2f(0.0f),
                  Vector_2f(8000.0f),
                  Vector_2f(5.0f, 5.0f),
                  Vector_2f(0.0f), 120.0f);
                ImGui_layer::draw_property("Scale", this->text_boxes.back()->get_text_scale(),
                                           Vector_2f(1.0f),
                                           Vector_2f(500.0f),
                                           Vector_2f(0.1f),
                                           Vector_2f(0.0f), 120.0f);
                ImGui::TreePop();  // Transform.
              }
              
              this->text_boxes.back()->set_text_uniform_color(s_text_color_previous_value);
              
              if (s_text_box_offset != this->text_boxes.back()->get_text_offset() ||
                  s_text_color_previous_value != this->text_boxes.back()->get_text_uniform_color() ||
                  s_pixel_size_previous_value != this->text_boxes.back()->get_pixel_size())
              {
                this->text_boxes.back()->set_pixel_size(s_pixel_size_previous_value);
                this->text_boxes.back()->set_text_offset(s_text_box_offset);
                this->text_boxes.back()->send_gpu(1);
              }
              
              s_text_color_previous_value = this->text_boxes.back()->get_text_uniform_color();
            }
            ImGui::End();  // Properties.
            ImGui::TreePop();  // Tex box.
          }
          ImGui::PopStyleVar();  // Window padding.
        }
        ImGui::End();  // Scene.
      }
    }
  }
}