//
// Created by nami on 10/05/23.
//

#pragma once

#include <Core/core.h>
#include <Scene/scene_camera.h>
#include <Renderer/texture.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm/gtx/quaternion.hpp>
#include <utility>

namespace Wave
{
  
  struct ID_component_s
  {
    uint64_t ID = 0;
    
    ID_component_s() = default;
    ID_component_s(const ID_component_s &) = default;
  };
  
  struct Tag_component_s
  {
    std::string Tag;
    
    Tag_component_s() = default;
    Tag_component_s(const Tag_component_s &) = default;
    
    
    explicit Tag_component_s(std::string tag)
      : Tag(std::move(tag))
    {}
  };
  
  struct Transform_component_s
  {
    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};
    
    Transform_component_s() = default;
    Transform_component_s(const Transform_component_s &) = default;
    
    explicit Transform_component_s(const glm::vec3 &translation)
      : Translation(translation)
    {}
    
    [[nodiscard]] glm::mat4 GetTransform() const
    {
      glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
      
      return glm::translate(glm::mat4(1.0f), Translation)
             * rotation
             * glm::scale(glm::mat4(1.0f), Scale);
    }
  };
  
  struct Sprite_component_s
  {
    glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
    std::shared_ptr<Texture> texture;
    float TilingFactor = 1.0f;
    
    Sprite_component_s() = default;
    Sprite_component_s(const Sprite_component_s &) = default;
    
    explicit Sprite_component_s(const glm::vec4 &color)
      : Color(color)
    {}
  };
  
  struct Circle_component_s
  {
    glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
    float Thickness = 1.0f;
    float Fade = 0.005f;
    
    Circle_component_s() = default;
    Circle_component_s(const Circle_component_s &) = default;
  };
  
  struct Camera_component_s
  {
    Scene_camera Camera;
    bool Primary = true; // TODO: think about moving to Scene
    bool FixedAspectRatio = false;
    
    Camera_component_s() = default;
    Camera_component_s(const Camera_component_s &) = default;
  };
  
  // Forward declaration
  class Scriptable_entity;
  
  struct Script_component_s
  {
    std::string ClassName;
    
    Script_component_s() = default;
    Script_component_s(const Script_component_s &) = default;
    
    Scriptable_entity *Instance = nullptr;
    
    Scriptable_entity *(*InstantiateScript)(){};
    void (*DestroyScript)(Script_component_s *){};
    
    template<typename T>
    void Bind()
    {
      InstantiateScript = []()
      { return static_cast<Scriptable_entity *>(new T()); };
      DestroyScript = [](Script_component_s *nsc)
      {
        delete nsc->Instance;
        nsc->Instance = nullptr;
      };
    }
  };
  
  // Physics
  
  struct Rigid_body_2D_component_s
  {
    enum class BodyType
    {
      Static = 0, Dynamic, Kinematic
    };
    BodyType Type = BodyType::Static;
    bool FixedRotation = false;
    
    // Storage for runtime
    void *RuntimeBody = nullptr;
    
    Rigid_body_2D_component_s() = default;
    Rigid_body_2D_component_s(const Rigid_body_2D_component_s &) = default;
  };
  
  struct Box_collider_2D_component_s
  {
    glm::vec2 Offset = {0.0f, 0.0f};
    glm::vec2 Size = {0.5f, 0.5f};
    
    // TODO(Yan): move into physics material in the future maybe
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;
    float RestitutionThreshold = 0.5f;
    
    // Storage for runtime
    void *RuntimeFixture = nullptr;
    
    Box_collider_2D_component_s() = default;
    Box_collider_2D_component_s(const Box_collider_2D_component_s &) = default;
  };
  
  struct Circle_collider_2D_component_s
  {
    glm::vec2 Offset = {0.0f, 0.0f};
    float Radius = 0.5f;
    
    // TODO(Yan): move into physics material in the future maybe
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;
    float RestitutionThreshold = 0.5f;
    
    // Storage for runtime
    void *RuntimeFixture = nullptr;
    
    Circle_collider_2D_component_s() = default;
    Circle_collider_2D_component_s(const Circle_collider_2D_component_s &) = default;
  };
  
  template<typename... Component>
  struct Component_group_s
  {
  };
  
  using all_components =
    Component_group_s<Transform_component_s, Sprite_component_s,
      Circle_component_s, Camera_component_s, Script_component_s,
      Rigid_body_2D_component_s, Box_collider_2D_component_s,
      Circle_collider_2D_component_s>;
}
