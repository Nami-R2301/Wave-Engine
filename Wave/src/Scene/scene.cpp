//
// Created by nami on 10/05/23.
//

#include <Renderer/renderer.h>
#include <Scene/scene.h>
#include <Scene/entity.h>
#include <Events/scene_events.h>

namespace Wave
{
  
  template<typename... Component>
  static void copy_component(entt::registry &dst, entt::registry &src, const std::unordered_map<UUID,
    entt::entity> &entity_map_)
  {
    ([&]()
    {
      auto view = src.view<Component>();
      for (auto srcEntity: view)
      {
        entt::entity dstEntity = entity_map_.at(src.get<ID_component_s>(srcEntity).ID);
        
        auto &srcComponent = src.get<Component>(srcEntity);
        dst.emplace_or_replace<Component>(dstEntity, srcComponent);
      }
    }(), ...);
  }
  
  template<typename... Component>
  static void copy_component(Component_group_s<Component...>, entt::registry &dst, entt::registry &src,
                             const std::unordered_map<UUID, entt::entity> &entity_map_)
  {
    copy_component<Component...>(dst, src, entity_map_);
  }
  
  template<typename... Component>
  static void copy_component_if_exists(Entity dst, Entity src)
  {
    ([&]()
    {
      if (src.has_component<Component>())
        dst.add_or_replace_component<Component>(src.get_component<Component>());
    }(), ...);
  }
  
  template<typename... Component>
  static void copy_component_if_exists(Component_group_s<Component...>, Entity dst, Entity src)
  {
    copy_component_if_exists<Component...>(dst, src);
  }
  
  std::shared_ptr<Scene> Scene::copy(const std::shared_ptr<Scene> &other)
  {
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
    
    newScene->viewport_dimensions.set_x(other->viewport_dimensions.get_x());
    newScene->viewport_dimensions.set_y(other->viewport_dimensions.get_y());
    
    auto &srcSceneRegistry = other->registry;
    auto &dstSceneRegistry = newScene->registry;
    std::unordered_map<UUID, entt::entity> entity_map;
    
    // Create entities in new scene
    const auto &idView = srcSceneRegistry.view<ID_component_s>();
    for (const auto &e: idView)
    {
      UUID uuid = UUID(srcSceneRegistry.get<ID_component_s>(e).ID);
      const auto &name = srcSceneRegistry.get<Tag_component_s>(e).Tag;
      Entity newEntity = newScene->create_entity_with_uuid(uuid, name);
      entity_map[uuid] = (entt::entity) newEntity;
    }
    
    // Copy components (except IDComponent and TagComponent)
    copy_component(All_components{}, dstSceneRegistry, srcSceneRegistry, entity_map);
    
    return newScene;
  }
  
  Entity Scene::create_entity(const std::string &name)
  {
    return create_entity_with_uuid(UUID(), name);
  }
  
  Entity Scene::create_entity_with_uuid(UUID uuid, const std::string &name)
  {
    Entity entity = {this->registry.create(), this};
    entity.add_component<ID_component_s>(uuid);
    entity.add_component<Transform_component_s>();
    auto &tag = entity.add_component<Tag_component_s>();
    tag.Tag = name.empty() ? "Entity" : name;
    
    this->entity_map[uuid] = (entt::entity) entity;
    
    return entity;
  }
  
  Entity Scene::create_entity_with_uuid(Entity entity)
  {
    this->entity_map[entity.get_uuid()] = (entt::entity) entity;
    
    return entity;
  }
  
  void Scene::destroy_entity(Entity entity)
  {
    this->entity_map.erase(entity.get_uuid());
    this->registry.destroy((entt::entity) entity);
    
    On_entity_delete on_entity_destroyed(entity);
  }
  
  void Scene::on_runtime_start()
  {
    this->running = true;

//    on_physics_start();
    
    // Scripting
    {
      // Instantiate all script entities

//      auto view = this->registry.view<Script_component_s>();
//      for (auto e: view)
//      {
//        Entity entity = {e, this};
////        ScriptEngine::OnCreateEntity(entity);
//      }
    }
  }
  
  void Scene::on_runtime_stop()
  {
    this->running = false;

//    on_physics_stop();

//    ScriptEngine::OnRuntimeStop();
  }
  
