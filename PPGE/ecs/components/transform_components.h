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
    PPGE::Math::Vector3 position = PPGE::Math::Vector3::Zero;
    PPGE::Math::Quaternion rotation = PPGE::Math::Quaternion::Identity;
    PPGE::Math::Vector3 scale = PPGE::Math::Vector3::One;
};
} // namespace PPGE
