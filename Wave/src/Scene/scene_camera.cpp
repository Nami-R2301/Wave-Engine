//
// Created by nami on 10/05/23.
//

#include <Scene/scene_camera.h>

#include <glm/glm/gtc/matrix_transform.hpp>

namespace Wave
{
  
  Scene_camera::Scene_camera()
  {
    RecalculateProjection();
  }
  
  void Scene_camera::SetViewportSize(uint32_t width, uint32_t height)
  {
    assert(width > 0 && height > 0);
    m_AspectRatio = (float) width / (float) height;
    RecalculateProjection();
  }
  
  void Scene_camera::RecalculateProjection()
  {
    view_matrix_glm = glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear,
                                       m_PerspectiveFar);
  }
}