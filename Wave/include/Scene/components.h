//
// Created by nami on 10/05/23.
//

#pragma once

#include <Core/core.h>
#include <Scene/scene_camera.h>
#include <Math/transform.h>
#include <Renderer/texture.h>
#include <utility>

namespace Wave
{
  
  struct ID_component_s
  {
    UUID ID = UUID();
    
    ID_component_s() = default;
    ID_component_s(const ID_component_s &) = default;
    
    explicit ID_component_s(const UUID &uuid) : ID(uuid)
    {}
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
    Math::Transform transform_component;
    Math::Matrix_4f model_matrix = Math::Matrix_4f(1.0f);
    
    Transform_component_s() = default;
    Transform_component_s(const Transform_component_s &) = default;
    
    explicit Transform_component_s(const Math::Transform &transform_) : transform_component(transform_)
    {}
    
    explicit Transform_component_s(const Math::Vector_3f &translation, const Math::Vector_3f &rotation,
                                   const Math::Vector_3f &scale)
    {
      this->transform_component.set_translation(translation);
      this->transform_component.set_rotation(rotation);
      this->transform_component.set_scale(scale);
    }
    
    [[nodiscard]] const Math::Transform &get_transform() const
    {
      return this->transform_component;
    }
    
    [[nodiscard]] Math::Transform &get_transform()
    {
      return this->transform_component;
    }
    
    [[nodiscard]] Math::Matrix_4f get_transform_matrix() const
    {
      return this->transform_component.get_transform_matrix();
    }
  };
  
  struct Sprite_component_s
  {
    Math::Vector_4f Color{1.0f, 1.0f, 1.0f, 1.0f};
    std::shared_ptr<Texture> texture;
    float TilingFactor = 1.0f;
    
    Sprite_component_s() = default;
    Sprite_component_s(const Sprite_component_s &) = default;
    
    explicit Sprite_component_s(const Math::Vector_4f &color) : Color(color)
    {}
  };
  
  struct Circle_component_s
  {
    Math::Vector_4f Color{1.0f, 1.0f, 1.0f, 1.0f};
    float Thickness = 1.0f;
    float Fade = 0.005f;
    
    Circle_component_s() = default;
    Circle_component_s(const Circle_component_s &) = default;
  };
  
  struct Camera_component_s
  {
    Scene_camera Camera;
    bool Primary = true;
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
    Math::Vector_2f Offset = {0.0f, 0.0f};
    Math::Vector_2f Size = {0.5f, 0.5f};
    
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
    Math::Vector_2f Offset = {0.0f, 0.0f};
    float Radius = 0.5f;
    
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
  
  using All_components =
    Component_group_s<Transform_component_s, Sprite_component_s,
      Circle_component_s, Camera_component_s, Script_component_s,
      Rigid_body_2D_component_s, Box_collider_2D_component_s,
      Circle_collider_2D_component_s>;
}
