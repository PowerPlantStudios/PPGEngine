#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/descriptors.h"
#include "renderer/vertex_layout.h"
#include "system/logger_system.h"

namespace PPGE
{
class RendererSystemDX11;
class ShaderD3D11;

class PPGE_API VertexLayoutD3D11
{
  public:
    VertexLayoutD3D11();
    
    ~VertexLayoutD3D11();

    void SetLayout(const VertexLayout& layout);
    void Destroy();
    bool Create(const ShaderD3D11 &vs_shader);
    void Set();

    inline bool HasLayoutDesc() const
    {
        return b_haslayout_desc;
    }
    inline bool IsCreated() const
    {
        return m_layout != NULL;
    }

  private:
    RendererSystemDX11 *m_renderer;
    ID3D11InputLayout *m_layout;
    VertexLayout m_layout_desc;
    bool b_haslayout_desc;
};
} // namespace PPGE