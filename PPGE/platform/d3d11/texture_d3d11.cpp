#include "texture_d3d11.h"

#include "platform/d3d11/renderer_system_d3d11.h"

namespace PPGE
{
TextureD3D11::TextureD3D11() : m_renderer(RendererSystem::GetRendererSystem<RendererSystemD3D11>())
{
}

TextureD3D11::~TextureD3D11()
{
    Destroy();
}

void TextureD3D11::Destroy()
{
}

bool TextureD3D11::Create(const Texture2DDesc &desc)
{
    return false;
}

void TextureD3D11::Set()
{
}
} // namespace PPGE