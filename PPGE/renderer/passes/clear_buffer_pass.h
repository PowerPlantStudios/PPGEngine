#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_pass.h"
#include "renderer/render_pass_resource_descs.h"
#include "rhi/texture_view.h"

namespace PPGE
{
class ClearBufferPass : public RenderPass
{
  public:
    const std::vector<RenderPassResource> &GetPassInputs() const override final;

    const std::vector<RenderPassResource> &GetPassOutputs() const override final;

    ClearBufferPass();

    void Load(RenderGraph &render_graph) override final;

    void Unload() override final;

    void Execute() override final;

  private:
    const std::vector<RenderPassResource> m_pass_inputs{
        {RenderPassResourceDescs::Color_Buffer_Resource, RenderPassResourceDescs::Color_Buffer_Resource_Desc},
        {RenderPassResourceDescs::Depth_Buffer_Resource, RenderPassResourceDescs::Depth_Buffer_Resource_Desc}};

    const std::vector<RenderPassResource> m_pass_outputs{
        {RenderPassResourceDescs::Color_Buffer_Resource, RenderPassResourceDescs::Color_Buffer_Resource_Desc},
        {RenderPassResourceDescs::Depth_Buffer_Resource, RenderPassResourceDescs::Depth_Buffer_Resource_Desc}};

    /* Resources shared between other render passes within render graph */
    std::shared_ptr<PPGETextureView> m_color_buffer_rtv;

    std::shared_ptr<PPGETextureView> m_depth_buffer_dsv;

};
} // namespace PPGE
