#include "scene.h"

#include "ecs/components/camera_component.h"
#include "ecs/components/debug_component.h"
#include "ecs/components/light_component.h"
#include "ecs/components/mesh_component.h"
#include "ecs/components/transform_components.h"
#include "ecs/entity.h"

namespace PPGE
{
Entity Scene::CreateEntity()
{
    Entity entity(this, m_registry.create());
    entity.AddComponent<TransformComponent>();
    return entity;
}

Entity Scene::CreateEntity(const std::string &tag)
{
    Entity entity = CreateEntity();
    auto &tag_component = entity.AddComponent<TagComponent>();
    tag_component.tag = tag;
    return entity;
}

void Scene::DestroyEntity(Entity entity)
{
    entity.Destroy();
}
} // namespace PPGE
