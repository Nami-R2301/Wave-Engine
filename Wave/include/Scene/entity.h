//
// Created by nami on 10/05/23.
//

#pragma once

#include <Core/core.h>
#include <Scene/scene.h>
#include <Scene/components.h>


namespace Wave
{
  
  class Entity
  {
    public:
    Entity() = default;
    Entity(entt::entity handle, Scene *scene);
    Entity(const Entity &other) = default;
    
    template<typename T, typename... Args>
    T &add_component(Args &&... args)
    {
      T &component = scene->registry.emplace<T>(entity_handle, std::forward<Args>(args)...);
      scene->OnComponentAdded<T>(*this, component);
      return component;
    }
    
    template<typename T, typename... Args>
    T &add_or_replace_component(Args &&... args)
    {
      T &component = scene->registry.emplace_or_replace<T>(entity_handle, std::forward<Args>(args)...);
      scene->OnComponentAdded<T>(*this, component);
      return component;
    }
    
    template<typename T>
    T &get_component()
    {
      if (!has_component<T>())
        alert(WAVE_ERROR, "[Entity] --> Entity does not have component!");
      return scene->registry.get<T>(entity_handle);
    }
    
    template<typename T>
    bool has_component()
    {
      return scene->registry.any_of<T>(entity_handle);
    }
    
    template<typename T>
    void remove_component()
    {
      if (!has_component<T>())
        alert(WAVE_ERROR, "[Entity] --> Entity does not have component!");
      scene->registry.remove<T>(entity_handle);
    }
    
    explicit operator bool() const
    { return entity_handle != entt::null; }
    
    explicit operator entt::entity() const
    { return entity_handle; }
    
    explicit operator uint32_t() const
    { return (uint32_t) entity_handle; }
    
    uint64_t get_uuid()
    { return get_component<ID_component_s>().ID; }
    
    const std::string &get_name()
    { return get_component<Tag_component_s>().Tag; }
    
    bool operator==(const Entity &other) const
    {
      return entity_handle == other.entity_handle && scene == other.scene;
    }
    
    bool operator!=(const Entity &other) const
    {
      return !(*this == other);
    }
    
    private:
    entt::entity entity_handle{entt::null};
    Scene *scene = nullptr;
  };
}

