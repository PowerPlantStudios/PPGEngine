#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/buffer.h"

namespace PPGE
{
class PPGE_API PPGEBufferD3D11 : public PPGEBuffer
{
  public:
    // Returns the CComPtr for the underlying D3D11 Buffer ptr
    // Use it when strong reference is needed for the interface
    virtual CComPtr<ID3D11Buffer> GetD3D11BufferShared() const = 0;

    // Returns the raw pointer in the underlying buffer object
    // Do not release it or delete it as it will break the ref count
    virtual ID3D11Buffer *GetD3D11BufferRaw() const = 0;
};
} // namespace PPGE
