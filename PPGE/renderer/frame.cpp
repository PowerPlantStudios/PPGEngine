#include "frame.h"

namespace PPGE
{
void Frame::SetVertexBufferHandle(VertexBufferHandle handle)
{
    PPGE_ASSERT(handle.IsValid(),
                "Adding vertex buffer handle to frame has failed. Handle to vertex buffer is not valid.");
    PPGE_ASSERT(!ReachedDrawCallLimit(), "Number of draw calls per frame has exceeded maximum limit.");
    DrawData &draw_data = m_draw_data[m_num_of_draw_calls];
    draw_data.vb_handle = handle;
}

void Frame::SetVertexLayoutHandle(VertexLayoutHandle handle)
{
    PPGE_ASSERT(handle.IsValid(),
                "Adding vertex buffer handle to frame has failed. Handle to vertex buffer is not valid.");
    PPGE_ASSERT(!ReachedDrawCallLimit(), "Number of draw calls per frame has exceeded maximum limit.");
    DrawData &draw_data = m_draw_data[m_num_of_draw_calls];
    draw_data.ly_handle = handle;
}

void Frame::SetIndexBufferHandle(IndexBufferHandle handle)
{
    PPGE_ASSERT(handle.IsValid(),
                "Adding index buffer handle to frame has failed. Handle to index buffer is not valid.");
    PPGE_ASSERT(!ReachedDrawCallLimit(), "Number of draw calls per frame has exceeded maximum limit.");
    DrawData &draw_data = m_draw_data[m_num_of_draw_calls];
    draw_data.ib_handle = handle;
}

void Frame::UpdateObjectUniform(UniformHandle handle, SubResource subresource)
{
    PPGE_ASSERT(handle.IsValid(), "Updating per object uniform has failed. Handle to uniform is not valid.");
    PPGE_ASSERT(!ReachedDrawCallLimit(), "Number of draw calls per frame has exceeded maximum limit.");

    PPGE_ASSERT(((m_uniform_cache_offset + subresource.m_size) < 1 * 1024 * 1024), "Per objcet uniform cache is full.");
    void *cache = &m_uniform_cache[m_uniform_cache_offset]; 
    memcpy(cache, subresource.m_pData, subresource.m_size);
    m_uniform_cache_offset += subresource.m_size;

    SubResource sr;
    sr.m_pData = cache;
    sr.m_size = subresource.m_size;
    
    DrawData &draw_data = m_draw_data[m_num_of_draw_calls];
    draw_data.PushUniformUpdate(handle, sr);
}

void Frame::SetObjectUniform(UniformHandle handle, ShaderDesc::ShaderType target, uint8_t slot)
{
    PPGE_ASSERT(handle.IsValid(),
                "Adding index buffer handle to frame has failed. Handle to index buffer is not valid.");
    PPGE_ASSERT(!ReachedDrawCallLimit(), "Number of draw calls per frame has exceeded maximum limit.");
    DrawData &draw_data = m_draw_data[m_num_of_draw_calls];
    draw_data.PushUniformBind(handle, target, slot);
}

void Frame::Submit(ProgramHandle handle)
{
    PPGE_ASSERT(handle.IsValid(), "Adding draw call to frame has failed. Handle to program is not valid.");
    PPGE_ASSERT(!ReachedDrawCallLimit(), "Number of draw calls per frame has exceeded maximum limit.");
    DrawData &draw_data = m_draw_data[m_num_of_draw_calls];
    draw_data.pg_handle = handle;
    m_num_of_draw_calls++;
}
} // namespace PPGE