#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/texture.h"

namespace PPGE
{
class PPGE_API PPGETextureD3D11 : public PPGETexture
{
  public:
    virtual ID3D11Resource *GetD3D11Resource() const = 0;
};
} // namespace PPGE
