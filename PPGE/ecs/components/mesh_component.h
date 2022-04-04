#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/buffer.h"
#include "rhi/shader_resource_binding.h"
#include "system/logger_system.h"

namespace PPGE
{
struct MeshFilterComponent
{
    std::shared_ptr<PPGEBuffer> vertex_buffer;
    std::shared_ptr<PPGEBuffer> index_buffer;
};

struct MeshRendererComponent
{
};
} // namespace PPGE
