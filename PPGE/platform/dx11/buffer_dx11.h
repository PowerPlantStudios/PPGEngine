#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"
#include "renderer/buffer.h"

namespace PPGE
{
class RendererSystemDX11;

class PPGE_API BufferD3D11
{
    BufferD3D11(RendererSystemDX11 &renderer_ref, const BufferDesc &desc);

    virtual void CreateBuffer();
    virtual void UpdateBuffer();
    virtual void SetBuffer();

  protected:
    RendererSystemDX11 &m_renderer_ref;
    ID3D11Buffer *m_buffer;
};

class PPGE_API IndexBufferD3D11 : public BufferD3D11
{
};

class PPGE_API VertexBufferD3D11 : public BufferD3D11
{
    VertexBufferD3D11(RendererSystemDX11 &renderer, const VertexBufferDesc &desc);

    protected:
};
} // namespace PPGE
