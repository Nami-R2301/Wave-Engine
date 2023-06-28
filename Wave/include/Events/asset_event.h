//
// Created by nami on 28/06/23.
//

#pragma once

#include <Events/event.h>

namespace Wave::Event_system
{
  
  class On_asset_modification : public Event
  {
    explicit On_asset_modification(Event &event);
    ~On_asset_modification() override = default;
  };
}
