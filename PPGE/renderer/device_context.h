#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/ppge_object.h"
#include "renderer/buffer.h"
#include "renderer/device_object.h"
#include "renderer/pipeline_state.h"

namespace PPGE
{
struct DeviceContextDesc
{
};

class PPGE_API PPGEDeviceContext : public PPGEObject
{
  public:
    virtual void SetVertexBuffers(uint32_t num_buffers, PPGEBuffer *p_buffer) = 0;

    virtual void SetIndexBuffer(PPGEBuffer *p_buffer) = 0;

    virtual void SetPipelineObject(PPGEPipelineState *p_PSO) = 0;

    virtual void SetShaderResourceBinding(PPGEShaderResourceBinding *p_SRB) = 0;

    // virtual void SetRenderTargets(uint32_t num_render_targets, ITextureView **pp_RTs, ITextureView *p_DS) = 0;
    //
    // virtual void ClearRenderTarget(ITextureView* p_view, float r, float b, float g, float a) = 0;
    //
    // virtual void ClearDepthStencil(ITextureView* p_view, float d, uint8_t s) = 0;

    virtual void Draw(uint32_t num_vertices) = 0;

    virtual void DrawIndexed(uint32_t num_indicies) = 0;

    virtual void Map(PPGEBuffer *p_buffer, MapType map_type, MapFlags map_flags, void **pp_map_data) = 0;

    virtual void Unmap(PPGEBuffer *p_buffer) = 0;
};
} // namespace PPGE
