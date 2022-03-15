#include "shader_resource_variable_d3d11_impl.h"

#include "platform/d3d11/buffer_d3d11_impl.h"
#include "platform/d3d11/texture_view_d3d11_impl.h"
#include "platform/d3d11/sampler_d3d11_impl.h"

namespace PPGE
{
void ConstantBufferBindingD3D11::BindResource(std::shared_ptr<PPGEDeviceObject> device_object_sp)
{
    m_device_resource_sp = std::static_pointer_cast<BufferD3D11Impl>(std::move(device_object_sp));
}

std::shared_ptr<PPGEDeviceObject> ConstantBufferBindingD3D11::GetResource() const
{
    return std::static_pointer_cast<PPGEDeviceObject>(m_device_resource_sp);
}

CComPtr<ID3D11DeviceChild> ConstantBufferBindingD3D11::GetD3D11Resource() const
{
    CComPtr<ID3D11DeviceChild> resource_ptr;
    CComPtr<ID3D11Buffer> buffer_ptr = m_device_resource_sp->GetD3D11Buffer();
    PPGE_HR(buffer_ptr.QueryInterface<ID3D11DeviceChild>(&resource_ptr));
    return resource_ptr;
}

void TextureSRVBindingD3D11::BindResource(std::shared_ptr<PPGEDeviceObject> device_object_sp)
{
    m_device_resource_sp = std::static_pointer_cast<TextureViewD3D11Impl>(std::move(device_object_sp));
}

std::shared_ptr<PPGEDeviceObject> TextureSRVBindingD3D11::GetResource() const
{
    return std::static_pointer_cast<PPGEDeviceObject>(m_device_resource_sp);
}

CComPtr<ID3D11DeviceChild> TextureSRVBindingD3D11::GetD3D11Resource() const
{
    CComPtr<ID3D11DeviceChild> resource_ptr;
    CComPtr<ID3D11View> srv_ptr = m_device_resource_sp->GetD3D11View();
    PPGE_HR(srv_ptr.QueryInterface<ID3D11DeviceChild>(&resource_ptr));
    return resource_ptr;
}

void SamplerBindingD3D11::BindResource(std::shared_ptr<PPGEDeviceObject> device_object_sp)
{
    m_device_resource_sp = std::static_pointer_cast<SamplerD3D11Impl>(std::move(device_object_sp));
}

std::shared_ptr<PPGEDeviceObject> SamplerBindingD3D11::GetResource() const
{
    return std::static_pointer_cast<PPGEDeviceObject>(m_device_resource_sp);
}

CComPtr<ID3D11DeviceChild> SamplerBindingD3D11::GetD3D11Resource() const
{
    CComPtr<ID3D11DeviceChild> resource_ptr;
    CComPtr<ID3D11SamplerState> sampler_ptr = m_device_resource_sp->GetD3D11SamplerState();
    PPGE_HR(sampler_ptr.QueryInterface<ID3D11DeviceChild>(&resource_ptr));
    return resource_ptr;
}

} // namespace PPGE