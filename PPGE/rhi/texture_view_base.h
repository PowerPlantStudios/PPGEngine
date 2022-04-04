#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/device_object_base.h"
#include "rhi/texture.h"
#include "rhi/texture_view.h"

namespace PPGE
{
template <typename RendererTraits>
class PPGE_API TextureViewBase : public DeviceObjectBase<typename RendererTraits::DeviceImpl, TextureViewDesc,
                                                         typename RendererTraits::ITextureView>
{
  public:
    using TextureViewInterface = typename RendererTraits::ITextureView;

    using DeviceImplType = typename RendererTraits::DeviceImpl;
    using TextureImplType = typename RendererTraits::TextureImpl;

    using DeviceObjectBaseType = DeviceObjectBase<DeviceImplType, TextureViewDesc, TextureViewInterface>;

    TextureViewBase(std::shared_ptr<DeviceImplType> device_sp, std::shared_ptr<TextureImplType> texture_sp,
                    const TextureViewDesc &desc)
        : DeviceObjectBaseType(std::move(device_sp), desc), m_texture_sp(std::move(texture_sp))
    {
    }

  protected:
    std::shared_ptr<TextureImplType> m_texture_sp;
};
} // namespace PPGE