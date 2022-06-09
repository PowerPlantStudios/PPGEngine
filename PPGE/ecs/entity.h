#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
namespace EntityTraits
{
using EntityHandle = entt::entity;
} // namespace EntityTraits

class Scene;

class Entity
{
  public:
    Entity() = default;

    Entity(const Entity &other) = default;

    Entity &operator=(const Entity &other) = default;

    bool operator==(const Entity &rhs) const
    {
        return (rhs.m_handle == m_handle) && (rhs.m_scene_ptr == m_scene_ptr);
    }

    bool operator!=(const Entity &rhs) const
    {
        return !(*this == rhs);
    }

    operator uint64_t() const
    {
        return static_cast<uint64_t>(m_handle);
    }

    operator bool() const
    {
        return IsValid();
    }

    inline bool IsValid() const
    {
        return m_scene_ptr && m_handle != entt::null && GetRegistry().valid(m_handle);
    }

    Entity &CreateChildEntity();

    /**
     * Attempting to add a component that already exists for the
     * given entity results in underfined behaviour. If in doubt, it is
     * recommended to use Has function to check before adding.
     */
    template <typename ComponentType, typename... Args> decltype(auto) AddComponent(Args &&...args)
    {
        auto &registry = GetRegistry();
        return registry.emplace<ComponentType>(m_handle, std::forward<Args>(args)...);
    }

    /**
     * Patching a component that doesn't exist for the given entity
     * results in undefined behaviour. If in doubt, it is recommended
     * to use Has function to check before patching.
     */
    template <typename ComponentType, typename... Funcs> decltype(auto) PatchComponent(Funcs &&...funcs)
    {
        auto &registry = GetRegistry();
        return registry.patch<ComponentType>(m_handle, std::forward<Funcs>(funcs)...);
    }

    /**
     * Slightly slower than directly adding an un-existing component
     * or replacing an existing one, however safer when the component type
     * might or might not exist for the given entity.
     */
    template <typename ComponentType, typename... Args> decltype(auto) AddOrReplaceComponent(Args &&...args)
    {
        auto &registry = GetRegistry();
        return registry.emplace_or_replace<ComponentType>(m_handle, std::forward<Args>(args)...);
    }

    /**
     * Erases the given component type from the given entity if it exists.
     * Otherwise it results in undefined behavior.
     */
    template <typename ComponentType, typename... Other> void EraseComponents()
    {
        auto &registry = GetRegistry();
        registry.erase<ComponentType, Other...>(m_handle);
    }

    /**
     * When the given componenty type is not available for the given entity,
     * function returns safely and doesn't result in undefined behavior.
     */
    template <typename ComponentType, typename... Other> void RemoveComponents()
    {
        auto &registry = GetRegistry();
        registry.remove<ComponentType, Other...>(m_handle);
    }

    template <typename... ComponentTypes> decltype(auto) GetComponents()
    {
        auto &registry = GetRegistry();
        return registry.get<ComponentTypes...>(m_handle);
    }

    template <typename... ComponentTypes> decltype(auto) GetComponents() const
    {
        const auto &registry = GetRegistry();
        return registry.get<ComponentTypes...>(m_handle);
    }

    template <typename ComponentType, typename... Args> decltype(auto) GetOrEmplaceComponent(Args &&...args)
    {
        auto &registry = GetRegistry();
        return registry.get_or_emplace<ComponentType>(m_handle, std::forward<Args>(args)...);
    }

    template <typename ComponentType> bool Has()
    {
        auto &registry = GetRegistry();
        return registry.all_of<ComponentType>(m_handle);
    }

    template <typename... ComponentTypes> bool HasAllOf()
    {
        auto &registry = GetRegistry();
        return registry.all_of<ComponentTypes...>(m_handle);
    }

    template <typename... ComponentTypes> bool HasAnyOf()
    {
        auto &registry = GetRegistry();
        return registry.any_of<ComponentTypes...>(m_handle);
    }

  private:
    friend class Scene;

    Entity(Scene *scene_ptr, entt::entity handle);

    entt::registry &GetRegistry() const;

    void Destroy();

    Scene *m_scene_ptr = nullptr;

    EntityTraits::EntityHandle m_handle = entt::null;

    std::vector<Entity> m_childred;
    
    Entity *m_parent;
};
} // namespace PPGE
