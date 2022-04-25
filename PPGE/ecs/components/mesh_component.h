#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/buffer.h"
#include "rhi/texture_view.h"
#include "system/logger_system.h"

namespace PPGE
{
struct MeshFilterComponent
{
    std::shared_ptr<PPGEBuffer> vertex_buffer;
    std::shared_ptr<PPGEBuffer> index_buffer;
    uint64_t num_indices;
};

struct MeshRendererComponent
{
    std::shared_ptr<PPGETextureView> albedo_map;
    Math::Color albedo_color;
    std::shared_ptr<PPGETextureView> specular_map;
    Math::Color specular_color;
    std::shared_ptr<PPGETextureView> normal_map;
};
} // namespace PPGE
