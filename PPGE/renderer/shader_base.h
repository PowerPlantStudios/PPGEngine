#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/device_object_base.h"
#include "renderer/shader.h"

namespace PPGE
{
template <typename RendererTraits>
class PPGE_API ShaderBase
    : public DeviceObjectBase<typename RendererTraits::DeviceImpl, ShaderDesc, typename RendererTraits::IShader>
{
  public:
    using ShaderInterface = typename RendererTraits::IShader;

    using DeviceImplType = typename RendererTraits::DeviceImpl;

    using DeviceObjectBaseType = DeviceObjectBase<DeviceImplType, ShaderDesc, ShaderInterface>;

    ShaderBase(std::shared_ptr<DeviceImplType> device_sp, const ShaderDesc &desc)
        : DeviceObjectBaseType(std::move(device_sp), desc)
    {
        // TODO: CHECK IF DEVICE SUPPORTS THE TARGET SHADER TYPE. FOR EXAMPLE, DX11 OR GL DOESN'T SUPPORT RTX SHADER
        // TYPES
    }
};
} // namespace PPGE