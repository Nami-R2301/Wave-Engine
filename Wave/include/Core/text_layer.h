//
// Created by nami on 24/04/23.
//

#pragma once

#include <Core/layer.h>
#include <Renderer/renderer.h>
#include <Objects/gl_text.h>
#include <Renderer/shader.h>

namespace Wave
{
  
  class Text_layer : public Layer
  {
  public:
    Text_layer(const std::vector<std::shared_ptr<Text>> &strings_,
               const std::vector<std::shared_ptr<Shader>>& text_shaders_);
    ~Text_layer() override;
    
    void on_attach() override;
    void on_detach() override;
    void on_update(float time_step) override;
    void on_event(Event &event) override;
    void on_imgui_render(float time_step) override;
  private:
    std::vector<std::shared_ptr<Text>> strings;
    std::vector<std::shared_ptr<Shader>> text_shaders;
    std::vector<std::shared_ptr<Vertex_array_buffer>> vao_list{};
    glm::mat4x4 transform = glm::mat4(1.0f);
  };
}
