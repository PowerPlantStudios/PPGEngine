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
    ClearBufferPass();
    
    std::vector<RenderPassResource> GetPassInputs() const override final
    {
        return {{RenderPassResourceDescs::Color_Buffer_Resource, RenderPassResourceDescs::Color_Buffer_Resource_Desc},
                {RenderPassResourceDescs::Depth_Buffer_Resource, RenderPassResourceDescs::Depth_Buffer_Resource_Desc}};
    }

    std::vector<RenderPassResource> GetPassOutputs() const override final
    {
        return {{RenderPassResourceDescs::Color_Buffer_Resource, RenderPassResourceDescs::Color_Buffer_Resource_Desc},
                {RenderPassResourceDescs::Depth_Buffer_Resource, RenderPassResourceDescs::Depth_Buffer_Resource_Desc}};
    }

    void Load(RenderGraph &render_graph) override final;

    void Unload() override final;

    void Execute() override final;

  private:
    /* Resources shared between other render passes within render graph */
    std::shared_ptr<PPGETextureView> m_color_buffer_rtv;

    std::shared_ptr<PPGETextureView> m_depth_buffer_dsv;
};
} // namespace PPGE
