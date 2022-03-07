#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/device_object_base.h"
#include "renderer/texture.h"
#include "system/logger_system.h"

namespace PPGE
{
template <typename RendererTraits>
class PPGE_API TextureBase
    : public DeviceObjectBase<typename RendererTraits::DeviceImpl, TextureDesc, typename RendererTraits::ITexture>,
      public std::enable_shared_from_this<typename RendererTraits::TextureImpl>
{
  public:
    using TextureInterface = typename RendererTraits::ITexture;

    using DeviceImplType = typename RendererTraits::DeviceImpl;

    using DeviceObjectBaseType = DeviceObjectBase<DeviceImplType, TextureDesc, TextureInterface>;

    TextureBase(std::shared_ptr<DeviceImplType> device_sp, const TextureDesc &desc)
        : DeviceObjectBaseType(std::move(device_sp), desc)
    {
    }

  protected:
    void ValidateResourceView(ResourceViewType resurce_view_type)
    {
        PPGE_ASSERT(resurce_view_type != ResourceViewType::RESOURCE_VIEW_UNDEFINED, "Resource view type is undefined.");
        PPGE_ASSERT(
            resurce_view_type == ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE ||
                resurce_view_type == ResourceViewType::RESOURCE_VIEW_RENDER_TARGET ||
                resurce_view_type == ResourceViewType::RESOURCE_VIEW_DEPTH_STENCIL,
            "Resource view type for texture resource can be either shader resource, render target or depth stencil.");
        PPGE_ASSERT((resurce_view_type == ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE &&
                     Enum::ToBoolean(m_desc.bind_flags & BindFlags::BIND_SHADER_RESOURCE)) ||
                        (resurce_view_type == ResourceViewType::RESOURCE_VIEW_RENDER_TARGET &&
                         Enum::ToBoolean(m_desc.bind_flags & BindFlags::BIND_RENDER_TARGET)) ||
                        (resurce_view_type == ResourceViewType::RESOURCE_VIEW_DEPTH_STENCIL &&
                         Enum::ToBoolean(m_desc.bind_flags & BindFlags::BIND_DEPTH_STENCIL)),
                    "Resource view type and resource bind flag doesn't match.");
    }
};
} // namespace PPGE