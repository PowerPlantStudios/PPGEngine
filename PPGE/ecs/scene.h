#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "ecs/entity.h"
#include "system/logger_system.h"

namespace PPGE
{
class Scene
{
  public:
    Scene()
    {
    }

    ~Scene()
    {
    }

    Entity CreateEntity();

    Entity CreateEntity(const std::string &tag);
    
    void DestroyEntity(Entity entity);

  private:
    friend entt::registry &Entity::GetRegistry() const;

    entt::registry m_registry;
};
} // namespace PPGE
