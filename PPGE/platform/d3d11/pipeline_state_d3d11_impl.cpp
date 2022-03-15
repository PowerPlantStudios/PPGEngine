#include "pipeline_state_d3d11_impl.h"

#include "platform/d3d11/device_d3d11_impl.h"
#include "platform/d3d11/shader_d3d11_impl.h"
#include "platform/d3d11/shader_resource_binding_d3d11_impl.h"

namespace PPGE
{
PipelineStateD3D11Impl::PipelineStateD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp,
                                               const GfxPipelineStateCreateDesc &desc)
    : PipelineStateBaseType(std::move(device_sp), desc)
{
    for (size_t i = 0; i < desc.srv_num; i++)
    {
        ShaderResourceCreateDesc cd = desc.srv[i];
        m_bindable_resource_descs.push_back(std::make_tuple(std::string(cd.name), cd.desc));
    }

#define PUSH_SHADER_BACK(ShaderType, Idx)                                                                              \
    if (desc.commited_##ShaderType)                                                                                    \
    {                                                                                                                  \
        m_active_shader_indicies[Idx] = m_shaders.size();                                                              \
        m_shaders.push_back(std::move(std::static_pointer_cast<PPGEShaderD3D11>(desc.commited_##ShaderType)));         \
    }

    PUSH_SHADER_BACK(vs, 0);
    PUSH_SHADER_BACK(ps, 1);
    PUSH_SHADER_BACK(gs, 2);
    PUSH_SHADER_BACK(hs, 3);
    PUSH_SHADER_BACK(ds, 4);

    auto vs_index = m_active_shader_indicies[0];
    if (vs_index > -1 && vs_index < m_shaders.size())
    {
        if (auto vs_sp = m_shaders[vs_index])
        {
            auto vs_code = vs_sp->GetByteCode();
            auto d3d11_device = m_device_sp->GetD3D11Device();
            /* TEMPORARY */
            D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
                {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}};
            PPGE_HR(d3d11_device->CreateInputLayout(vertexDesc, 3, vs_code->GetBufferPointer(),
                                                    vs_code->GetBufferSize(), &m_input_layout_ptr));
        }
    }
}

PipelineStateD3D11Impl::~PipelineStateD3D11Impl()
{
}

ID3D11InputLayout *PipelineStateD3D11Impl::GetD3D11InputLayout() const
{
    return m_input_layout_ptr;
}

ID3D11VertexShader *PipelineStateD3D11Impl::GetD3D11VertexShader() const
{
    ID3D11VertexShader *d3d11_vs_shader = nullptr;
    auto vs_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_VERTEX);
    if (vs_index > -1 && vs_index < m_shaders.size())
    {
        if (auto vs_sp = m_shaders[vs_index])
        {
            auto d3d11_shader_ptr = vs_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11VertexShader),
                                                     reinterpret_cast<void **>(&d3d11_vs_shader)));
        }
    }
    return d3d11_vs_shader;
}

ID3D11PixelShader *PipelineStateD3D11Impl::GetD3D11PixelShader() const
{
    ID3D11PixelShader *d3d11_ps_shader = nullptr;
    auto ps_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_PIXEL);
    if (ps_index > -1 && ps_index < m_shaders.size())
    {
        if (auto ps_sp = m_shaders[ps_index])
        {
            auto d3d11_shader_ptr = ps_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11PixelShader),
                                                     reinterpret_cast<void **>(&d3d11_ps_shader)));
        }
    }
    return d3d11_ps_shader;
}

ID3D11GeometryShader *PipelineStateD3D11Impl::GetD3D11GeometryShader() const
{
    ID3D11GeometryShader *d3d11_gs_shader = nullptr;
    auto gs_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_GEOMETRY);
    if (gs_index > -1 && gs_index < m_shaders.size())
    {
        if (auto gs_sp = m_shaders[gs_index])
        {
            auto d3d11_shader_ptr = gs_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11GeometryShader),
                                                     reinterpret_cast<void **>(&d3d11_gs_shader)));
        }
    }
    return d3d11_gs_shader;
}

ID3D11DomainShader *PipelineStateD3D11Impl::GetD3D11DomainShader() const
{
    ID3D11DomainShader *d3d11_ds_shader = nullptr;
    auto ds_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_DOMAIN);
    if (ds_index > -1 && ds_index < m_shaders.size())
    {
        if (auto ds_sp = m_shaders[ds_index])
        {
            auto d3d11_shader_ptr = ds_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11DomainShader),
                                                     reinterpret_cast<void **>(&d3d11_ds_shader)));
        }
    }
    return d3d11_ds_shader;
}

ID3D11HullShader *PipelineStateD3D11Impl::GetD3D11HullShader() const
{
    ID3D11HullShader *d3d11_hs_shader = nullptr;
    auto hs_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_HULL);
    if (hs_index > -1 && hs_index < m_shaders.size())
    {
        if (auto hs_sp = m_shaders[hs_index])
        {
            auto d3d11_shader_ptr = hs_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11HullShader),
                                                     reinterpret_cast<void **>(&d3d11_hs_shader)));
        }
    }
    return d3d11_hs_shader;
}

ID3D11ComputeShader *PipelineStateD3D11Impl::GetD3D11ComputeShader() const
{
    ID3D11ComputeShader *d3d11_cs_shader = nullptr;
    auto cs_index = GetPipelineShaderIndex(ShaderTypeFlags::SHADER_TYPE_COMPUTE);
    if (cs_index > -1 && cs_index < m_shaders.size())
    {
        if (auto cs_sp = m_shaders[cs_index])
        {
            auto d3d11_shader_ptr = cs_sp->GetD3D11Shader();
            PPGE_HR(d3d11_shader_ptr->QueryInterface(__uuidof(ID3D11ComputeShader),
                                                     reinterpret_cast<void **>(&d3d11_cs_shader)));
        }
    }
    return d3d11_cs_shader;
}

void PipelineStateD3D11Impl::CreateShaderResourceBinding(
    std::shared_ptr<PPGEShaderResourceBinding> &pp_shader_resource_binding)
{
    pp_shader_resource_binding =
        std::make_shared<ShaderResourceBindingD3D11Impl>(shared_from_this(), m_bindable_resource_descs);
}
} // namespace PPGE