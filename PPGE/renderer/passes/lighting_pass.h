#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_pass.h"
#include "renderer/render_pass_resource_descs.h"

namespace PPGE
{
class PPGEBuffer;
class PPGEPipelineState;
class PPGESampler;
class PPGEShaderResourceBinding;

class LightingPass : public SceneRenderPass
{
  public:
    LightingPass();

    std::vector<RenderPassResource> GetPassInputs() const override final
    {
        using namespace RenderPassResourceDescs;
        return {{Albedo_Buffer_Resource, Albedo_Buffer_Resource_Desc},
                {Normal_Buffer_Resource, Normal_Buffer_Resource_Desc},
                {Position_Buffer_Resource, Position_Buffer_Resource_Desc},
                {Material_Buffer_Resource, Material_Buffer_Resource_Desc},
                {Emission_Buffer_Resource, Emission_Buffer_Resource_Desc},
                {Depth_Buffer_Resource, Depth_Buffer_Resource_Desc}};
    }

    std::vector<RenderPassResource> GetPassOutputs() const override final
    {
        using namespace RenderPassResourceDescs;
        return {{Present_Buffer_Resource, Present_Buffer_Resource_Desc}};
    }

    void Load(RenderGraph &render_graph) override final;

    void Unload() override final;

    void Execute() override final;

  private:
    /* Resources shared between other render passes within render graph */
    std::shared_ptr<PPGETextureView> m_albedo_buffer_srv;

    std::shared_ptr<PPGETextureView> m_normal_buffer_srv;

    std::shared_ptr<PPGETextureView> m_position_buffer_srv;

    std::shared_ptr<PPGETextureView> m_material_buffer_srv;

    std::shared_ptr<PPGETextureView> m_emission_buffer_srv;

    std::shared_ptr<PPGETextureView> m_color_buffer_rtv;

    std::shared_ptr<PPGETextureView> m_depth_buffer_dsv;

    /* Resources created and used within the pass */
    std::shared_ptr<PPGEBuffer> m_cb_light;

    std::shared_ptr<PPGESampler> m_comparison_sampler;

    std::shared_ptr<PPGESampler> m_anisotropic_sampler;

    std::shared_ptr<PPGEPipelineState> m_light_PSO;

    std::shared_ptr<PPGEShaderResourceBinding> m_light_SRB;

    std::shared_ptr<PPGEPipelineState> m_emission_PSO;

    std::shared_ptr<PPGEShaderResourceBinding> m_emission_SRB;
};
} // namespace PPGE
