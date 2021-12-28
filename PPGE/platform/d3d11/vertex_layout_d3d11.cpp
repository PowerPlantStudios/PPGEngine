#include "vertex_layout_d3d11.h"

#include "platform/d3d11/renderer_system_d3d11.h"
#include "platform/d3d11/shader_d3d11.h"

namespace PPGE
{
static D3D11_INPUT_ELEMENT_DESC Element_Descs[VertexLayout::AttributeCount] = {};

static const char *GetSemanticName(VertexLayout::Attribute attribute)
{
    switch (attribute)
    {
    case VertexLayout::Attribute::Position:
        return "POSITION";
    case VertexLayout::Attribute::Normal:
        return "NORMAL";
    case VertexLayout::Attribute::Tangent:
        return "TANGENT";
    case VertexLayout::Attribute::Bitangent:
        return "BITANGENT";
    case VertexLayout::Attribute::Color0:
    case VertexLayout::Attribute::Color1:
    case VertexLayout::Attribute::Color2:
    case VertexLayout::Attribute::Color3:
        return "COLOR";
    case VertexLayout::Attribute::TexCoord0:
    case VertexLayout::Attribute::TexCoord1:
    case VertexLayout::Attribute::TexCoord2:
    case VertexLayout::Attribute::TexCoord3:
        return "TEXCOORD";
    default:
        PPGE_ASSERT(false, "Attribute type is unknown.");
        return "";
    }
}

UINT GetSemanticIndex(VertexLayout::Attribute attribute)
{
    switch (attribute)
    {
    case VertexLayout::Attribute::Position:
    case VertexLayout::Attribute::Normal:
    case VertexLayout::Attribute::Tangent:
    case VertexLayout::Attribute::Bitangent:
    case VertexLayout::Attribute::Color0:
    case VertexLayout::Attribute::TexCoord0:
        return 0;
    case VertexLayout::Attribute::Color1:
        return 1;
    case VertexLayout::Attribute::Color2:
        return 2;
    case VertexLayout::Attribute::Color3:
        return 3;
    case VertexLayout::Attribute::TexCoord1:
        return 1;
    case VertexLayout::Attribute::TexCoord2:
        return 2;
    case VertexLayout::Attribute::TexCoord3:
        return 3;
    default:
        PPGE_ASSERT(false, "Attribute type is unknown.");
        return 0;
    }
}

DXGI_FORMAT GetLayoutFormat(VertexLayout::Type type, unsigned int noe)
{
    switch (type)
    {
    case VertexLayout::Type::Uint8:
        if (noe == 1)
            return DXGI_FORMAT_R8_UNORM;
        else if (noe == 2)
            return DXGI_FORMAT_R8G8_UNORM;
        else if (noe == 3 || noe == 4)
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        else
        {
            PPGE_ASSERT(false, "Unknwon format.");
            return DXGI_FORMAT_UNKNOWN;
        }
    case VertexLayout::Type::Int16:
        if (noe == 1)
            return DXGI_FORMAT_R16_UNORM;
        else if (noe == 2)
            return DXGI_FORMAT_R16G16_SNORM;
        else if (noe == 3 || noe == 4)
            return DXGI_FORMAT_R16G16B16A16_UNORM;
        else
        {
            PPGE_ASSERT(false, "Unknwon format.");
            return DXGI_FORMAT_UNKNOWN;
        }
    case VertexLayout::Type::Float:
        if (noe == 1)
            return DXGI_FORMAT_R32_FLOAT;
        else if (noe == 2)
            return DXGI_FORMAT_R32G32_FLOAT;
        else if (noe == 3 || noe == 4)
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        else
        {
            PPGE_ASSERT(false, "Unknwon format.");
            return DXGI_FORMAT_UNKNOWN;
        }
    default:
        PPGE_ASSERT(false, "Unknown format.");
        return DXGI_FORMAT_UNKNOWN;
    }
}

VertexLayoutD3D11::VertexLayoutD3D11()
    : m_renderer(RendererSystem::GetRendererSystem<RendererSystemD3D11>()), m_layout(NULL), b_haslayout_desc(false),
      m_layout_desc(VertexLayout{})
{
}

VertexLayoutD3D11::~VertexLayoutD3D11()
{
    Destroy();
}

void VertexLayoutD3D11::SetLayout(const VertexLayout &layout)
{
    m_layout_desc = layout;
    b_haslayout_desc = true;
    m_layout = NULL;
}

void VertexLayoutD3D11::Destroy()
{
    m_layout_desc = VertexLayout();
    b_haslayout_desc = false;
    m_layout = NULL;
}

bool VertexLayoutD3D11::Create(const ShaderD3D11 &vs_shader)
{
    VertexLayout::Element e{0};
    UINT num_elements = 0;
    for (auto element_code : m_layout_desc)
    {
        if (element_code == 0)
            continue;
        e.m_code = element_code;
        D3D11_INPUT_ELEMENT_DESC &desc = Element_Descs[num_elements++];
        desc.SemanticName = GetSemanticName(e.m_fields.attribute);
        desc.SemanticIndex = GetSemanticIndex(e.m_fields.attribute);
        desc.Format = GetLayoutFormat(e.m_fields.type, e.m_fields.num);
        desc.InputSlot = 0;
        desc.AlignedByteOffset = e.m_fields.offset;
        desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        desc.InstanceDataStepRate = 0;
    }

    auto vs_shader_code = vs_shader.GetShaderBuffer();
    PPGE_ASSERT(vs_shader_code, "Buffer to VS code is invalid.");
    PPGE_HR(m_renderer->m_device->CreateInputLayout(Element_Descs, num_elements, vs_shader_code->GetBufferPointer(),
                                                    vs_shader_code->GetBufferSize(), &m_layout));
    return m_layout != NULL;
}

void VertexLayoutD3D11::Set()
{
    m_renderer->m_immediate_context->IASetInputLayout(m_layout);
}
} // namespace PPGE