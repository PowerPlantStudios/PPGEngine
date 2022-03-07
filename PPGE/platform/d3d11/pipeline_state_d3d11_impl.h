#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/renderer_traits_d3d11.h"
#include "renderer/pipeline_state_base.h"
#include "system/logger_system.h"

namespace PPGE
{
class PipelineStateD3D11Impl final : public PipelineStateBase<RendererTraitsD3D11>
{
  public:
    using PipelineStateBaseType = PipelineStateBase<RendererTraitsD3D11>;

    PipelineStateD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, const GfxPipelineStateCreateDesc &desc);

    ~PipelineStateD3D11Impl();

    ID3D11InputLayout *GetD3D11InputLayout() const override final;

    ID3D11VertexShader *GetD3D11VertexShader() const override final;

    ID3D11PixelShader *GetD3D11PixelShader() const override final;

    ID3D11GeometryShader *GetD3D11GeometryShader() const override final;

    ID3D11DomainShader *GetD3D11DomainShader() const override final;

    ID3D11HullShader *GetD3D11HullShader() const override final;

    ID3D11ComputeShader *GetD3D11ComputeShader() const override final;

    void CreateShaderResourceBinding(
        std::shared_ptr<PPGEShaderResourceBinding> &pp_shader_resource_binding) override final;

    int8_t GetNumActiveShaders() const
    {
        return m_shaders.size();
    }

    constexpr inline int8_t GetPipelineShaderIndex(ShaderTypeFlags shader_type) const
    {
        switch (shader_type)
        {
        case PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX:
            return m_active_shader_indicies[0];
        case PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL:
            return m_active_shader_indicies[1];
        case PPGE::ShaderTypeFlags::SHADER_TYPE_GEOMETRY:
            return m_active_shader_indicies[2];
        case PPGE::ShaderTypeFlags::SHADER_TYPE_HULL:
            return m_active_shader_indicies[3];
        case PPGE::ShaderTypeFlags::SHADER_TYPE_DOMAIN:
            return m_active_shader_indicies[4];
        case PPGE::ShaderTypeFlags::SHADER_TYPE_COMPUTE:
            return m_active_shader_indicies[5];
        default:
            PPGE_ASSERT(false,
                        "Unknown shader type flag to retrieve shader index bound to D3D11 Pipeline State Object.");
            return -1;
        }
    }

  private:
    std::array<int8_t, D3D11_Num_of_Shaders> m_active_shader_indicies = {-1, -1, -1, -1, -1, -1};

    using ActiveShaders = std::vector<std::shared_ptr<PPGEShaderD3D11>>;
    ActiveShaders m_shaders;

    using BindableShaderResourceDescs = std::vector<std::tuple<std::string, ShaderResourceDesc>>;
    BindableShaderResourceDescs m_bindable_resource_descs;

    CComPtr<ID3D11InputLayout> m_input_layout_ptr;
};
} // namespace PPGE