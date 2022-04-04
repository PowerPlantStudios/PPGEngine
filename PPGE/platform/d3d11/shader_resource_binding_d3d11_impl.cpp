#include "shader_resource_binding_d3d11_impl.h"

#include "platform/d3d11/pipeline_state_d3d11_impl.h"
#include "platform/d3d11/shader_resource_variable_d3d11_impl.h"
#include "rhi/shader.h"
#include "system/logger_system.h"

namespace PPGE
{
ShaderResourceBindingD3D11Impl::ShaderResourceBindingD3D11Impl(
    std::shared_ptr<PipelineStateD3D11Impl> PSO_sp,
    const std::vector<std::tuple<std::string, ShaderResourceDesc>> &bindable_resources)
    : ShaderResourceBindingBaseType(std::move(PSO_sp))
{
    m_SRBs.resize(m_PSO_sp->GetNumActiveShaders());
    m_bound_resources.resize(m_PSO_sp->GetNumActiveShaders());

    for (auto &[name, desc] : bindable_resources)
    {
        auto shader_idx = m_PSO_sp->GetPipelineShaderIndex(desc.shader_types);
        auto &SRB = m_SRBs[shader_idx];

        SRB.SRB_indicies[name] = SRB.SRBs.size();

        std::unique_ptr<PPGEShaderResourceVariableD3D11> SRV_up;

        switch (desc.resource_type)
        {
        case PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER:
            SRV_up = std::make_unique<ConstantBufferBindingD3D11>(desc);
            break;
        case PPGE::ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV:
            SRV_up = std::make_unique<TextureSRVBindingD3D11>(desc);
            break;
        case PPGE::ShaderResourceType::SHADER_RESOURCE_SAMPLER:
            SRV_up = std::make_unique<SamplerBindingD3D11>(desc);
            break;
        case PPGE::ShaderResourceType::SHADER_RESOURCE_UNKNOWN:
            PPGE_ASSERT(false, "Creating shader resource variable has failed: Reource type is not set.");
            break;
        default:
            PPGE_ASSERT(false, "Unkown shader resource type to create shader resource variable.");
            break;
        }

        if (SRV_up)
            SRB.SRBs.push_back(std::move(SRV_up));
    }
}

PPGEShaderResourceVariable *ShaderResourceBindingD3D11Impl::GetVariableByName(const char *name,
                                                                              ShaderTypeFlags shader_type)
{
    auto shader_idx = m_PSO_sp->GetPipelineShaderIndex(shader_type);
    PPGE_ASSERT(shader_idx > -1 && shader_idx < m_PSO_sp->GetNumActiveShaders(),
                "Given shader type is not active in the owner pipeline of shader resource binding.");
    auto &SRB = m_SRBs[shader_idx];
    auto it = SRB.SRB_indicies.find(name);
    if (it != SRB.SRB_indicies.end())
    {
        PPGE_ASSERT(it->second < SRB.SRBs.size(), "SRV index of {0} in {1} is out of bounds.", name,
                    ShaderTypeToString(shader_type));
        b_is_bound_res_dirty[shader_idx] = true;
        return SRB.SRBs[it->second].get();
    }
    return nullptr;
}

PPGEShaderResourceVariable *ShaderResourceBindingD3D11Impl::GetVariableByIndex(uint32_t index,
                                                                               ShaderTypeFlags shader_type)
{
    auto shader_idx = m_PSO_sp->GetPipelineShaderIndex(shader_type);
    PPGE_ASSERT(shader_idx > -1 && shader_idx < m_PSO_sp->GetNumActiveShaders(),
                "Given shader type is not active in the owner pipeline of shader resource binding.");
    auto &SRB = m_SRBs[shader_idx];
    PPGE_ASSERT(index < SRB.SRBs.size(), "SRV index {0} in {1} is out of bounds.", index,
                ShaderTypeToString(shader_type));
    if (index < SRB.SRBs.size())
    {
        b_is_bound_res_dirty[shader_idx] = true;
        return SRB.SRBs[index].get();
    }
    return nullptr;
}

const ShaderResourceBindingD3D11Impl::D3D11Bindables &ShaderResourceBindingD3D11Impl::GetD3D11Bindables(
    ShaderTypeFlags shader_type)
{
    auto shader_idx = m_PSO_sp->GetPipelineShaderIndex(shader_type);
    PPGE_ASSERT(shader_idx > -1 && shader_idx < m_PSO_sp->GetNumActiveShaders(),
                "Given shader type is not active in the owner pipeline of shader resource binding.");
    if (b_is_bound_res_dirty[shader_idx])
        RefreshBoundResources(shader_type);
    return m_bound_resources[shader_idx];
}

void ShaderResourceBindingD3D11Impl::RefreshBoundResources(ShaderTypeFlags shader_type)
{
    auto shader_idx = m_PSO_sp->GetPipelineShaderIndex(shader_type);
    PPGE_ASSERT(shader_idx > -1 && shader_idx < m_PSO_sp->GetNumActiveShaders(),
                "Given shader type is not active in the owner pipeline of shader resource binding.");
    auto &SRB = m_SRBs[shader_idx];

    auto &bound_resource = m_bound_resources[shader_idx];
    bound_resource.constant_buffers.clear();
    bound_resource.shader_resource_views.clear();
    bound_resource.sampler_states.clear();

    for (auto &SRV : SRB.SRBs)
    {
        auto d3d11_resource = SRV->GetD3D11Resource();
        switch (SRV->GetDesc().resource_type)
        {
        case PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER: {
            ID3D11Buffer *d3d11_constant_buffer = nullptr;
            PPGE_HR(d3d11_resource->QueryInterface(__uuidof(ID3D11Buffer),
                                                   reinterpret_cast<void **>(&d3d11_constant_buffer)));
            if (d3d11_constant_buffer)
                bound_resource.constant_buffers.push_back(d3d11_constant_buffer);
            break;
        }
        case PPGE::ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV: {
            ID3D11ShaderResourceView *d3d11_srv = nullptr;
            PPGE_HR(d3d11_resource->QueryInterface(__uuidof(ID3D11ShaderResourceView),
                                                   reinterpret_cast<void **>(&d3d11_srv)));
            if (d3d11_srv)
                bound_resource.shader_resource_views.push_back(d3d11_srv);
            break;
        }
        case PPGE::ShaderResourceType::SHADER_RESOURCE_SAMPLER: {
            ID3D11SamplerState *d3d11_sampler_state = nullptr;
            PPGE_HR(d3d11_resource->QueryInterface(__uuidof(ID3D11SamplerState),
                                                   reinterpret_cast<void **>(&d3d11_sampler_state)));
            if (d3d11_sampler_state)
                bound_resource.sampler_states.push_back(d3d11_sampler_state);
            break;
        }
        case PPGE::ShaderResourceType::SHADER_RESOURCE_UNKNOWN:
            PPGE_ASSERT(false, "Shader resource type is unset.");
            break;
        default:
            PPGE_ASSERT(false, "Unknown shader resource type.");
            break;
        }
    }

    b_is_bound_res_dirty[shader_idx] = false;
}

} // namespace PPGE