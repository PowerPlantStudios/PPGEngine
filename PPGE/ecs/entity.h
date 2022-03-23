#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
class Scene;

class Entity
{
  public:
    Entity() = delete;

    Entity(Scene &scene_ref, entt::entity handle);

    Entity(const Entity &other) = default;

    Entity &operator=(const Entity &other) = default;

    template <typename ComponentType, typename... Args> ComponentType &AddComponent(Args &&...args)
    {
        auto &registry = GetRegistry();
        return registry.emplace<ComponentType>(m_handle, std::forward<Args>(args)...);
    }

    template<typename ComponentType, typename... Other>
    void RemoveComponents()
    {
        auto &registry = GetRegistry();
        registry.erase<ComponentType, Other...>(m_handle);
    }

  private:
    friend class Scene;

    entt::registry &GetRegistry() const;

    Scene &m_scene_ref;

    entt::entity m_handle = entt::null;
};
} // namespace PPGE
