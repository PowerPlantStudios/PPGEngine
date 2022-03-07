#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/swap_chain.h"
#include "platform/d3d11/texture_view_d3d11.h"

namespace PPGE
{
class PPGE_API PPGESwapChainD3D11 : public PPGESwapChain
{
  public:
    virtual IDXGISwapChain *GetDXGISwapChain() const = 0;

    virtual PPGETextureViewD3D11 *GetBackBufferRTV() = 0;

    virtual PPGETextureViewD3D11 *GetDepthBufferDSV() = 0;
};
} // namespace PPGE
