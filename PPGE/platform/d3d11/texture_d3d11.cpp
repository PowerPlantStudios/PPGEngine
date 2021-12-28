#include "texture_d3d11.h"

#include "platform/d3d11/renderer_system_d3d11.h"

namespace PPGE
{
TextureD3D11::TextureD3D11()
    : m_renderer(RendererSystem::GetRendererSystem<RendererSystemD3D11>()), m_texture(NULL), m_srv(NULL)
{
}

TextureD3D11::~TextureD3D11()
{
    Destroy();
}

void TextureD3D11::Destroy()
{
    PPGE_RELEASE_COM(m_texture);
    PPGE_RELEASE_COM(m_srv);
}

bool TextureD3D11::Create(const TextureDesc &desc)
{
    return false;
}

bool TextureD3D11::Create(const Texture2DDesc &desc)
{
    return false;
}

bool TextureD3D11::Create(const Texture3DDesc &desc)
{
    return false;
}

bool TextureD3D11::Create(const TextureResurceDesc &desc)
{
    switch (desc.m_format)
    {
    case PPGE::TextureResurceDesc::FileFormat::DDS:
        PPGE_HR(DirectX::CreateDDSTextureFromFile(m_renderer->m_device, m_renderer->m_immediate_context,
                                                  desc.m_path.c_str(), &m_texture, &m_srv));
        break;
    case PPGE::TextureResurceDesc::FileFormat::TGA:
    case PPGE::TextureResurceDesc::FileFormat::HDR:
    case PPGE::TextureResurceDesc::FileFormat::BMP:
    case PPGE::TextureResurceDesc::FileFormat::JPEG:
    case PPGE::TextureResurceDesc::FileFormat::PNG:
    case PPGE::TextureResurceDesc::FileFormat::TIFF:
        PPGE_HR(DirectX::CreateWICTextureFromFile(m_renderer->m_device, m_renderer->m_immediate_context,
                                                  desc.m_path.c_str(), &m_texture, &m_srv));
        break;
    default:
        PPGE_ASSERT(false, "Unknown file format for texture creation.");
        return false;
    }
    return m_texture != NULL && m_srv != NULL;
}

void TextureD3D11::Set(ShaderResourceTarget target, uint8_t slot)
{
    PPGE_ASSERT(m_renderer, "Binding D3D11 SRV to the pipeline has failed. RendererSystemD3D11 is inaccessible.");

    UINT number_of_buffer = 1;
    switch (target)
    {
    case PPGE::ShaderResourceTarget::VS:
        m_renderer->m_immediate_context->VSSetShaderResources(slot, number_of_buffer, &m_srv);
        break;
    case PPGE::ShaderResourceTarget::HS:
        m_renderer->m_immediate_context->HSSetShaderResources(slot, number_of_buffer, &m_srv);
        break;
    case PPGE::ShaderResourceTarget::DS:
        m_renderer->m_immediate_context->DSSetShaderResources(slot, number_of_buffer, &m_srv);
        break;
    case PPGE::ShaderResourceTarget::GS:
        m_renderer->m_immediate_context->GSSetShaderResources(slot, number_of_buffer, &m_srv);
        break;
    case PPGE::ShaderResourceTarget::PS:
        m_renderer->m_immediate_context->PSSetShaderResources(slot, number_of_buffer, &m_srv);
        break;
    default:
        PPGE_ASSERT(false, "Unknown shader type for setting shader resource into the pipeline.");
        break;
    }
}
} // namespace PPGE