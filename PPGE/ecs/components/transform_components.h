#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/buffer.h"
#include "rhi/shader_resource_binding.h"
#include "system/logger_system.h"

namespace PPGE
{
struct TransformComponent
{
    Math::Vector3 position = Math::Vector3::Zero;
    Math::Quaternion rotation = Math::Quaternion::Identity;
    Math::Vector3 scale = Math::Vector3::One;

    Math::Matrix GetWorldMatrix() const
    {
        Math::Matrix world = Math::Matrix::CreateScale(scale);
        world *= Math::Matrix::CreateFromQuaternion(rotation);
        world.Translation(position);
        return world;
    }
};
} // namespace PPGE
