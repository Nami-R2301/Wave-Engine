//
// Created by nami on 10/05/23.
//

#include <Scene/scene.h>
#include <Scene/entity.h>

namespace Wave
{
  void Scene::DestroyEntity(Entity entity)
  {
    this->entity_map.erase(entity.get_uuid());
    this->registry.destroy((entt::entity) entity);
  }
}