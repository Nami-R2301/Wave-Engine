//
// Created by nami on 10/05/23.
//

#include <Scene/entity.h>

namespace Wave
{
  
  Entity::Entity(entt::entity handle, Scene *scene)
    : entity_handle(handle), scene(scene)
  {
  }
}