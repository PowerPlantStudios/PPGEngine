#include "camera_controller.h"

#include "core/input.h"
#include "ecs/components/camera_component.h"
#include "ecs/components/transform_components.h"

namespace PPGE
{
constexpr float pi = 3.14159265359f;

void CameraController::Possess(Entity camera_entity)
{
    PPGE_ASSERT(camera_entity.IsValid(), "Camera entity is not valid");

    if (!camera_entity.HasAllOf<TransformComponent, CameraComponent>())
    {
        PPGE_ERROR("Given entity doesn't have necessary components");
        return;
    }

    m_camera_entity = camera_entity;
}

Math::Vector3 CameraController::GetPosition() const
{
    const auto &transform_component = m_camera_entity.GetComponents<TransformComponent>();

    return transform_component.position;
}

Math::Matrix CameraController::GetView() const
{
    const auto &transform_component = m_camera_entity.GetComponents<TransformComponent>();
#if !defined(PPGE_PLATFORM_APPLE)
    auto view = Math::Matrix::CreateFromQuaternion(transform_component.rotation);
    view.Translation(transform_component.position);
    return view.Invert();
#else
    auto view = Math::CreateFromQuaternion(transform_component.rotation);
    view = view * Math::translate(transform_component.position);
    return matrix_invert(view);
#endif

    
}

Math::Matrix CameraController::GetProj() const
{
    const auto &camera_component = m_camera_entity.GetComponents<CameraComponent>();
    return camera_component.projection;
}

Math::Matrix CameraController::GetViewProj() const
{
    return GetView() * GetProj();
}

void CameraController2D::Move(const Math::Vector2 &delta_pos)
{
    auto &transform_component = m_camera_entity.GetComponents<TransformComponent>();

    transform_component.position += Math::Vector3{delta_pos.x, delta_pos.y, 0.0f};
}

void CameraController2D::Rotate(float angle)
{
    auto &transform_component = m_camera_entity.GetComponents<TransformComponent>();
#if !defined(PPGE_PLATFORM_APPLE)
    auto orientation = Math::Matrix::CreateFromQuaternion(transform_component.rotation);
    auto rotator = Math::Matrix::CreateRotationZ((angle / 180.0f) * pi);
    orientation *= rotator;

    transform_component.rotation = Math::Quaternion::CreateFromRotationMatrix(orientation);
#else
    auto orientation = Math::CreateFromQuaternion(transform_component.rotation);
    auto rotator = Math::rotateZ((angle / 180.0f) * pi);
    orientation *= rotator;

    transform_component.rotation = Math::CreateFromRotationMatrix(orientation);
#endif
}

void CameraController3D::Move(const Math::Vector3 &delta_pos)
{
    auto &transform_component = m_camera_entity.GetComponents<TransformComponent>();

    transform_component.position += delta_pos;
}

void CameraController3D::LookAt(const Math::Vector3 &target, const Math::Vector3 &world_up)
{
    auto &transform_component = m_camera_entity.GetComponents<TransformComponent>();
    auto F = target - transform_component.position;
#if !defined(PPGE_PLATFORM_APPLE)
    F.Normalize();
    auto L = F.Cross(world_up);
    L.Normalize();
    auto U = L.Cross(F);

    Math::Matrix orientation;
    orientation.Forward(F);
    orientation.Right(L);
    orientation.Up(U);

    transform_component.rotation = Math::Quaternion::CreateFromRotationMatrix(orientation);
#else
    F = simd_normalize(F);
    auto L = simd_normalize(simd_cross(F, world_up));
    auto U = simd_cross(L, F);
    Math::Matrix orientation;
    Math::Forward(orientation, F);
    Math::Right(orientation, L);
    Math::Up(orientation, U);
    
    transform_component.rotation = Math::CreateFromRotationMatrix(orientation);
#endif
}

void FreeLookCamera::Update(float delta_time)
{
    float x = Input::GetMouseX();
    float delta_x = m_mouse_pos.x - x;
    m_mouse_pos.x = x;

    float y = Input::GetMouseY();
    float delta_y = y - m_mouse_pos.y;
    m_mouse_pos.y = y;

    Math::Vector3 delta_pos;
    
    {
        auto &transform_component = m_camera_entity.GetComponents<TransformComponent>();
#if !defined(PPGE_PLATFORM_APPLE)
        auto orientation = Math::Matrix::CreateFromQuaternion(transform_component.rotation);

        if (Input::IsKeyPressed(PPGE_KEY_W))
        {
            delta_pos += orientation.Forward() * m_walk_speed * delta_time;
        }
        else if (Input::IsKeyPressed(PPGE_KEY_S))
        {
            delta_pos -= orientation.Forward() * m_walk_speed * delta_time;
        }

        if (Input::IsKeyPressed(PPGE_KEY_A))
        {
            delta_pos += orientation.Left() * m_walk_speed * delta_time;
        }
        else if (Input::IsKeyPressed(PPGE_KEY_D))
        {
            delta_pos -= orientation.Left() * m_walk_speed * delta_time;
        }

        if (Input::IsKeyPressed(PPGE_KEY_E))
        {
            delta_pos += orientation.Up() * m_walk_speed * delta_time;
        }
        else if (Input::IsKeyPressed(PPGE_KEY_Q))
        {
            delta_pos -= orientation.Up() * m_walk_speed * delta_time;
        }
#else
        auto orientation = Math::CreateFromQuaternion(transform_component.rotation);

        if (Input::IsKeyPressed(PPGE_KEY_W))
        {
            delta_pos += Math::Forward(orientation) * m_walk_speed * delta_time;
        }
        else if (Input::IsKeyPressed(PPGE_KEY_S))
        {
            delta_pos -= Math::Forward(orientation) * m_walk_speed * delta_time;
        }

        if (Input::IsKeyPressed(PPGE_KEY_A))
        {
            delta_pos += Math::Left(orientation) * m_walk_speed * delta_time;
        }
        else if (Input::IsKeyPressed(PPGE_KEY_D))
        {
            delta_pos -= Math::Left(orientation) * m_walk_speed * delta_time;
        }

        if (Input::IsKeyPressed(PPGE_KEY_E))
        {
            delta_pos += Math::Up(orientation) * m_walk_speed * delta_time;
        }
        else if (Input::IsKeyPressed(PPGE_KEY_Q))
        {
            delta_pos -= Math::Up(orientation) * m_walk_speed * delta_time;
        }
#endif
    }
    Move(delta_pos);

    if (Input::IsMouseButtonPressed(PPGE_MOUSE_BUTTON_1))
    {
        Math::Vector3 look_at_dir;
        {
            auto &transform_component = m_camera_entity.GetComponents<TransformComponent>();
#if !defined(PPGE_PLATFORM_APPLE)
            auto orientation = Math::Matrix::CreateFromQuaternion(transform_component.rotation);
            Math::Matrix rotator;
            {
                rotator = Math::Matrix::CreateFromAxisAngle(
                    orientation.Left(), m_rotation_speed * ((delta_y * delta_time) / 180.0f * pi));
                rotator *= Math::Matrix::CreateFromAxisAngle(
                    orientation.Up(), m_rotation_speed * ((delta_x * delta_time) / 180.0f * pi));
            }
            orientation *= rotator;
            look_at_dir = transform_component.position + orientation.Forward();
        }
        LookAt(look_at_dir, Math::Vector3::Up);
#else
        auto orientation = Math::CreateFromQuaternion(transform_component.rotation);
        Math::Matrix rotator;
        {
            rotator = simd::quatf(m_rotation_speed * ((delta_y * delta_time) / 180.0f * pi)), Math::Left(orientation);
            rotator *= simd::quatf(m_rotation_speed * ((delta_x * delta_time) / 180.0f * pi)), Math::Up(orientation);
        }
        orientation *= rotator;
        look_at_dir = transform_component.position + Math::Forward(orientation);
    }
    LookAt(look_at_dir, simd::float3{0.f, 1.f, 0.f});
#endif
    }
}
} // namespace PPGE
