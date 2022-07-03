#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "io/resources.h"
#include "rhi/buffer.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
class Entity;
class PPGETextureView;

namespace ImportHelper
{

void ImportModel(const LazyResource &resource, Entity &entity);

} // namespace ImportHelper
} // namespace PPGE
