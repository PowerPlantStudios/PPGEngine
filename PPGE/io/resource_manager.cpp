#include "resource_manager.h"

#include "system/io_system.h"

namespace PPGE
{
std::shared_ptr<Resource> ResourceManager::GetCachedResource(std::string_view name)
{
    size_t uuid = hasher(name);
    if (!IsResourceCached(uuid))
        LoadResource(m_resource_root / name);
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
    if (IsResourceCached(uuid))
    {
        PPGE_WARN("Resource #{0} has already been cached.", uuid);
        return;
    }
    m_resource_cache.emplace(uuid, std::move(resource));
}

void ResourceManager::FLushResource(std::string_view name)
{
    size_t uuid = hasher(name);
    FLushResource(uuid);
}

void ResourceManager::FLushResource(size_t uuid)
{
    if (!IsResourceCached(uuid))
    {
        PPGE_WARN("Resource #{0} not cached.", uuid);
        return;
    }
    m_resource_cache.erase(uuid);
}

void ResourceManager::LoadResource(const std::filesystem::path &path_to_resource)
{
    if (!std::filesystem::exists(path_to_resource))
    {
        PPGE_WARN("Resource '{0}' doesn't exist.", path_to_resource.string().c_str());
        return;
    }

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

void ResourceManager::ConnectFileSystemObserver(const FileSystemObserver::Callback &callback)
{
    if (m_file_system_observer)
    {
        PPGE_WARN("Resource manager has already been connected to a file system observer");
        DisconnectFileSystemObserver();
    }
    PPGE_ASSERT(!m_file_system_observer, "File system observer is still connected.");

    m_file_system_observer = std::move(IOSystem::Get().GetFileSystemObserver());
    m_file_system_observer->SetPath(m_resource_root);
    m_file_system_observer->SetCallback(callback);
    m_file_system_observer->StartObserving();
}

void ResourceManager::DisconnectFileSystemObserver()
{
    m_file_system_observer->StopObserving();
    m_file_system_observer.release();
}
} // namespace PPGE