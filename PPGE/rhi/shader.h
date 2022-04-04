#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/device_object.h"

namespace PPGE
{
inline const char *ShaderTypeToString(ShaderTypeFlags shader_type)
{
    switch (shader_type)
    {
    // clang-format off
    case PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX:            return "Vertex Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL:             return "Pixel Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_GEOMETRY:          return "Geometry Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_HULL:              return "Hull Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_DOMAIN:            return "Domain Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_COMPUTE:           return "Compute Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_AMPLIFICATION:     return "Amplification Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_MESH:              return "Mesh Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_RAY_GEN:           return "Ray Gen Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_RAY_MISS:          return "Ray Miss Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_RAY_CLOSEST_HIT:   return "Ray Closest Hit Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_RAY_ANY_HIT:       return "Ray Any Hit Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_RAY_INTERSECTION:  return "Ray Intersection Shader";
    case PPGE::ShaderTypeFlags::SHADER_TYPE_CALLABLE:          return "Callable Shader";
    default:
        return "Unknown Shader";
    // clang-format on
    }
}

struct ShaderDesc : public DeviceObjectDesc
{
    ShaderTypeFlags shader_type_flags = ShaderTypeFlags::SHADER_TYPE_UNKNOW;
};

struct ShaderCreateDesc
{
    ShaderDesc desc;

    const char *file_path = nullptr;

    const char *source_code = nullptr;

    const char *entry_point_name = "main";
    ShaderLanguageType lang = ShaderLanguageType::SHADER_LANGUAGE_DEFAULT;
    ShaderCompilerType compiler = ShaderCompilerType::SHADER_COMPILER_DEFAULT;

    const char *byte_code = nullptr;
    union {
        size_t source_code_size = 0;
        size_t byte_code_size;
    };
};

class PPGE_API PPGEShader : public PPGEDeviceObject
{
  public:
    virtual const ShaderDesc &GetDesc() const = 0;
};
} // namespace PPGE
