#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "ecs/entity.h"

namespace PPGE
{
class CameraController
{
  public:
    void Possess(Entity camera_entity);

    Math::Vector3 GetPosition() const;

    Math::Matrix GetView() const;

    Math::Matrix GetProj() const;

    Math::Matrix GetViewProj() const;

    virtual void Update(float delta_time) = 0;

  protected:
    Entity m_camera_entity;
};

class CameraController2D : public CameraController
{
  public:
    void Move(const Math::Vector2 &delta_pos);

    void Rotate(float angle);
};

class CameraController3D : public CameraController
{
  public:
    void Move(const Math::Vector3 &delta_pos);

    void LookAt(const Math::Vector3 &target, const Math::Vector3 &world_up);
};

class FreeLookCamera : public CameraController3D
{
  public:
    void Update(float delta_time) override final;

  private:
    Math::Vector2 m_mouse_pos;

    float m_walk_speed = 1.5f;
    
    float m_rotation_speed = 3.0f;
};
} // namespace PPGE
