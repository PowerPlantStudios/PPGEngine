#include "buffer_d3d11_impl.h"

#include "platform/d3d11/device_d3d11_impl.h"

namespace PPGE
{
BufferD3D11Impl::BufferD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, const BufferDesc &desc,
                                 const BufferData *data)
    : BufferBaseType(std::move(device_sp), desc)
{
    D3D11_BUFFER_DESC d3d11_buffer_desc{};
    d3d11_buffer_desc.ByteWidth = static_cast<UINT>(m_desc.byte_width);
    d3d11_buffer_desc.Usage = PPGEUsageTypeToD3D11UsageType(m_desc.usage);
    d3d11_buffer_desc.BindFlags = PPGEBindFlagsToD3D11BindFlags(m_desc.bind_flags);
    d3d11_buffer_desc.CPUAccessFlags = PPGECPUAccessFlagsToD3D11CPUAccessFlags(m_desc.cpu_access_flags);
    d3d11_buffer_desc.MiscFlags = 0;
    d3d11_buffer_desc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA d3d11_init_data{};
    d3d11_init_data.pSysMem = data ? data->data_ptr : nullptr;
    d3d11_init_data.SysMemPitch = 0;
    d3d11_init_data.SysMemSlicePitch = 0;

    auto d3d11_device = m_device_sp->GetD3D11Device();
    PPGE_HR(d3d11_device->CreateBuffer(&d3d11_buffer_desc, d3d11_init_data.pSysMem ? &d3d11_init_data : nullptr,
                                       &m_d3d11_buffer_ptr));

    PPGE_ASSERT(m_d3d11_buffer_ptr, "D3D11 Buffer creation failed.");
}

BufferD3D11Impl::~BufferD3D11Impl()
{
}
} // namespace PPGE