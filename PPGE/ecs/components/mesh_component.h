#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/material.h"
#include "renderer/mesh.h"
#include "system/logger_system.h"

namespace PPGE
{
struct MeshFilterComponent
{
    std::shared_ptr<Mesh> mesh;
};

struct MeshRendererComponent
{
    std::shared_ptr<Material> material;
};
} // namespace PPGE
