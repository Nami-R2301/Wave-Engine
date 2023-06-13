//
// Created by nami on 18/05/23.
//

#include <Renderer/renderer.h>
#include <ImGUI/scene_panel.h>
#include <ImGUI/imGUI_layer.h>

namespace Wave
{
  
  Scene_ui_panel::Scene_ui_panel(const std::shared_ptr<Scene> &context_, const Scene_ui_data_s &scene_ui_data_)
  {
    this->context = context_;
    this->scene_ui_data = scene_ui_data_;
  }
  
  void Scene_ui_panel::translate(const Vector_3f &position)
  {
    this->scene_ui_data.origin = (Vector_2f) position;
    this->moved = true;
  }
  
  void Scene_ui_panel::translate(float x, float y, [[maybe_unused]] float z)
  {
    this->scene_ui_data.origin = Vector_2f(x, y);
    this->moved = true;
  }
  
  void *Scene_ui_panel::copy() const
  {
    auto copy = std::make_shared<Scene_ui_panel>(*this);
    return (void *) &(*copy);
  }
  
  void Scene_ui_panel::on_ui_render()
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 10.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {this->scene_ui_data.size_boundaries.get_x(),
                                                      this->scene_ui_data.size_boundaries.get_y()});
    
    ImGuiDockNode *scene_dock_node;
    scene_dock_node = ImGui::DockBuilderGetNode(ImGui_layer::scene_panel_dock_id);
    Vector_2f current_position, current_size;
    if (scene_dock_node)
    {
      current_position = Vector_2f(scene_dock_node->Pos.x, scene_dock_node->Pos.y),
        current_size = Vector_2f(scene_dock_node->Size.x, scene_dock_node->Size.y);
      
      if (current_position != this->scene_ui_data.origin)
      {
        this->translate(Vector_3f(current_position));
        
        scene_dock_node->Pos = ImVec2(this->scene_ui_data.origin.get_x(), this->scene_ui_data.origin.get_y());
      }
      
      
      if (Vector_2f(this->get_size_boundaries().get_x(), this->get_size_boundaries().get_y()) != current_size)
      {
        this->scene_ui_data.size_boundaries = Vector_4f(this->get_size_boundaries().get_x(),
                                                        this->get_size_boundaries().get_y(),
                                                        current_size.get_x(), current_size.get_y());
        
        scene_dock_node->Size = ImVec2(this->scene_ui_data.size_boundaries.get_z(),
                                       this->scene_ui_data.size_boundaries.get_w());
      }
    }
    
    if (ImGui_layer::show_scene_panel)
    {
      if (ImGui::Begin("Scene Editor", &ImGui_layer::show_scene_panel, ImGuiWindowFlags_AlwaysUseWindowPadding))
      {
        if (this->context)
        {
          this->context->registry.each([&](auto entity_id)
                                       {
                                         Entity entity{entity_id, this->context.get()};
                                         display_entity(entity);
                                       });
        }
      }
      ImGui::End();  // Scene hierarchy
      
      auto node = ImGui::DockBuilderGetNode(ImGui_layer::scene_panel_dock_id);
      if (node)
      {
        ImGui::DockNodeBeginAmendTabBar(node);
        if (ImGui::BeginTabItem("Renderer Editor", &ImGui_layer::show_scene_panel, ImGuiTabItemFlags_Trailing))
        {
          ImGui::SetCursorPosX(ImGui::GetCursorPosX());
          ImGui::EndTabItem();
        }
        ImGui::DockNodeEndAmendTabBar();
      }
    }
    ImGui::PopStyleVar();  // Window minimum size
    ImGui::PopStyleVar();  // Window padding
  }
  
  const Vector_2f &Scene_ui_panel::get_position() const
  {
    return this->scene_ui_data.origin;
  }
  
  const Vector_4f &Scene_ui_panel::get_size_boundaries() const
  {
    return this->scene_ui_data.size_boundaries;
  }
  
  const Color &Scene_ui_panel::get_background_color() const
  {
    return this->scene_ui_data.background_color;
  }
  
  float Scene_ui_panel::get_font_scale() const
  {
    return this->scene_ui_data.font_scale;
  }
  
  void Scene_ui_panel::set_position(const Vector_2f &position_)
  {
    this->scene_ui_data.origin = position_;
  }
  
  void Scene_ui_panel::set_size_boundaries(const Vector_4f &size_boundaries_)
  {
    this->scene_ui_data.size_boundaries = size_boundaries_;
  }
  
  void Scene_ui_panel::set_background_color(const Color &back_ground_color_)
  {
    this->scene_ui_data.background_color = back_ground_color_;
  }
  
  void Scene_ui_panel::set_font_scale(float font_scale_)
  {
    this->scene_ui_data.font_scale = font_scale_;
  }
  
  void Scene_ui_panel::display_entity(Entity entity)
  {
    auto &tag = entity.get_name();
    
    ImGuiTreeNodeFlags flags_ = ImGuiTreeNodeFlags_OpenOnArrow;
    flags_ |= ImGuiTreeNodeFlags_SpanAvailWidth;
    bool opened_ = ImGui::TreeNodeEx((void *) (uint64_t) (uint32_t) entity, flags_, "%s", tag.c_str());
    
    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem())
    {
      if (ImGui::MenuItem("Delete Entity")) entityDeleted = true;
      ImGui::EndPopup();
    }
    
    if (opened_)
    {
      ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
      
      ImGui::Begin("Properties", &opened_);
      if (ImGui::TreeNodeEx("##Entity", flags, "%s", tag.c_str())) ImGui::TreePop();
      
      if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen |
                                         ImGuiTreeNodeFlags_OpenOnArrow |
                                         ImGuiTreeNodeFlags_SpanAvailWidth |
                                         ImGuiTreeNodeFlags_FramePadding))
      {
        if (entity.has_component<Transform_component_s>())
        {
          ImGui_layer::display_property("Translation",
                                        entity.get_component<Transform_component_s>().get_transform().get_translation(),
                                        Vector_3f(0.0f),
                                        Vector_3f(0.0f),
                                        Vector_3f(5.0f),
                                        Vector_3f(0.0f),
                                        120.0f);
          ImGui_layer::display_property("Rotation",
                                        entity.get_component<Transform_component_s>().get_transform().get_rotation(),
                                        Vector_3f(0.0f),
                                        Vector_3f(0.0f),
                                        Vector_3f(10.0f),
                                        Vector_3f(0.0f),
                                        120.0f);
          ImGui_layer::display_property("Scale",
                                        entity.get_component<Transform_component_s>().get_transform().get_scale(),
                                        Vector_3f(0.0f),
                                        Vector_3f(0.0f),
                                        Vector_3f(0.1f),
                                        Vector_3f(0.0f),
                                        120.0f);
        }
        ImGui::TreePop();  // Transform.
      }
      ImGui::End();  // Properties.
      ImGui::TreePop();  // Entity.
    }
    
    if (entityDeleted) this->context->destroy_entity(entity);
  }
}