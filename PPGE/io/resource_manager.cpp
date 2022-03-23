#include "resource_manager.h"

namespace PPGE
{
std::shared_ptr<Resource> ResourceManager::GetCachedResource(std::string_view name)
{
    size_t uuid = hasher(name);
    return GetCachedResource(uuid);
}

std::shared_ptr<Resource> ResourceManager::GetCachedResource(size_t uuid)
{
    return IsResourceCached(uuid) ? m_resource_cache[uuid] : nullptr;
}

void ResourceManager::CacheResource(std::string_view name, std::shared_ptr<Resource> resource)
{
    size_t uuid = hasher(name);
    CacheResource(uuid, std::move(resource));
}

void ResourceManager::CacheResource(size_t uuid, std::shared_ptr<Resource> resource)
{
    auto it = m_resource_cache.find(uuid);
    if (it != m_resource_cache.end())
    {
        PPGE_WARN("Resource #{0} has already been cached.", uuid);
        return;
    }
    m_resource_cache.emplace(uuid, std::move(resource));
}

void ResourceManager::LoadResource(const std::filesystem::path &path_to_resource)
{
    if (!path_to_resource.has_extension())
    {
        PPGE_WARN("Resource '{0}' has no extension. It will be skipped in resource root directory scan.",
                  path_to_resource.string().c_str());
        return;
    }

    auto ext = path_to_resource.extension().string();

    auto it = m_loaders.find(ext);
    if (it == m_loaders.end())
    {
        PPGE_WARN("No loader is registered for resources with '{0}' extension. Loading resource '{1}' will be "
                  "skipped.",
                  ext, path_to_resource.string().c_str());
        return;
    }

    (it->second)(path_to_resource);
}

void ResourceManager::WalkRoot(const std::filesystem::path &resource_root)
{
    m_resource_root = resource_root;
    for (auto it : std::filesystem::recursive_directory_iterator(
             m_resource_root, std::filesystem::directory_options::skip_permission_denied))
    {
        if (it.is_directory())
            continue;

        auto &path_to_resource = it.path();

        LoadResource(path_to_resource);
    }
}

void ResourceManager::SafeCleanCache()
{
    for (auto it = m_resource_cache.begin(); it != m_resource_cache.end();)
    {
        if (it->second.use_count() == 1)
        {
            it = m_resource_cache.erase(it);
        }
        else
        {
            it++;
        }
    }
}
} // namespace PPGE