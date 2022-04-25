#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/renderer_traits_d3d11.h"
#include "platform/d3d11/renderer_types_d3d11_conversion.h"
#include "rhi/buffer_base.h"

namespace PPGE
{
class BufferD3D11Impl final : public BufferBase<RendererTraitsD3D11>
{
  public:
    using BufferBaseType = BufferBase<RendererTraitsD3D11>;

    BufferD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, const BufferDesc &desc,
                    const BufferData *data = nullptr);

    ~BufferD3D11Impl();

    CComPtr<ID3D11Buffer> GetD3D11BufferShared() const override final
    {
        return m_d3d11_buffer_ptr;
    }

    ID3D11Buffer* GetD3D11BufferRaw() const override final
    {
        return m_d3d11_buffer_ptr.p;
    }

  private:
    CComPtr<ID3D11Buffer> m_d3d11_buffer_ptr;
};
} // namespace PPGE