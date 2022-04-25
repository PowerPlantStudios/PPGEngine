#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
template <typename RendererTraits> class PPGE_API DeviceContextBase : public RendererTraits::IDeviceContext
{
  public:
    using DeviceContextInterface = typename RendererTraits::IDeviceContext;

    using DeviceImplType = typename RendererTraits::DeviceImpl;
    using DeviceContextImplType = typename RendererTraits::DeviceContextImpl;
    using BufferImplType = typename RendererTraits::BufferImpl;
    using PipelineStateImplType = typename RendererTraits::PipelineStateImpl;
    using TextureViewImplType = typename RendererTraits::TextureViewImpl;

    DeviceContextBase(std::shared_ptr<DeviceImplType> device_sp, const DeviceContextDesc &desc)
        : m_device_sp(std::move(device_sp)), m_desc(desc)
    {
    }

    ~DeviceContextBase()
    {
    }

  protected:
    std::shared_ptr<DeviceImplType> m_device_sp;

    std::shared_ptr<BufferImplType> m_bound_vertex_buffers_sp[Num_Of_Buffer_Slots];

    size_t m_bound_vertex_buffer_count = 0;

    std::shared_ptr<BufferImplType> m_bound_index_buffer_sp;

    std::shared_ptr<PipelineStateImplType> m_bound_pipeline_state_sp;

    Viewport m_viewports[Num_Of_Viewports];

    size_t m_viewport_count = 0;

    Rect m_scissor_rects[Num_Of_Scissor_Rects];

    size_t m_scissor_count = 0;

    std::shared_ptr<TextureViewImplType> m_render_targets_sp[Num_Of_Render_Targets];

    size_t m_bound_render_target_count = 0;

    std::shared_ptr<TextureViewImplType> m_depth_stencil_buffer_sp;

    DeviceContextDesc m_desc;
};
} // namespace PPGE