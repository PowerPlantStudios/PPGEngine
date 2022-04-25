#include "entity.h"

#include "ecs/scene.h"

namespace PPGE
{
Entity::Entity(Scene *scene_ptr, entt::entity handle) : m_scene_ptr(scene_ptr), m_handle(handle)
{
}

entt::registry &Entity::GetRegistry() const
{
    PPGE_ASSERT(m_scene_ptr, "Entity doesn't have valid pointer to a scene. It is either not created by a scene or not "
                             "assigned to a valid entity.");
    return m_scene_ptr->m_registry;
}

} // namespace PPGE
