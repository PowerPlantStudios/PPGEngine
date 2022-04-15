#include "shader_library.h"

namespace PPGE
{
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
}
} // namespace PPGE