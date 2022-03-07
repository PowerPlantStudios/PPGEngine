#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/buffer.h"

namespace PPGE
{
class PPGE_API PPGEBufferD3D11 : public PPGEBuffer
{
  public:
    virtual ID3D11Buffer *GetD3D11Buffer() const = 0;
};
} // namespace PPGE
