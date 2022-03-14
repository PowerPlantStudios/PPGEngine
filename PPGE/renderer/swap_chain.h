#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/ppge_object.h"
#include "renderer/renderer_types.h"
#include "renderer/texture_view.h"

namespace PPGE
{
struct SwapChainDesc
{
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t refresh_rate_num = 0;
    uint32_t refresh_rate_denum = 0;
    TextureFormatType color_buffer_format = TextureFormatType::TEXTURE_FORMAT_R8G8B8A8_UNORM_SRGB;
    TextureFormatType depth_buffer_format = TextureFormatType::TEXTURE_FORMAT_D24_UNORM_S8_UINT;
    SwapChainUsageFlags swap_chain_usages = SwapChainUsageFlags::SWAP_CHAIN_USAGE_UNDEFINED;
    uint8_t buffer_count = 2;
};

class PPGE_API PPGESwapChain : public PPGEObject
{
  public:
    virtual const SwapChainDesc &GetDesc() const = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual void Present(uint32_t sync_interval) = 0;

    virtual PPGETextureView *GetBackBufferRTV() = 0;

    virtual PPGETextureView *GetDepthBufferDSV() = 0;
};
} // namespace PPGE
