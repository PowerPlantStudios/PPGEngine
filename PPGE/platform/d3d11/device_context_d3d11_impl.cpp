#include "device_context_d3d11_impl.h"

#include "platform/d3d11/buffer_d3d11_impl.h"
#include "platform/d3d11/renderer_types_d3d11_conversion.h"

namespace PPGE
{
void DeviceContextD3D11Impl::SetVertexBuffers(uint32_t num_buffers, PPGEBuffer *p_buffer)
{
    std::vector<ID3D11Buffer *> vertex_buffers;
    vertex_buffers.reserve(num_buffers);
    for (uint32_t i = 0; i < num_buffers; i++)
    {
        auto d3d11_buffer = reinterpret_cast<PPGEBufferD3D11 *>(p_buffer + i);
        vertex_buffers.push_back(d3d11_buffer->GetD3D11Buffer());
    }
    UINT stride = 32;
    UINT offset = 0;
    m_d3d11_device_context_ptr->IASetVertexBuffers(0, num_buffers, vertex_buffers.data(), &stride, &offset);
}

void DeviceContextD3D11Impl::SetIndexBuffer(PPGEBuffer *p_buffer)
{
    auto d3d11_buffer = reinterpret_cast<PPGEBufferD3D11 *>(p_buffer);
    m_d3d11_device_context_ptr->IASetIndexBuffer(d3d11_buffer->GetD3D11Buffer(), DXGI_FORMAT_R16_UINT, 0);
}

void DeviceContextD3D11Impl::SetPipelineObject(PPGEPipelineState *p_PSO)
{
    b_is_shader_bound.fill(false);

    auto d3d11_pso = reinterpret_cast<PPGEPipelineStateD3D11 *>(p_PSO);

    if (auto d3d11_shader = d3d11_pso->GetD3D11VertexShader())
    {
        m_d3d11_device_context_ptr->VSSetShader(d3d11_shader, nullptr, 0);
        b_is_shader_bound[0] = true;
    }
    if (auto d3d11_shader = d3d11_pso->GetD3D11PixelShader())
    {
        m_d3d11_device_context_ptr->PSSetShader(d3d11_shader, nullptr, 0);
        b_is_shader_bound[1] = true;
    }
    if (auto d3d11_shader = d3d11_pso->GetD3D11GeometryShader())
    {
        m_d3d11_device_context_ptr->GSSetShader(d3d11_shader, nullptr, 0);
        b_is_shader_bound[2] = true;
    }
    if (auto d3d11_shader = d3d11_pso->GetD3D11HullShader())
    {
        m_d3d11_device_context_ptr->HSSetShader(d3d11_shader, nullptr, 0);
        b_is_shader_bound[3] = true;
    }
    if (auto d3d11_shader = d3d11_pso->GetD3D11DomainShader())
    {
        m_d3d11_device_context_ptr->DSSetShader(d3d11_shader, nullptr, 0);
        b_is_shader_bound[4] = true;
    }
    if (auto d3d11_shader = d3d11_pso->GetD3D11ComputeShader())
    {
        m_d3d11_device_context_ptr->CSSetShader(d3d11_shader, nullptr, 0);
        b_is_shader_bound[5] = true;
    }

    if (auto d3d11_layout = d3d11_pso->GetD3D11InputLayout())
        m_d3d11_device_context_ptr->IASetInputLayout(d3d11_layout);
}

void DeviceContextD3D11Impl::SetShaderResourceBinding(PPGEShaderResourceBinding *p_SRB)
{
    auto d3d11_srb = reinterpret_cast<PPGEShaderResourceBindingD3D11 *>(p_SRB);
#define SET_RESOURCE(SHADER_TYPE, D3D11_PREFIX)                                                                        \
    {                                                                                                                  \
        const auto &##D3D11_PREFIX##_Bindable = d3d11_srb->GetD3D11Bindables(ShaderTypeFlags::SHADER_##SHADER_TYPE##); \
        if (##D3D11_PREFIX##_Bindable.constant_buffers.size() > 0)                                                     \
            m_d3d11_device_context_ptr->##D3D11_PREFIX##SetConstantBuffers(                                            \
                0, ##D3D11_PREFIX##_Bindable.constant_buffers.size(),                                                  \
                ##D3D11_PREFIX##_Bindable.constant_buffers.data());                                                    \
        if (##D3D11_PREFIX##_Bindable.shader_resource_views.size() > 0)                                                \
            m_d3d11_device_context_ptr->##D3D11_PREFIX##SetShaderResources(                                            \
                0, ##D3D11_PREFIX##_Bindable.shader_resource_views.size(),                                             \
                ##D3D11_PREFIX##_Bindable.shader_resource_views.data());                                               \
        if (##D3D11_PREFIX##_Bindable.sampler_states.size() > 0)                                                       \
            m_d3d11_device_context_ptr->##D3D11_PREFIX##SetSamplers(                                                   \
                0, ##D3D11_PREFIX##_Bindable.sampler_states.size(), ##D3D11_PREFIX##_Bindable.sampler_states.data());  \
    }

    if (b_is_shader_bound[0])
        SET_RESOURCE(TYPE_VERTEX, VS)
    if (b_is_shader_bound[1])
        SET_RESOURCE(TYPE_PIXEL, PS)
    if (b_is_shader_bound[2])
        SET_RESOURCE(TYPE_GEOMETRY, GS)
    if (b_is_shader_bound[3])
        SET_RESOURCE(TYPE_HULL, HS)
    if (b_is_shader_bound[4])
        SET_RESOURCE(TYPE_DOMAIN, DS)
    if (b_is_shader_bound[5])
        SET_RESOURCE(TYPE_COMPUTE, CS)
}

void DeviceContextD3D11Impl::Draw(uint32_t num_vertices)
{
}

void DeviceContextD3D11Impl::DrawIndexed(uint32_t num_indicies)
{
    m_d3d11_device_context_ptr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_d3d11_device_context_ptr->DrawIndexed(num_indicies, 0, 0);
}

void DeviceContextD3D11Impl::Map(PPGEBuffer *p_buffer, MapType map_type, MapFlags map_flags, void **pp_map_data)
{
    const auto &desc = p_buffer->GetDesc();
    PPGE_ASSERT(
        !Enum::ToBoolean(desc.cpu_access_flags & (CPUAccessFlags::CPU_ACCESS_READ & CPUAccessFlags::CPU_ACCESS_WRITE)),
        "Buffer that will be mapped should be created with cpu access flag read, write or read-write.");
#ifdef PPGE_DEBUG
    switch (map_type)
    {
    case PPGE::MapType::MAP_READ:
        PPGE_ASSERT(Enum::ToBoolean(desc.cpu_access_flags & CPUAccessFlags::CPU_ACCESS_READ),
                    "Cannot create buffer map with read type. Buffer hasn't been created with CPU read access.");
        break;
    case PPGE::MapType::MAP_WRITE:
        PPGE_ASSERT(Enum::ToBoolean(desc.cpu_access_flags & CPUAccessFlags::CPU_ACCESS_WRITE),
                    "Cannot create buffer map with write type. Buffer hasn't been created with CPU write access.");
        if (Enum::ToBoolean(map_flags & MapFlags::MAP_DISCARD))
        {
            PPGE_ASSERT(desc.usage == UsageType::USAGE_DYNAMIC,
                        "Map with type write and flag discard cannot be created. Buffer hasn't been created with "
                        "dynamic usage type.");
        }
        else if (Enum::ToBoolean(map_flags & MapFlags::MAP_NO_OVERWRITE))
        {
            PPGE_ASSERT(!Enum::ToBoolean(desc.bind_flags & BindFlags::BIND_CONSTANT_BUFFER) &&
                            (Enum::ToBoolean(desc.bind_flags & BindFlags::BIND_VERTEX_BUFFER) ||
                             Enum::ToBoolean(desc.bind_flags & BindFlags::BIND_INDEX_BUFFER)),
                        "Map with type write and flag no overwrite canno be created. Buffer has to be bound as vertex "
                        "and/or index buffer and no constant buffer.");
        }
        break;
    case PPGE::MapType::MAP_READ_WRITE:
        PPGE_ASSERT(Enum::ToBoolean(desc.cpu_access_flags &
                                    (CPUAccessFlags::CPU_ACCESS_READ & CPUAccessFlags::CPU_ACCESS_WRITE)),
                    "Cannot create buffer map with read and write type. Buffer hasn't been created with CPU read and "
                    "write access.");
        break;
    default:
        break;
    }
#endif
    BufferD3D11Impl *p_d3d11_buffer = static_cast<BufferD3D11Impl *>(p_buffer);
    D3D11_MAP d3d11_map_type = PPGEMapTypeToD3D11MapType(map_type, map_flags);
    UINT d3d11_map_flags = PPGEMapFlagsToD3D11MapFlags(map_flags);
    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    HRESULT hr = m_d3d11_device_context_ptr->Map(p_d3d11_buffer->GetD3D11Buffer(), 0, d3d11_map_type, d3d11_map_flags,
                                                 &mapped_resource);
    *pp_map_data = SUCCEEDED(hr) ? mapped_resource.pData : nullptr;
}

void DeviceContextD3D11Impl::Unmap(PPGEBuffer *p_buffer)
{
    BufferD3D11Impl *p_d3d11_buffer = static_cast<BufferD3D11Impl *>(p_buffer);
    m_d3d11_device_context_ptr->Unmap(p_d3d11_buffer->GetD3D11Buffer(), 0);
}
} // namespace PPGE