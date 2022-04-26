#include "device_context_d3d11_impl.h"

#include "platform/d3d11/buffer_d3d11_impl.h"
#include "platform/d3d11/pipeline_state_d3d11_impl.h"
#include "platform/d3d11/renderer_types_d3d11_conversion.h"
#include "platform/d3d11/shader_resource_binding_d3d11_impl.h"
#include "platform/d3d11/texture_view_d3d11_impl.h"

namespace PPGE
{
static std::array<ID3D11Buffer *, Num_Of_Buffer_Slots> NULL_VERTEX_BUFFERS{nullptr};

void DeviceContextD3D11Impl::SetVertexBuffers(uint32_t num_buffers, std::shared_ptr<PPGEBuffer> p_buffers[],
                                              const uint64_t offsets[])
{
    if (m_bound_vertex_buffer_count > num_buffers)
    {
        m_d3d11_device_context_ptr->IASetVertexBuffers(num_buffers, (m_bound_vertex_buffer_count - num_buffers),
                                                       NULL_VERTEX_BUFFERS.data(), NULL, NULL);
        for (uint64_t i = num_buffers; i < m_bound_vertex_buffer_count; i++)
        {
            m_bound_vertex_buffers_sp[i].reset();
            m_bound_d3d11_vertex_buffers_ptr[i] = nullptr;
            m_bound_d3d11_vertex_buffer_strides[i] = 0;
            m_bound_d3d11_vertex_buffer_offsets[i] = 0;
        }
    }

    m_bound_vertex_buffer_count = num_buffers > Num_Of_Buffer_Slots ? Num_Of_Buffer_Slots : num_buffers;
    for (uint64_t i = 0; i < m_bound_vertex_buffer_count; i++)
    {
        m_bound_vertex_buffers_sp[i] = std::static_pointer_cast<BufferD3D11Impl>(std::move(p_buffers[i]));
        m_bound_d3d11_vertex_buffers_ptr[i] = m_bound_vertex_buffers_sp[i]->GetD3D11BufferRaw();
        m_bound_d3d11_vertex_buffer_strides[i] = m_bound_pipeline_state_sp->GetStride(i);
        m_bound_d3d11_vertex_buffer_offsets[i] = static_cast<UINT>(offsets[i]);
    }

    m_d3d11_device_context_ptr->IASetVertexBuffers(
        0, m_bound_vertex_buffer_count, m_bound_d3d11_vertex_buffers_ptr.data(),
        m_bound_d3d11_vertex_buffer_strides.data(), m_bound_d3d11_vertex_buffer_offsets.data());
}

void DeviceContextD3D11Impl::SetIndexBuffer(std::shared_ptr<PPGEBuffer> p_buffer)
{
    if (m_bound_index_buffer_sp.get() == p_buffer.get())
        return;

    m_bound_index_buffer_sp = std::static_pointer_cast<BufferD3D11Impl>(std::move(p_buffer));
    m_bound_d3d11_index_buffer_ptr = m_bound_index_buffer_sp->GetD3D11BufferShared();
    m_d3d11_device_context_ptr->IASetIndexBuffer(m_bound_d3d11_index_buffer_ptr, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContextD3D11Impl::SetPipelineStateObject(std::shared_ptr<PPGEPipelineState> p_PSO)
{
    if (m_bound_pipeline_state_sp.get() == p_PSO.get())
        return;

    static ID3D11Buffer *nullCB[] = {nullptr};
    static ID3D11ShaderResourceView *nullSRV[] = {nullptr};
    static ID3D11SamplerState *nullSamplerState[] = {nullptr};

#define UNBIND_SHADER_RESOURCES(SHADER_NAME_SHORT)                                                                     \
    m_d3d11_device_context_ptr->##SHADER_NAME_SHORT##SetConstantBuffers(0, 1, nullCB);                                 \
    m_d3d11_device_context_ptr->##SHADER_NAME_SHORT##SetShaderResources(0, 1, nullSRV);                                \
    m_d3d11_device_context_ptr->##SHADER_NAME_SHORT##SetSamplers(0, 1, nullSamplerState);

    UNBIND_SHADER_RESOURCES(VS);
    UNBIND_SHADER_RESOURCES(PS);
    UNBIND_SHADER_RESOURCES(GS);
    UNBIND_SHADER_RESOURCES(HS);
    UNBIND_SHADER_RESOURCES(DS);
    UNBIND_SHADER_RESOURCES(CS);

    b_is_shader_bound.fill(false);
    m_bound_pipeline_state_sp = std::static_pointer_cast<PipelineStateD3D11Impl>(std::move(p_PSO));

#define BIND_SHADER(SHADER_NAME, SHADER_NAME_SHORT, SHADER_IDX)                                                        \
    auto d3d11_##SHADER_NAME_SHORT##_shader = m_bound_pipeline_state_sp->GetD3D11##SHADER_NAME##Shared();              \
    if (d3d11_##SHADER_NAME_SHORT##_shader)                                                                            \
    {                                                                                                                  \
        b_is_shader_bound[##SHADER_IDX##] = true;                                                                      \
        if (m_bound_d3d11_shaders_ptr[##SHADER_IDX##] != d3d11_##SHADER_NAME_SHORT##_shader)                           \
        {                                                                                                              \
            m_bound_d3d11_shaders_ptr[##SHADER_IDX##] = d3d11_##SHADER_NAME_SHORT##_shader;                            \
            m_d3d11_device_context_ptr->##SHADER_NAME_SHORT##SetShader(d3d11_##SHADER_NAME_SHORT##_shader, nullptr,    \
                                                                       0);                                             \
        }                                                                                                              \
    }

    BIND_SHADER(VertexShader, VS, 0);
    BIND_SHADER(PixelShader, PS, 1);
    BIND_SHADER(GeometryShader, GS, 2);
    BIND_SHADER(HullShader, HS, 3);
    BIND_SHADER(DomainShader, DS, 4);
    BIND_SHADER(ComputeShader, CS, 5);
#undef BIND_SHADER

    m_bound_pipeline_state_sp->GetD3D11BlendStateRaw();
    m_d3d11_device_context_ptr->RSSetState(m_bound_pipeline_state_sp->GetD3D11RasterizerStateRaw());
    m_d3d11_device_context_ptr->OMSetBlendState(m_bound_pipeline_state_sp->GetD3D11BlendStateRaw(), nullptr,
                                                0xffffffff);
    m_d3d11_device_context_ptr->OMSetDepthStencilState(m_bound_pipeline_state_sp->GetD3D11DepthStencilStateRaw(), 0);

    auto d3d11_input_layout = m_bound_pipeline_state_sp->GetD3D11InputLayoutRaw();
    if (m_input_layout_ptr != d3d11_input_layout)
    {
        m_input_layout_ptr = d3d11_input_layout;
        m_d3d11_device_context_ptr->IASetInputLayout(m_input_layout_ptr);
    }

    auto primitive_topology = PPGEPrimitiveTopologyTypeToD3D11PrimitiveTopology(
        m_bound_pipeline_state_sp->GetGraphicsPipelineStateDesc().primitive_topology);
    if (m_bound_primitive_topology != primitive_topology)
    {
        m_bound_primitive_topology = primitive_topology;
        m_d3d11_device_context_ptr->IASetPrimitiveTopology(m_bound_primitive_topology);
    }
}

void DeviceContextD3D11Impl::CommitShaderResources(const std::shared_ptr<PPGEShaderResourceBinding> &p_SRB)
{
    auto d3d11_srb = static_cast<PPGEShaderResourceBindingD3D11 *>(p_SRB.get());
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

#undef SET_RESOURCE
}

void DeviceContextD3D11Impl::SetViewports(uint32_t num_viewports, const Viewport viewports[])
{
}

void DeviceContextD3D11Impl::SetScissorRects(uint32_t num_rects, const Rect rects[])
{
}

void DeviceContextD3D11Impl::SetRenderTargets(uint32_t num_render_targets, std::shared_ptr<PPGETextureView> pp_RTs[],
                                              std::shared_ptr<PPGETextureView> p_DS)
{

    if (m_depth_stencil_buffer_sp.get() != p_DS.get())
        m_depth_stencil_buffer_sp = std::static_pointer_cast<TextureViewD3D11Impl>(std::move(p_DS));

    if (m_bound_render_target_count > num_render_targets)
    {
        for (uint32_t i = num_render_targets; i < m_bound_render_target_count; i++)
        {
            m_render_targets_sp[i].reset();
        }
    }
    m_bound_render_target_count =
        num_render_targets > Num_Of_Render_Targets ? Num_Of_Render_Targets : num_render_targets;
    for (uint32_t i = 0; i < m_bound_render_target_count; i++)
    {
        if (m_render_targets_sp[i].get() != pp_RTs[i].get())
            m_render_targets_sp[i] = std::static_pointer_cast<TextureViewD3D11Impl>(std::move(pp_RTs[i]));
    }

    ID3D11DepthStencilView *d3d11_dsv_ptr = nullptr;
    {
        if (m_depth_stencil_buffer_sp)
        {
            auto d3d11_view_ptr = static_cast<PPGETextureViewD3D11 *>(m_depth_stencil_buffer_sp.get())->GetD3D11View();
            PPGE_HR(d3d11_view_ptr.QueryInterface<ID3D11DepthStencilView>(&d3d11_dsv_ptr));
        }
    }

    ID3D11RenderTargetView *d3d11_rtvs[Num_Of_Render_Targets] = {};
    for (uint32_t i = 0; i < Num_Of_Render_Targets; i++)
    {
        if (m_render_targets_sp[i])
        {
            auto d3d11_view_ptr = static_cast<PPGETextureViewD3D11 *>(m_render_targets_sp[i].get())->GetD3D11View();
            PPGE_HR(d3d11_view_ptr.QueryInterface<ID3D11RenderTargetView>(&d3d11_rtvs[i]));
        }
    }

    m_d3d11_device_context_ptr->OMSetRenderTargets(Num_Of_Render_Targets, d3d11_rtvs ? d3d11_rtvs : nullptr,
                                                   d3d11_dsv_ptr ? d3d11_dsv_ptr : nullptr);

    if (d3d11_dsv_ptr)
        d3d11_dsv_ptr->Release();

    for (uint32_t i = 0; i < Num_Of_Render_Targets; i++)
    {
        if (d3d11_rtvs[i])
        {
            d3d11_rtvs[i]->Release();
        }
    }
}

void DeviceContextD3D11Impl::ClearRenderTarget(const std::shared_ptr<PPGETextureView> &p_view, float r, float g,
                                               float b, float a)
{
    auto d3d11_view_ptr = static_cast<PPGETextureViewD3D11 *>(p_view.get())->GetD3D11View();
    CComPtr<ID3D11RenderTargetView> d3d11_rtv_ptr;
    PPGE_HR(d3d11_view_ptr.QueryInterface<ID3D11RenderTargetView>(&d3d11_rtv_ptr));
    float color[] = {r, g, b, a};
    m_d3d11_device_context_ptr->ClearRenderTargetView(d3d11_rtv_ptr, color);
}

void DeviceContextD3D11Impl::ClearDepthStencil(const std::shared_ptr<PPGETextureView> &p_view, float d, uint8_t s)
{
    auto d3d11_view_ptr = static_cast<PPGETextureViewD3D11 *>(p_view.get())->GetD3D11View();
    CComPtr<ID3D11DepthStencilView> d3d11_dsv_ptr;
    PPGE_HR(d3d11_view_ptr.QueryInterface<ID3D11DepthStencilView>(&d3d11_dsv_ptr));
    UINT clear_flags = D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH;
    m_d3d11_device_context_ptr->ClearDepthStencilView(d3d11_dsv_ptr, clear_flags, d, s);
}

void DeviceContextD3D11Impl::Draw(uint32_t num_vertices)
{
    m_d3d11_device_context_ptr->Draw(num_vertices, 0);
}

void DeviceContextD3D11Impl::DrawIndirect(uint32_t num_vertices)
{
}

void DeviceContextD3D11Impl::DrawIndexed(uint32_t num_indicies)
{
    m_d3d11_device_context_ptr->IASetPrimitiveTopology(m_bound_primitive_topology);
    m_d3d11_device_context_ptr->DrawIndexed(num_indicies, 0, 0);
}

void DeviceContextD3D11Impl::DrawIndexedIndirect(uint32_t num_indicies)
{
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
    PPGEBufferD3D11 *p_d3d11_buffer = static_cast<PPGEBufferD3D11 *>(p_buffer);
    D3D11_MAP d3d11_map_type = PPGEMapTypeToD3D11MapType(map_type, map_flags);
    UINT d3d11_map_flags = PPGEMapFlagsToD3D11MapFlags(map_flags);
    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    HRESULT hr = m_d3d11_device_context_ptr->Map(p_d3d11_buffer->GetD3D11BufferRaw(), 0, d3d11_map_type,
                                                 d3d11_map_flags, &mapped_resource);
    *pp_map_data = SUCCEEDED(hr) ? mapped_resource.pData : nullptr;
}

void DeviceContextD3D11Impl::Unmap(PPGEBuffer *p_buffer)
{
    PPGEBufferD3D11 *p_d3d11_buffer = static_cast<PPGEBufferD3D11 *>(p_buffer);
    m_d3d11_device_context_ptr->Unmap(p_d3d11_buffer->GetD3D11BufferRaw(), 0);
}

void DeviceContextD3D11Impl::Flush()
{
    m_d3d11_device_context_ptr->Flush();
}
} // namespace PPGE