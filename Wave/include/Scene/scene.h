//
// Created by nami on 10/05/23.
//

#pragma once

#include <Core/core.h>
#include <Core/uuid.h>
#include <Scene/editor_camera.h>
#include <Objects/object.h>
#include <Text/text.h>

#include <entt/entt.hpp>

namespace Wave
{
  
  class Entity;
  
  class Scene
  {
    public:
    Scene() = default;
    ~Scene() = default;
    
    static std::shared_ptr<Scene> copy(const std::shared_ptr<Scene> &other);
    
    Entity create_entity(const std::string &name = std::string());
    Entity create_entity_with_uuid(UUID uuid, const std::string &name = std::string());
    Entity create_entity_with_uuid(Entity entity);
    void destroy_entity(Entity entity);
    
    void on_runtime_start();
    void on_runtime_stop();
    
    void send_gpu();
    
    void on_simulation_start();
    void on_simulation_stop();
    
    void on_update_runtime(float ts);
    void on_update_simulation(float ts, std::shared_ptr<Camera> &camera);
    void on_update_editor(float ts, std::shared_ptr<Camera> &camera);
    void on_viewport_resize(uint32_t width, uint32_t height);
    
    Entity duplicate_entity(Entity entity);
    Entity get_entity(std::string_view name);
    Entity get_entity(UUID uuid);
    Entity get_primary_camera();
    std::shared_ptr<Camera> *get_camera(const std::string &name);
    std::shared_ptr<Object> *get_object(const std::string &name);
    std::shared_ptr<Text_box> *get_text(const std::string &name);
    
    bool is_running() const
    { return this->running; }
    
    bool is_paused() const
    { return this->paused; }
    
    void set_paused(bool paused_)
    { this->paused = paused_; }
    
    void step(int frames = 1);
    
    template<typename... Components>
    auto get_all_entities_with()
    {
      return registry.view<Components...>();
    }
    
    private:
    void on_physics_start();
    void on_physics_stop();
    private:
    entt::registry registry;
    std::unordered_map<UUID, entt::entity> entity_map;
    Math::Vector_2f viewport_dimensions = Math::Vector_2f(0.0f);
    bool running = false;
    bool paused = false;
    int step_frames = 0;
    
    friend class Entity;
    
    friend class SceneSerializer;
    
    friend class Scene_ui_panel;
  };
}

