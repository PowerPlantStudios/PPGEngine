#include "shader_library.h"

namespace PPGE
{
void ShaderLibrary::Subscribe(const std::string &id, const SubscriberCallback &callback)
{
    if (m_subscribers.find(id) != m_subscribers.end())
    {
        PPGE_ASSERT(false, "ShaderLibrary::Subscribe : Subscriber ID already exists.");
        return;
    }

    m_subscribers.emplace(id, callback);
}

void ShaderLibrary::Unsubscribe(const std::string &id)
{
    auto result = m_subscribers.erase(id);
    PPGE_ASSERT(result == 0, "ShaderLibrary::Unsubscribe : Subscriber ID not found.");
}

const TextResource &ShaderLibrary::GetShaderCode(const std::string &name)
{
    static TextResource dummy_resource;

    const auto &it = m_cached_shader_codes.find(name);
    if (it != m_cached_shader_codes.end())
        return *(it->second.get());

    auto resource = m_resource_mgr.GetCachedResource(name);
    if (resource)
    {
        auto [it, success] =
            m_cached_shader_codes.emplace(name, std::move(std::static_pointer_cast<TextResource>(std::move(resource))));
        return *(it->second.get());
    }

    PPGE_ERROR("Look up for the resource {0} has failed.", name.c_str());
    return dummy_resource;
}

const ByteResource &ShaderLibrary::GetShaderByte(const std::string &name)
{
    static ByteResource dummy_resource;

    const auto &it = m_cached_shader_byte.find(name);
    if (it != m_cached_shader_byte.end())
        return *(it->second.get());

    auto resource = m_resource_mgr.GetCachedResource(name);
    if (resource)
    {
        auto [it, success] =
            m_cached_shader_byte.emplace(name, std::move(std::static_pointer_cast<ByteResource>(std::move(resource))));
        return *(it->second.get());
    }

    PPGE_ERROR("Look up for the resource {0} has failed.", name.c_str());
    return dummy_resource;
}

ShaderLibrary::ShaderLibrary()
{
    m_resource_mgr.RegisterLoaderMultiple<PPGE::TextResource, PPGE::TextLoader>({".hlsl", ".glsl"});
    m_resource_mgr.WalkRoot("../../PPGE/renderer/shaders");
    m_resource_mgr.ConnectFileSystemObserver(
        [this](const std::filesystem::path &path, FileSystemObserver::FileAction action) {
            if (std::filesystem::is_directory(path))
                return;

            switch (action)
            {
            case FileSystemObserver::FileAction::MODIFIED: {
                auto shader = std::filesystem::relative(path, "../../PPGE/renderer/shaders").string();

                FlushCachedShader(shader);

                for (auto &[id, callback] : m_subscribers)
                {
                    callback(shader);
                }
            }
            break;
            case FileSystemObserver::FileAction::ADDED:
                PPGE_TRACE("File '{0}' is added to shader library.", path.string());
                break;
            case FileSystemObserver::FileAction::REMOVED:
                PPGE_WARN("File '{0}' is removed from shader library.", path.string());
                break;
            case FileSystemObserver::FileAction::RENAMED_FROM:
                PPGE_WARN("File in shader library is renamed from '{0}'", path.string());
                break;
            case FileSystemObserver::FileAction::RENAMED_TO:
                PPGE_WARN("File in shader library is renamed to '{0}'", path.string());
                break;
            case FileSystemObserver::FileAction::UNKNOWN:
            default:
                break;
            }
        });
}

ShaderLibrary::~ShaderLibrary()
{
    m_resource_mgr.DisconnectFileSystemObserver();
}

void ShaderLibrary::FlushCachedShader(const std::string &shader)
{
    m_resource_mgr.FLushResource(shader);
    if (m_cached_shader_codes.erase(shader) != 0)
        PPGE_TRACE("Removed cached shader code: {0}", shader);
    if (m_cached_shader_byte.erase(shader) != 0)
        PPGE_TRACE("Removed cached shader byte: {0}", shader);
}
} // namespace PPGE