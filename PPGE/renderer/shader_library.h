#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "io/resource_manager.h"

namespace PPGE
{
class PPGE_API ShaderLibrary
{
  public:
    static ShaderLibrary &Get()
    {
        static ShaderLibrary s_instance;
        return s_instance;
    }

    const TextResource &GetShaderCode(const std::string &name);

    const ByteResource &GetShaderByte(const std::string &name);

  private:
    ShaderLibrary();

    ShaderLibrary(const ShaderLibrary &) = delete;

    ShaderLibrary &operator=(const ShaderLibrary &) = delete;

    ResourceManager m_resource_mgr;

    std::unordered_map<std::string, std::shared_ptr<TextResource>> m_cached_shader_codes;

    std::unordered_map<std::string, std::shared_ptr<ByteResource>> m_cached_shader_byte;
};
} // namespace PPGE