  void Scene::on_simulation_start()
  {
//    on_physics_start();
  }
  
  void Scene::on_simulation_stop()
  {
//    on_physics_stop();
  }

//
//  void Scene::OnUpdateRuntime(Timestep ts)
//  {
//    if (!m_IsPaused || m_StepFrames-- > 0)
//    {
//      // Update scripts
//      {
//        // C# Entity OnUpdate
//        auto view = m_Registry.view<ScriptComponent>();
//        for (auto e: view)
//        {
//          Entity entity = {e, this};
//          ScriptEngine::OnUpdateEntity(entity, ts);
//        }
//
//        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto &nsc)
//                                                      {
//                                                        // TODO: Move to Scene::OnScenePlay
//                                                        if (!nsc.Instance)
//                                                        {
//                                                          nsc.Instance = nsc.InstantiateScript();
//                                                          nsc.Instance->m_Entity = Entity{entity, this};
//                                                          nsc.Instance->OnCreate();
//                                                        }
//
//                                                        nsc.Instance->OnUpdate(ts);
//                                                      });
//      }
//
//      // Physics
//      {
//        const int32_t velocityIterations = 6;
//        const int32_t positionIterations = 2;
//        m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
//
//        // Retrieve transform from Box2D
//        auto view = m_Registry.view<Rigidbody2DComponent>();
//        for (auto e: view)
//        {
//          Entity entity = {e, this};
//          auto &transform = entity.GetComponent<TransformComponent>();
//          auto &rb2d = entity.GetComponent<Rigidbody2DComponent>();
//
//          b2Body *body = (b2Body *) rb2d.RuntimeBody;
//
//          const auto &position = body->GetPosition();
//          transform.Translation.x = position.x;
//          transform.Translation.y = position.y;
//          transform.Rotation.z = body->GetAngle();
//        }
//      }
//    }
//
//    // Render 2D
//    Camera *mainCamera = nullptr;
//    glm::mat4 cameraTransform;
//    {
//      auto view = m_Registry.view<TransformComponent, CameraComponent>();
//      for (auto entity: view)
//      {
//        auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
//
//        if (camera.Primary)
//        {
//          mainCamera = &camera.Camera;
//          cameraTransform = transform.GetTransform();
//          break;
//        }
//      }
//    }
//
//    if (mainCamera)
//    {
//      Renderer2D::BeginScene(*mainCamera, cameraTransform);
//
//      // Draw sprites
//      {
//        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
//        for (auto entity: group)
//        {
//          auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
//
//          Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int) entity);
//        }
//      }
//
//      // Draw circles
//      {
//        auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
//        for (auto entity: view)
//        {
//          auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
//
//          Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int) entity);
//        }
//      }
//
//      Renderer2D::EndScene();
//    }
//  }
//
//  void Scene::OnUpdateSimulation(Timestep ts, EditorCamera &camera)
//  {
//    if (!m_IsPaused || m_StepFrames-- > 0)
//    {
//      // Physics
//      {
//        const int32_t velocityIterations = 6;
//        const int32_t positionIterations = 2;
//        m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
//
//        // Retrieve transform from Box2D
//        auto view = m_Registry.view<Rigidbody2DComponent>();
//        for (auto e: view)
//        {
//          Entity entity = {e, this};
//          auto &transform = entity.GetComponent<TransformComponent>();
//          auto &rb2d = entity.GetComponent<Rigidbody2DComponent>();
//
//          b2Body *body = (b2Body *) rb2d.RuntimeBody;
//          const auto &position = body->GetPosition();
//          transform.Translation.x = position.x;
//          transform.Translation.y = position.y;
//          transform.Rotation.z = body->GetAngle();
//        }
//      }
//    }
//
//    // Render
//    RenderScene(camera);
//  }
//
  void Scene::on_update_editor(float ts, std::shared_ptr<Camera> &camera)
  {
    // Prepare gpu buffers.
    send_gpu();
  }
  
