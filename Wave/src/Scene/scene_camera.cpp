//
// Created by nami on 10/05/23.
//

#include <Scene/scene_camera.h>

namespace Wave
{
  
  Scene_camera::Scene_camera()
  {
    this->view_matrix = Perspective_camera(this->width, this->height, this->fov, this->z_near,
                                           this->z_far).get_view_matrix();
  }
}