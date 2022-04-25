#pragma once
#include "PPGEpch.h"

#include "platform/d3d11/buffer_d3d11.h"
#include "platform/d3d11/device_context_d3d11.h"
#include "platform/d3d11/device_d3d11.h"
#include "platform/d3d11/pipeline_state_d3d11.h"
#include "platform/d3d11/sampler_d3d11.h"
#include "platform/d3d11/shader_d3d11.h"
#include "platform/d3d11/shader_resource_binding_d3d11.h"
#include "platform/d3d11/texture_d3d11.h"
#include "platform/d3d11/texture_view_d3d11.h"

namespace PPGE
{
class DeviceD3D11Impl;
class DeviceContextD3D11Impl;
class BufferD3D11Impl;
class ShaderD3D11Impl;
class PipelineStateD3D11Impl;
class ShaderResourceBindingD3D11Impl;
class TextureD3D11Impl;
class TextureViewD3D11Impl;
class SamplerD3D11Impl;

struct RendererTraitsD3D11
{
    using IDevice = PPGEDeviceD3D11;
    using IDeviceContext = PPGEDeviceContexD3D11;
    using IBuffer = PPGEBufferD3D11;
    using IShader = PPGEShaderD3D11;
    using IPipelineState = PPGEPipelineStateD3D11;
    using IShaderResourceBinding = PPGEShaderResourceBindingD3D11;
    using ITexture = PPGETextureD3D11;
    using ITextureView = PPGETextureViewD3D11;
    using ISampler = PPGESamplerD3D11;

    using DeviceImpl = DeviceD3D11Impl;
    using DeviceContextImpl = DeviceContextD3D11Impl;
    using BufferImpl = BufferD3D11Impl;
    using ShaderImpl = ShaderD3D11Impl;
    using PipelineStateImpl = PipelineStateD3D11Impl;
    using ShaderResourceBindingImpl = ShaderResourceBindingD3D11Impl;
    using TextureImpl = TextureD3D11Impl;
    using TextureViewImpl = TextureViewD3D11Impl;
    using SamplerImpl = SamplerD3D11Impl;
};
} // namespace PPGE