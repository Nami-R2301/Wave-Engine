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

namespace Wave
{
  
  struct IDComponent : public Printable
  {
    uint64_t ID;
    
    INTERFACE_PRINT
    
    IDComponent() = default;
    IDComponent(const IDComponent &) = default;
  };
  
  struct TagComponent : public Printable
  {
    std::string Tag;
    
    TagComponent() = default;
    TagComponent(const TagComponent &) = default;
    
    INTERFACE_PRINT
    
    explicit TagComponent(const std::string &tag)
      : Tag(tag)
    {}
  };
  
  struct TransformComponent : public Printable
  {
    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};
    
    TransformComponent() = default;
    TransformComponent(const TransformComponent &) = default;
    
    INTERFACE_PRINT
    
    explicit TransformComponent(const glm::vec3 &translation)
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
  
  struct SpriteRendererComponent : public Printable
  {
    glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
    std::shared_ptr<Texture> texture;
    float TilingFactor = 1.0f;
    
    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent &) = default;
    
    INTERFACE_PRINT
    
    explicit SpriteRendererComponent(const glm::vec4 &color)
      : Color(color)
    {}
  };
  
  struct CircleRendererComponent : public Printable
  {
    glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
    float Thickness = 1.0f;
    float Fade = 0.005f;
    
    INTERFACE_PRINT
    
    CircleRendererComponent() = default;
    CircleRendererComponent(const CircleRendererComponent &) = default;
  };
  
  struct CameraComponent : public Printable
  {
    Scene_camera Camera;
    bool Primary = true; // TODO: think about moving to Scene
    bool FixedAspectRatio = false;
    
    INTERFACE_PRINT
    
    CameraComponent() = default;
    CameraComponent(const CameraComponent &) = default;
  };
  
  struct ScriptComponent : public Printable
  {
    std::string ClassName;
    
    INTERFACE_PRINT
    
    ScriptComponent() = default;
    ScriptComponent(const ScriptComponent &) = default;
  };
  
  // Forward declaration
  class ScriptableEntity;
  
  struct NativeScriptComponent : public Printable
  {
    ScriptableEntity *Instance = nullptr;
    
    ScriptableEntity *(*InstantiateScript)();
    void (*DestroyScript)(NativeScriptComponent *);
    
    INTERFACE_PRINT
    
    template<typename T>
    void Bind()
    {
      InstantiateScript = []()
      { return static_cast<ScriptableEntity *>(new T()); };
      DestroyScript = [](NativeScriptComponent *nsc)
      {
        delete nsc->Instance;
        nsc->Instance = nullptr;
      };
    }
  };
  
  // Physics
  
  struct Rigidbody2DComponent : public Printable
  {
    enum class BodyType
    {
      Static = 0, Dynamic, Kinematic
    };
    BodyType Type = BodyType::Static;
    bool FixedRotation = false;
    
    // Storage for runtime
    void *RuntimeBody = nullptr;
    
    INTERFACE_PRINT
    
    Rigidbody2DComponent() = default;
    Rigidbody2DComponent(const Rigidbody2DComponent &) = default;
  };
  
  struct BoxCollider2DComponent : public Printable
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
    
    INTERFACE_PRINT
    
    BoxCollider2DComponent() = default;
    BoxCollider2DComponent(const BoxCollider2DComponent &) = default;
  };
  
  struct CircleCollider2DComponent : public Printable
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
    
    INTERFACE_PRINT
    
    CircleCollider2DComponent() = default;
    CircleCollider2DComponent(const CircleCollider2DComponent &) = default;
  };
  
  template<typename... Component>
  struct ComponentGroup
  {
  };
  
  using AllComponents =
    ComponentGroup<TransformComponent, SpriteRendererComponent,
      CircleRendererComponent, CameraComponent, ScriptComponent,
      NativeScriptComponent, Rigidbody2DComponent, BoxCollider2DComponent,
      CircleCollider2DComponent>;
}
