#include "entity.h"

#include "ecs/components/transform_components.h"
#include "ecs/scene.h"

namespace PPGE
{
Entity::Entity(Scene *scene_ptr, entt::entity handle)
    : m_scene_ptr(scene_ptr), m_handle(handle), m_childred(), m_parent(nullptr)
{
}

Entity &Entity::CreateChildEntity()
{
    PPGE_ASSERT(m_scene_ptr, "Reference to scene is not valid.");

    m_childred.push_back(m_scene_ptr->CreateEntity());
    auto &child = m_childred.back();
    child.m_parent = this;
    
    return child;
}

entt::registry &Entity::GetRegistry() const
{
    PPGE_ASSERT(m_scene_ptr, "Entity doesn't have valid pointer to a scene. It is either not created by a scene or not "
                             "assigned to a valid entity.");
    return m_scene_ptr->m_registry;
}

void Entity::Destroy()
{
    for (auto &child : m_childred)
    {
        child.Destroy();
        child.m_parent = nullptr;
    }

    GetRegistry().destroy(m_handle);
    m_handle = entt::null;

    m_scene_ptr = nullptr;
}

} // namespace PPGE
