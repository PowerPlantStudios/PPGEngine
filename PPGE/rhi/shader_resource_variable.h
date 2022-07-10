#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/ppge_object.h"
#include "rhi/device_object.h"
#include "rhi/rhi_types.h"

namespace PPGE
{
struct ShaderResourceDesc
{
    ShaderTypeFlags shader_types;
    ShaderResourceType resource_type;
    uint32_t offest = 0;
};

struct ShaderResourceRangeCreateDesc
{
    ShaderResourceDesc sr_desc;
    uint32_t sr_name_num = 0;
    const char **sr_name = nullptr;
};

struct ShaderResourceCreateDesc
{
    uint32_t range_num = 0;
    ShaderResourceRangeCreateDesc *range = nullptr;
};

class PPGE_API PPGEShaderResourceVariable : public PPGEObject
{
  public:
    virtual const ShaderResourceDesc &GetDesc() const = 0;

    virtual void Set(std::shared_ptr<PPGEDeviceObject> device_object_sp) = 0;

    virtual std::shared_ptr<PPGEDeviceObject> Get() const = 0;
};
} // namespace PPGE