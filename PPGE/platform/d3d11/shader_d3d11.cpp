#include "shader_d3d11.h"

#include <d3dcompiler.h>

#include "platform/d3d11/renderer_system_d3d11.h"

namespace PPGE
{
static const char *GetShaderTarget(ShaderDesc::ShaderType type)
{
    switch (type)
    {
    case ShaderDesc::ShaderType::VS:
        return "vs_5_0";
    case ShaderDesc::ShaderType::PS:
        return "ps_5_0";
    case ShaderDesc::ShaderType::GS:
        return "gs_5_0";
    case ShaderDesc::ShaderType::DS:
        return "ds_5_0";
    case ShaderDesc::ShaderType::HS:
        return "hs_5_0";
    case ShaderDesc::ShaderType::Unknown:
    default:
        PPGE_ASSERT(false, "Unknown shader type for D3D11 shader compilation.");
        return "";
    }
}

ShaderD3D11::ShaderD3D11()
    : m_renderer(RendererSystem::GetRendererSystem<RendererSystemD3D11>()), m_buffer(NULL), m_shader(NULL),
      m_type(ShaderDesc::ShaderType::Unknown)
{
}

ShaderD3D11::~ShaderD3D11()
{
    Destroy();
}

void ShaderD3D11::Destroy()
{
    PPGE_RELEASE_COM(m_buffer);
    PPGE_RELEASE_COM(m_shader);
}

bool ShaderD3D11::Create(const ShaderDesc &desc)
{
    m_type = desc.m_type;

    ID3DBlob *error_message;
    DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    shaderFlags |= D3D10_SHADER_DEBUG;
    shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
    PPGE_HR(D3DCompile(desc.m_data, desc.m_size, NULL, NULL, NULL, "main", GetShaderTarget(m_type), shaderFlags, 0,
                       &m_buffer,
                       &error_message));
    if (error_message)
    {
        // Print shader compilation error message
        m_type = ShaderDesc::ShaderType::Unknown;
        return false;
    }

    switch (m_type)
    {
    case ShaderDesc::ShaderType::VS:
        PPGE_HR(m_renderer->m_device->CreateVertexShader(m_buffer->GetBufferPointer(), m_buffer->GetBufferSize(), NULL,
                                                         &m_vertex_shader));
        break;
    case ShaderDesc::ShaderType::PS:
        PPGE_HR(m_renderer->m_device->CreatePixelShader(m_buffer->GetBufferPointer(), m_buffer->GetBufferSize(), NULL,
                                                        &m_pixel_shader));
        break;
    case ShaderDesc::ShaderType::GS:
        PPGE_HR(m_renderer->m_device->CreateGeometryShader(m_buffer->GetBufferPointer(), m_buffer->GetBufferSize(),
                                                           NULL, &m_geometry_shader));
        break;
    case ShaderDesc::ShaderType::DS:
        PPGE_HR(m_renderer->m_device->CreateDomainShader(m_buffer->GetBufferPointer(), m_buffer->GetBufferSize(), NULL,
                                                         &m_domain_shader));
        break;
    case ShaderDesc::ShaderType::HS:
        PPGE_HR(m_renderer->m_device->CreateHullShader(m_buffer->GetBufferPointer(), m_buffer->GetBufferSize(), NULL,
                                                       &m_hull_shader));
        break;
    case ShaderDesc::ShaderType::Unknown:
    default:
        PPGE_ASSERT(false, "Unknown shader type for D3D11 shader creation.");
        m_type = ShaderDesc::ShaderType::Unknown;
        return false;
    }

    return m_shader != NULL;
}

void ShaderD3D11::Set()
{
    switch (m_type)
    {
    case ShaderDesc::ShaderType::VS:
        m_renderer->m_immediate_context->VSSetShader(m_vertex_shader, NULL, 0);
        break;
    case ShaderDesc::ShaderType::PS:
        m_renderer->m_immediate_context->PSSetShader(m_pixel_shader, NULL, 0);
        break;
    case ShaderDesc::ShaderType::GS:
        m_renderer->m_immediate_context->GSSetShader(m_geometry_shader, NULL, 0);
        break;
    case ShaderDesc::ShaderType::DS:
        m_renderer->m_immediate_context->DSSetShader(m_domain_shader, NULL, 0);
        break;
    case ShaderDesc::ShaderType::HS:
        m_renderer->m_immediate_context->HSSetShader(m_hull_shader, NULL, 0);
        break;
    case ShaderDesc::ShaderType::Unknown:
    default:
        PPGE_ASSERT(false, "Unknown shader type for D3D11 shader creation.");
    }
}

ProgramD3D11::ProgramD3D11() : m_vs_handle(ShaderHandle{}), m_ps_handle(ShaderHandle{})
{
}

ProgramD3D11::~ProgramD3D11()
{
    Destroy();
}

void ProgramD3D11::Destroy()
{
    m_vs_handle = ShaderHandle{};
    m_ps_handle = ShaderHandle{};
}

bool ProgramD3D11::Create(const ProgramDesc &desc)
{
    m_vs_handle = desc.m_vertex_shader_handle;
    m_ps_handle = desc.m_pixel_shader_handle;
    return true;
}
} // namespace PPGE