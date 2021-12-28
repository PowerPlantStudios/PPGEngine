#include "sampler_d3d11.h"

#include "platform/d3d11/renderer_system_d3d11.h"

namespace PPGE
{
SamplerD3D11::SamplerD3D11()
    : m_renderer(RendererSystem::GetRendererSystem<RendererSystemD3D11>()), m_sampler_state(NULL)
{
}

SamplerD3D11::~SamplerD3D11()
{
    Destroy();
}

void SamplerD3D11::Destroy()
{
    PPGE_RELEASE_COM(m_sampler_state);
}

bool SamplerD3D11::Create(const SamplerDesc &desc)
{
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 8;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    PPGE_HR(m_renderer->m_device->CreateSamplerState(&samplerDesc, &m_sampler_state));

    return m_sampler_state != NULL;
}

void SamplerD3D11::Set(ShaderResourceTarget target, uint8_t slot)
{
    PPGE_ASSERT(m_renderer, "Binding D3D11 SRV to the pipeline has failed. RendererSystemD3D11 is inaccessible.");

    UINT number_of_buffer = 1;
    switch (target)
    {
    case PPGE::ShaderResourceTarget::VS:
        m_renderer->m_immediate_context->VSSetSamplers(slot, number_of_buffer, &m_sampler_state);
        break;
    case PPGE::ShaderResourceTarget::HS:
        m_renderer->m_immediate_context->HSSetSamplers(slot, number_of_buffer, &m_sampler_state);
        break;
    case PPGE::ShaderResourceTarget::DS:
        m_renderer->m_immediate_context->DSSetSamplers(slot, number_of_buffer, &m_sampler_state);
        break;
    case PPGE::ShaderResourceTarget::GS:
        m_renderer->m_immediate_context->GSSetSamplers(slot, number_of_buffer, &m_sampler_state);
        break;
    case PPGE::ShaderResourceTarget::PS:
        m_renderer->m_immediate_context->PSSetSamplers(slot, number_of_buffer, &m_sampler_state);
        break;
    default:
        PPGE_ASSERT(false, "Unknown shader type for setting texture sampler into the pipeline.");
        break;
    }
}
} // namespace PPGE
