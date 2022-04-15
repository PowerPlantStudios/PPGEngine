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
};

struct ShaderResourceCreateDesc
{
    const char *name;
    ShaderResourceDesc desc;
};

class PPGE_API PPGEShaderResourceVariable : public PPGEObject
{
  public:
    virtual const ShaderResourceDesc &GetDesc() const = 0;

    virtual void Set(std::shared_ptr<PPGEDeviceObject> device_object_sp) = 0;

    virtual std::shared_ptr<PPGEDeviceObject> Get() const = 0;
};
} // namespace PPGE