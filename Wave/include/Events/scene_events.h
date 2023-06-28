//
// Created by nami on 22/06/23.
//

#pragma once

#include <Events/event.h>
#include <Scene/entity.h>

namespace Wave::Event_system
{
  
  class On_scene_load : public Event
  {
    public:
    On_scene_load() = default;
    ~On_scene_load() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_scene_load)
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINTABLE;
  };
  
  class On_entity_add : public Event
  {
    public:
    explicit On_entity_add(Entity entity_added);
    ~On_entity_add() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_entity_add)
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINTABLE;
  };
  
  class On_entity_delete : public Event
  {
    public:
    explicit On_entity_delete(Entity entity_deleted);
    ~On_entity_delete() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_entity_delete)
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINTABLE;
  };
  
  class On_entity_duplicate : public Event
  {
    public:
    explicit On_entity_duplicate(Entity entity_duplicated);
    ~On_entity_duplicate() override = default;
    
    EVENT_CLASS_TYPE(Event_type::On_entity_duplicate)
    
    EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
    
    INTERFACE_PRINTABLE;
  };
}
