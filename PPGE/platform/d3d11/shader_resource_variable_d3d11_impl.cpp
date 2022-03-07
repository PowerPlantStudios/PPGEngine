#include "shader_resource_variable_d3d11_impl.h"

#include "platform/d3d11/buffer_d3d11_impl.h"

namespace PPGE
{
void ConstantBufferBindingD3D11::BindResource(std::shared_ptr<PPGEDeviceObject> device_object_sp)
{
    m_device_resource_sp = std::static_pointer_cast<BufferD3D11Impl>(std::move(device_object_sp));
}

ID3D11DeviceChild *ConstantBufferBindingD3D11::GetD3D11Resource() const
{
    ID3D11DeviceChild *resource_ptr;
    ID3D11Buffer *buffer_ptr = m_device_resource_sp->GetD3D11Buffer();
    PPGE_HR(buffer_ptr->QueryInterface(__uuidof(ID3D11DeviceChild), reinterpret_cast<void **>(&resource_ptr)));
    return resource_ptr;
}
} // namespace PPGE