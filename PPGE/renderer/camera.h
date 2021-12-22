#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
class Camera
{
  public:
    Camera()
    {
        m_projection = Math::Matrix::Identity;
    }

    ~Camera() = default;

    void SetPerpective(float width, float height, float near_plane, float far_plane)
    {
        m_projection = Math::Matrix::CreatePerspective(width, height, near_plane, far_plane);
    }

    void SetPerpectiveFOV(float fov, float aspect_ratio, float near_plane, float far_plane)
    {
        m_projection = Math::Matrix::CreatePerspectiveFieldOfView(fov, aspect_ratio, near_plane, far_plane);
    }

    void SetOrtogpraphic(float width, float height, float z_near, float z_far)
    {
        m_projection = Math::Matrix::CreateOrthographic(width, height, z_near, z_far);
    }

    inline Math::Matrix GetProjection() const
    {
        return m_projection;
    }

  private:
    Math::Matrix m_projection;
};
} // namespace PPGE