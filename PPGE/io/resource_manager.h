#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "io/file_system_observer.h"
#include "io/resource_loaders.h"
#include "io/resources.h"
#include "system/logger_system.h"

namespace PPGE
{
class ResourceManager
{
    using ResourceLoaderCallback = std::function<void(const std::filesystem::path &)>;

  public:
    ResourceManager() = default;

    ResourceManager(const ResourceManager &) = delete;

    [[nodiscard]] bool IsCacheEmpty() const
    {
        return m_resource_cache.empty();
    }

    [[nodiscard]] bool IsResourceCached(size_t uuid) const
    {
        return m_resource_cache.find(uuid) != m_resource_cache.end();
    }

    [[nodiscard]] bool IsResourceCached(std::string_view name) const
    {
        size_t uuid = hasher(name);
        return IsResourceCached(uuid);
    }

    std::shared_ptr<Resource> GetCachedResource(std::string_view name);

    std::shared_ptr<Resource> GetCachedResource(size_t uuid);

    void CacheResource(std::string_view name, std::shared_ptr<Resource> resource);

    void CacheResource(size_t uuid, std::shared_ptr<Resource> resource);

    void LoadResource(const std::filesystem::path &path_to_resource);

    void WalkRoot(const std::filesystem::path &resource_root);

    void SafeCleanCache();

    void ConnectFileSystemObserver(const FileSystemObserver::Callback& callback);

    void DisconnectFileSystemObserver();

    template <typename ResourceType, typename LoaderType,
              typename std::enable_if_t<std::is_base_of_v<Resource, ResourceType>> * = nullptr,
              typename std::enable_if_t<std::is_base_of_v<ResourceLoader, LoaderType>> * = nullptr>
    void RegisterLoaderMultiple(std::initializer_list<const char *> extensions)
    {
        for (auto extension : extensions)
        {
            RegisterLoader<ResourceType, LoaderType>(extension);
        }
    }

    template <typename ResourceType, typename LoaderType,
              typename std::enable_if_t<std::is_base_of_v<Resource, ResourceType>> * = nullptr,
              typename std::enable_if_t<std::is_base_of_v<ResourceLoader, LoaderType>> * = nullptr>
    void RegisterLoader(const std::string &extension)
    {
        auto it = m_loaders.find(extension);
        if (it != m_loaders.end())
        {
            PPGE_WARN("Loader for the extension {0} has already been registered.", extension.c_str());
            return;
        }

        m_loaders.emplace(extension, [&](const std::filesystem::path &path_to_resource) {
            LoaderType loader;

            auto resource = std::make_shared<ResourceType>();
            PPGE_INFO("Loading resource: {0}", path_to_resource.string().c_str());
            resource->visit(loader, path_to_resource);

            auto rel_path = std::filesystem::relative(path_to_resource, m_resource_root).string();
            std::replace(rel_path.begin(), rel_path.end(), '\\', '/');
            PPGE_INFO("Cacheing resource: {0}", rel_path.c_str());
            CacheResource(rel_path, std::move(std::static_pointer_cast<Resource>(std::move(resource))));
        });
    }

  private:
    std::filesystem::path m_resource_root;

    std::unique_ptr<FileSystemObserver> m_file_system_observer;

    std::unordered_map<std::string, ResourceLoaderCallback> m_loaders;

    std::hash<std::string_view> hasher;

    std::unordered_map<size_t, std::shared_ptr<Resource>> m_resource_cache;
};
} // namespace PPGE