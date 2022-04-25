#include "device_d3d11_impl.h"

#include "platform/d3d11/buffer_d3d11_impl.h"
#include "platform/d3d11/pipeline_state_d3d11_impl.h"
#include "platform/d3d11/sampler_d3d11_impl.h"
#include "platform/d3d11/shader_d3d11_impl.h"
#include "platform/d3d11/texture_d3d11_impl.h"

namespace PPGE
{
void DeviceD3D11Impl::CreateBuffer(const BufferDesc &desc, const BufferData *data,
                                   std::shared_ptr<PPGEBuffer> &buffer_sp)
{
    CreateBufferImpl(desc, buffer_sp, data);
}

void DeviceD3D11Impl::CreatePipelineState(const GfxPipelineStateCreateDesc &desc,
                                          std::shared_ptr<PPGEPipelineState> &PSO_sp)
{
    CreatePipelineStateImpl(desc, PSO_sp);
}

void DeviceD3D11Impl::CreateShader(const ShaderCreateDesc &desc, std::shared_ptr<PPGEShader> &shader_sp)
{
    CreateShaderImpl(desc, shader_sp);
}

void DeviceD3D11Impl::CreateTexture(const TextureCreateDesc &desc, std::shared_ptr<PPGETexture> &texture_sp)
{
    CreateTextureImpl(desc, texture_sp);
}

void DeviceD3D11Impl::CreateTextureFromD3D11Resource(ID3D11Texture1D *texture_ptr,
                                                     std::shared_ptr<PPGETexture> &texture_sp)
{
    PPGE_ASSERT(texture_ptr, "Create PPGETexture has failed: ID3D11Texture1D ptr is empty.");

    TextureDesc desc;
    CreateDeviceObject(desc, texture_sp, [&]() {
        texture_sp = std::make_shared<TextureD3D11Impl>((this)->shared_from_this(), texture_ptr);
#ifdef PPGE_DEBUG
        if (!texture_sp)
            PPGE_ERROR("Creating shader has failed.");
#endif
    });
}
void DeviceD3D11Impl::CreateTextureFromD3D11Resource(ID3D11Texture2D *texture_ptr,
                                                     std::shared_ptr<PPGETexture> &texture_sp)
{
    PPGE_ASSERT(texture_ptr, "Create PPGETexture has failed: ID3D11Texture2D ptr is empty.");

    TextureDesc desc;
    CreateDeviceObject(desc, texture_sp, [&]() {
        texture_sp = std::make_shared<TextureD3D11Impl>((this)->shared_from_this(), texture_ptr);
#ifdef PPGE_DEBUG
        if (!texture_sp)
            PPGE_ERROR("Creating shader has failed.");
#endif
    });
}

void DeviceD3D11Impl::CreateTextureFromD3D11Resource(ID3D11Texture3D *texture_ptr,
                                                     std::shared_ptr<PPGETexture> &texture_sp)
{
    PPGE_ASSERT(texture_ptr, "Create PPGETexture has failed: ID3D11Texture3D ptr is empty.");

    TextureDesc desc;
    CreateDeviceObject(desc, texture_sp, [&]() {
        texture_sp = std::make_shared<TextureD3D11Impl>((this)->shared_from_this(), texture_ptr);
#ifdef PPGE_DEBUG
        if (!texture_sp)
            PPGE_ERROR("Creating shader has failed.");
#endif
    });
}

void DeviceD3D11Impl::CreateSampler(const SamplerDesc &desc, std::shared_ptr<PPGESampler> &sampler_sp)
{
    CreateSamplerImpl(desc, sampler_sp);
}
} // namespace PPGE
