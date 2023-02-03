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
#if !defined(PPGE_PLATFORM_APPLE)
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
#else
    Math::Vector3 position{0.f, 0.f,0.f};
    Math::Quaternion rotation = simd::quatf{0.0f, .0f, .0f, 1.f};
    Math::Vector3 scale{1.f,1.f,1.f};

    Math::Matrix GetWorldMatrix() const
    {
        Math::Matrix world = Math::CreateScale(scale) * Math::CreateFromQuaternion(rotation) * Math::translate(position);
        return world;
    }
    #endif
};
} // namespace PPGE
