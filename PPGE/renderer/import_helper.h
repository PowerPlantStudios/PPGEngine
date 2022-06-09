#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "io/resources.h"
#include "system/logger_system.h"

namespace PPGE
{
class Entity;
class PPGETextureView;

class ModelLoader
{
  public:
    static void ImportModel(const LazyResource &resource, Entity &entity);

    static std::shared_ptr<PPGETextureView> ImportTexture(const LazyResource &resource);
};
} // namespace PPGE
