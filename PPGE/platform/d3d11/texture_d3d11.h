#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/texture.h"

namespace PPGE
{
class PPGE_API PPGETextureD3D11 : public PPGETexture
{
  public:
    virtual CComPtr<ID3D11Resource> GetD3D11Resource() const = 0;
};
} // namespace PPGE
