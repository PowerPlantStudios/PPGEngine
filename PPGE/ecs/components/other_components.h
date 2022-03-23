#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/buffer.h"
#include "renderer/shader_resource_binding.h"
#include "system/logger_system.h"

namespace PPGE
{
struct TagComponent
{
    std::string tag;
};

struct TransformComponent
{
    PPGE::Math::Vector3 position = PPGE::Math::Vector3::Zero;
    PPGE::Math::Quaternion rotation = PPGE::Math::Quaternion::Identity;
    PPGE::Math::Vector3 scale = PPGE::Math::Vector3::One;
};

struct MeshFilterComponent
{
    std::shared_ptr<PPGEBuffer> vertex_buffer;
    std::shared_ptr<PPGEBuffer> index_buffer;
};

struct MeshRendererComponent
{
};
} // namespace PPGE
