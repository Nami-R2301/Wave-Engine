//
// Created by nami on 10/05/23.
//

#pragma once

#include <Core/core.h>
#include <Scene/editor_camera.h>

#include <entt/entt.hpp>

namespace Wave
{
  
  class Entity;
  
  class Scene
  {
    public:
    Scene();
    ~Scene();
    
    static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> other);
    
    Entity CreateEntity(const std::string &name = std::string());
    Entity CreateEntityWithUUID(uint64_t uuid, const std::string &name = std::string());
    void DestroyEntity(Entity entity);
    
    void OnRuntimeStart();
    void OnRuntimeStop();
    
    void OnSimulationStart();
    void OnSimulationStop();
    
    void OnUpdateRuntime(float ts);
    void OnUpdateSimulation(float ts, Editor_camera &camera);
    void OnUpdateEditor(float ts, Editor_camera &camera);
    void OnViewportResize(uint32_t width, uint32_t height);
    
    Entity DuplicateEntity(Entity entity);
    
    Entity FindEntityByName(std::string_view name);
    Entity GetEntityByUUID(uint64_t uuid);
    
    Entity GetPrimaryCameraEntity();
    
    bool IsRunning() const
    { return is_running; }
    
    bool IsPaused() const
    { return is_paused; }
    
    void SetPaused(bool paused)
    { is_paused = paused; }
    
    void Step(int frames = 1);
    
    template<typename... Components>
    auto GetAllEntitiesWith()
    {
      return registry.view<Components...>();
    }
    
    private:
    template<typename T>
    void OnComponentAdded(Entity entity, T &component);
    
    void OnPhysics2DStart();
    void OnPhysics2DStop();
    
    void RenderScene(Editor_camera &camera);
    private:
    entt::registry registry;
    std::unordered_map<uint64_t, entt::entity> entity_map;
    Vector_2f viewport_dimensions = Vector_2f(0.0f);
    bool is_running = false;
    bool is_paused = false;
    int step_frames = 0;
    
    friend class Entity;
    
    friend class SceneSerializer;
    
    friend class Scene_ui_panel;
  };
}

