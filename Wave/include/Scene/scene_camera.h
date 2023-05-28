//
// Created by nami on 10/05/23.
//

#pragma once

#include <Scene/camera.h>

namespace Wave
{
  
  class Scene_camera : public Perspective_camera
  {
    public:
    Scene_camera();
    ~Scene_camera() override = default;
  };
}
