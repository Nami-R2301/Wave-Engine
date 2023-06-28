//
// Created by nami on 22/06/23.
//


#include <Events/scene_events.h>

namespace Wave::Event_system
{
  
  
  On_entity_add::On_entity_add(Entity entity_added)
  {
  }
  
  std::string On_entity_add::to_string() const
  {
    return {};
  }
  
  On_entity_delete::On_entity_delete(Entity entity_deleted)
  {
  }
  
  std::string On_entity_delete::to_string() const
  {
    return {};
  }
  
  On_entity_duplicate::On_entity_duplicate([[maybe_unused]] Entity entity_duplicated)
  {
  }
  
  std::string On_entity_duplicate::to_string() const
  {
    return {};
  }
}