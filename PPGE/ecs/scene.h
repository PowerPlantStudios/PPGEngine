#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "ecs/entity.h"
#include "system/logger_system.h"

namespace PPGE
{
namespace SceneTraits
{
template <typename... ComponentType> using ViewFilter_t = entt::exclude_t<ComponentType...>;

template <typename... ComponentType> inline constexpr ViewFilter_t<ComponentType...> ViewFilter{};

using EntityCallback_t = std::function<void(Entity)>;
} // namespace SceneTraits

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

    template <typename ComponentType, typename... Other, typename... Filter>
    decltype(auto) View(SceneTraits::ViewFilter_t<Filter...> = {})
    {
        return m_registry.view<ComponentType, Other...>(SceneTraits::ViewFilter<Filter...>);
    }

    template <typename ComponentType, typename... Other, typename... Filter>
    decltype(auto) View(SceneTraits::ViewFilter_t<Filter...> = {}) const
    {
        return std::as_const(m_registry).view<const ComponentType, const Other...>(SceneTraits::ViewFilter<Filter...>);
    }

    decltype(auto) ForEachEntity(SceneTraits::EntityCallback_t func)
    {
        return m_registry.each([this, &func](entt::entity entity_handle) {
            Entity entity(this, entity_handle);
            func(entity);
        });
    }

  private:
    friend entt::registry &Entity::GetRegistry() const;

    entt::registry m_registry;
};
} // namespace PPGE
