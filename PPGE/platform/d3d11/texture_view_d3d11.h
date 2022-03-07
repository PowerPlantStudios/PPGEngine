#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/texture_view.h"

namespace PPGE
{
class PPGE_API PPGETextureViewD3D11 : public PPGETextureView
{
  public:
    virtual ID3D11View *GetD3D11View() const = 0;
};
} // namespace PPGE