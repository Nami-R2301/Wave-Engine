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
    T &AddComponent(Args &&... args)
    {
      HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
      T &component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
      m_Scene->OnComponentAdded<T>(*this, component);
      return component;
    }
    
    template<typename T, typename... Args>
    T &AddOrReplaceComponent(Args &&... args)
    {
      T &component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
      m_Scene->OnComponentAdded<T>(*this, component);
      return component;
    }
    
    template<typename T>
    T &GetComponent()
    {
      if (!HasComponent<T>())
        alert(WAVE_ERROR, "[Entity] --> Entity does not have component!");
      return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }
    
    template<typename T>
    bool HasComponent()
    {
      return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
    }
    
    template<typename T>
    void RemoveComponent()
    {
      HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
      m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }
    
    explicit operator bool() const
    { return m_EntityHandle != entt::null; }
    
    explicit operator entt::entity() const
    { return m_EntityHandle; }
    
    explicit operator uint32_t() const
    { return (uint32_t) m_EntityHandle; }
    
    uint64_t GetUUID()
    { return GetComponent<IDComponent>().ID; }
    
    const std::string &GetName()
    { return GetComponent<TagComponent>().Tag; }
    
    bool operator==(const Entity &other) const
    {
      return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
    }
    
    bool operator!=(const Entity &other) const
    {
      return !(*this == other);
    }
    
    private:
    entt::entity m_EntityHandle{entt::null};
    Scene *m_Scene = nullptr;
  };
}
