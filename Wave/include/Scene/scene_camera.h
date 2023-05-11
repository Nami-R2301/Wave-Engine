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
    
    void SetViewportSize(uint32_t width, uint32_t height);
    
    [[nodiscard]] float GetPerspectiveVerticalFOV() const
    { return m_PerspectiveFOV; }
    
    void SetPerspectiveVerticalFOV(float verticalFov)
    {
      m_PerspectiveFOV = verticalFov;
      RecalculateProjection();
    }
    
    [[nodiscard]] float GetPerspectiveNearClip() const
    { return m_PerspectiveNear; }
    
    void SetPerspectiveNearClip(float nearClip)
    {
      m_PerspectiveNear = nearClip;
      RecalculateProjection();
    }
    
    [[nodiscard]] float GetPerspectiveFarClip() const
    { return m_PerspectiveFar; }
    
    void SetPerspectiveFarClip(float farClip)
    {
      m_PerspectiveFar = farClip;
      RecalculateProjection();
    }
    
    [[nodiscard]] float GetOrthographicSize() const
    { return m_OrthographicSize; }
    
    void SetOrthographicSize(float size)
    {
      m_OrthographicSize = size;
      RecalculateProjection();
    }
    
    [[nodiscard]] float GetOrthographicNearClip() const
    { return m_OrthographicNear; }
    
    void SetOrthographicNearClip(float nearClip)
    {
      m_OrthographicNear = nearClip;
      RecalculateProjection();
    }
    
    [[nodiscard]] float GetOrthographicFarClip() const
    { return m_OrthographicFar; }
    
    void SetOrthographicFarClip(float farClip)
    {
      m_OrthographicFar = farClip;
      RecalculateProjection();
    }
    
    private:
    void RecalculateProjection();
    private:
    float m_PerspectiveFOV = 45.0f;
    float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
    
    float m_OrthographicSize = 10.0f;
    float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
    
    float m_AspectRatio = 0.0f;
  };
}
