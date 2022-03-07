#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/d3d11/renderer_traits_d3d11.h"
#include "platform/d3d11/shader_resource_variable_d3d11.h"
#include "renderer/shader_resource_binding_base.h"

namespace PPGE
{
class PipelineStateD3D11Impl;

class ShaderResourceBindingD3D11Impl : public ShaderResourceBindingBase<RendererTraitsD3D11>
{
  public:
    using ShaderResourceBindingBaseType = ShaderResourceBindingBase<RendererTraitsD3D11>;

    ShaderResourceBindingD3D11Impl(std::shared_ptr<PipelineStateD3D11Impl> PSO_sp,
                                   const std::vector<std::tuple<std::string, ShaderResourceDesc>> &bindable_resources);

    PPGEShaderResourceVariable *GetVariableByName(const char *name, ShaderTypeFlags shader_type) override final;

    PPGEShaderResourceVariable *GetVariableByIndex(uint32_t index, ShaderTypeFlags shader_type) override final;

    const D3D11Bindables &GetD3D11Bindables(ShaderTypeFlags shader_type) override final;

  private:
    void RefreshBoundResources(ShaderTypeFlags shader_type);

    struct SRBindings
    {
        std::unordered_map<std::string, size_t> SRB_indicies;
        std::vector<std::unique_ptr<PPGEShaderResourceVariableD3D11>> SRBs;

        SRBindings() = default;
        ~SRBindings() = default;
        SRBindings(const SRBindings &) = delete;
        SRBindings(SRBindings&& rhs) noexcept
        {
            SRB_indicies = rhs.SRB_indicies;
            for (auto &SRB : rhs.SRBs)
                SRBs.push_back(std::move(SRB));

            rhs.SRBs.clear();
            rhs.SRB_indicies.clear();
        }
    };
    std::vector<SRBindings> m_SRBs;

    std::array<bool, D3D11_Num_of_Shaders> b_is_bound_res_dirty = {true, true, true, true, true, true};
    std::vector<D3D11Bindables> m_bound_resources;
};
} // namespace PPGE