#include "entity.h"

#include "ecs/scene.h"

namespace PPGE
{
Entity::Entity(Scene &scene_ref, entt::entity handle) : m_scene_ref(scene_ref), m_handle(handle)
{
}

entt::registry &Entity::GetRegistry() const
{
    return m_scene_ref.m_registry;
}

} // namespace PPGE
