#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/ppge_object.h"
#include "renderer/buffer.h"
#include "renderer/device_object.h"
#include "renderer/pipeline_state.h"
#include "renderer/renderer_constants.h"
#include "renderer/texture_view.h"

namespace PPGE
{
struct Viewport
{
    float top_left_x = 0.0f;
    float top_left_y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    float min_depth = 0.0f;
    float max_depth = 0.0f;
};

struct Rect
{
    int32_t left = 0;
    int32_t top = 0;
    int32_t right = 0;
    int32_t bottom = 0;
};

struct DeviceContextDesc
{
};

class PPGE_API PPGEDeviceContext : public PPGEObject
{
  public:
    virtual void SetVertexBuffers(uint32_t num_buffers, std::shared_ptr<PPGEBuffer> p_buffers[],
                                  const uint64_t offsets[]) = 0;

    virtual void SetIndexBuffer(std::shared_ptr<PPGEBuffer> p_buffer) = 0;

    virtual void SetPipelineStateObject(std::shared_ptr<PPGEPipelineState> p_PSO) = 0;

    virtual void CommitShaderResources(const std::shared_ptr<PPGEShaderResourceBinding> &p_SRB) = 0;

    virtual void SetViewports(uint32_t num_viewports, const Viewport viewports[]) = 0;

    virtual void SetScissorRects(uint32_t num_rects, const Rect rects[]) = 0;

    virtual void SetRenderTargets(uint32_t num_render_targets, std::shared_ptr<PPGETextureView> pp_RTs[],
                                  std::shared_ptr<PPGETextureView> p_DS) = 0;

    virtual void ClearRenderTarget(const std::shared_ptr<PPGETextureView> &p_view, float r, float b, float g,
                                   float a) = 0;

    virtual void ClearDepthStencil(const std::shared_ptr<PPGETextureView> &p_view, float d, uint8_t s) = 0;

    virtual void Draw(uint32_t num_vertices) = 0;

    virtual void DrawIndirect(uint32_t num_vertices) = 0;

    virtual void DrawIndexed(uint32_t num_indicies) = 0;

    virtual void DrawIndexedIndirect(uint32_t num_indicies) = 0;

    virtual void Map(PPGEBuffer *p_buffer, MapType map_type, MapFlags map_flags, void **pp_map_data) = 0;

    virtual void Unmap(PPGEBuffer *p_buffer) = 0;

    virtual void Flush() = 0;
};
} // namespace PPGE