  void Scene::on_viewport_resize(uint32_t width, uint32_t height)
  {
    if (this->viewport_dimensions.get_x() == (float) width && this->viewport_dimensions.get_y() == (float) height)
      return;
    
    this->viewport_dimensions.set_x((float) width);
    this->viewport_dimensions.set_y((float) height);
    
    // Resize our non-FixedAspectRatio cameras
    auto view = this->registry.view<Camera_component_s>();
    for (auto entity: view)
    {
      auto &cameraComponent = view.get<Camera_component_s>(entity);
      if (!cameraComponent.FixedAspectRatio)
        cameraComponent.Camera.set_viewport((float) width, (float) height);
    }
  }
  
  Entity Scene::duplicate_entity(Wave::Entity entity)
  {
    // Copy name because we're going to modify component data structure
    std::string name = entity.get_name();
    Entity newEntity = create_entity(name);
    copy_component_if_exists(All_components{}, newEntity, entity);
    return newEntity;
  }
  
  Entity Scene::get_entity(std::string_view name)
  {
    auto view = this->registry.view<Tag_component_s>();
    for (auto entity: view)
    {
      const Tag_component_s &tc = view.get<Tag_component_s>(entity);
      if (tc.Tag == name)
        return Entity{entity, this};
    }
    return {};
  }
  
  Entity Scene::get_entity(UUID uuid)
  {
    if (this->entity_map.find(uuid) != this->entity_map.end())
      return {this->entity_map.at(uuid), this};
    
    return {};
  }

//
//  void Scene::OnPhysics2DStart()
//  {
//    m_PhysicsWorld = new b2World({0.0f, -9.8f});
//
//    auto view = m_Registry.view<Rigidbody2DComponent>();
//    for (auto e: view)
//    {
//      Entity entity = {e, this};
//      auto &transform = entity.GetComponent<TransformComponent>();
//      auto &rb2d = entity.GetComponent<Rigidbody2DComponent>();
//
//      b2BodyDef bodyDef;
//      bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
//      bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
//      bodyDef.angle = transform.Rotation.z;
//
//      b2Body *body = m_PhysicsWorld->CreateBody(&bodyDef);
//      body->SetFixedRotation(rb2d.FixedRotation);
//      rb2d.RuntimeBody = body;
//
//      if (entity.HasComponent<BoxCollider2DComponent>())
//      {
//        auto &bc2d = entity.GetComponent<BoxCollider2DComponent>();
//
//        b2PolygonShape boxShape;
//        boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);
//
//        b2FixtureDef fixtureDef;
//        fixtureDef.shape = &boxShape;
//        fixtureDef.density = bc2d.Density;
//        fixtureDef.friction = bc2d.Friction;
//        fixtureDef.restitution = bc2d.Restitution;
//        fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
//        body->CreateFixture(&fixtureDef);
//      }
//
//      if (entity.HasComponent<CircleCollider2DComponent>())
//      {
//        auto &cc2d = entity.GetComponent<CircleCollider2DComponent>();
//
//        b2CircleShape circleShape;
//        circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
//        circleShape.m_radius = transform.Scale.x * cc2d.Radius;
//
//        b2FixtureDef fixtureDef;
//        fixtureDef.shape = &circleShape;
//        fixtureDef.density = cc2d.Density;
//        fixtureDef.friction = cc2d.Friction;
//        fixtureDef.restitution = cc2d.Restitution;
//        fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
//        body->CreateFixture(&fixtureDef);
//      }
//    }
//  }
//
//  void Scene::OnPhysics2DStop()
//  {
//    delete m_PhysicsWorld;
//    m_PhysicsWorld = nullptr;
//  }
//
  void Scene::send_gpu()
  {
    // Draw Objects.
    {
      auto view = this->registry.view<std::shared_ptr<Object>>();
      for (auto entity: view)
      {
        if (!this->registry.any_of<std::shared_ptr<Object>>(entity)) continue;
        
        auto object = view.get<std::shared_ptr<Object>>(entity);
        object->send_gpu(1);
      }
    }
    
    // Draw text.
    {
      auto view = this->registry.view<std::shared_ptr<Text_box>>();
      for (auto entity: view)
      {
        if (!this->registry.any_of<std::shared_ptr<Text_box>>(entity)) continue;
        
        auto text = view.get<std::shared_ptr<Text_box>>(entity);
        text->send_gpu(1);
      }
    }
  }
}