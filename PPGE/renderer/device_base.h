#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/buffer.h"
#include "renderer/device.h"
#include "renderer/device_context.h"
#include "renderer/pipeline_state.h"
#include "renderer/sampler.h"
#include "renderer/shader.h"
#include "system/logger_system.h"

namespace PPGE
{
template <typename RendererTraits>
class PPGE_API DeviceBase : public RendererTraits::IDevice,
                            public std::enable_shared_from_this<typename RendererTraits::DeviceImpl>
{
  public:
    using DeviceInterface = typename RendererTraits::IDevice;

    using DeviceImplType = typename RendererTraits::DeviceImpl;
    using DeviceContextImplType = typename RendererTraits::DeviceContextImpl;
    using BufferImplType = typename RendererTraits::BufferImpl;
    using ShaderImplType = typename RendererTraits::ShaderImpl;
    using PipelineStateImplType = typename RendererTraits::PipelineStateImpl;
    using TextureImplType = typename RendererTraits::TextureImpl;
    using SamplerImplType = typename RendererTraits::SamplerImpl;

    DeviceBase()
    {
    }

    ~DeviceBase()
    {
    }

    void SetDeviceContext(const std::shared_ptr<DeviceContextImplType> &device_context_sp)
    {
        if (!m_device_context_wp.lock())
            m_device_context_wp = device_context_sp;
    }

    std::weak_ptr<DeviceContextImplType> GetImmediateContext() const
    {
        return m_device_context_wp;
    }

  protected:
    template <typename DeviceObjectType, typename DeviceObjectDescType, typename CreateDeviceObjectCbType>
    void CreateDeviceObject(const DeviceObjectDescType &desc, std::shared_ptr<DeviceObjectType> &device_object_sp,
                            CreateDeviceObjectCbType create_device_object_cb)
    {
#ifdef PPGE_DEBUG
        if (desc.name)
            PPGE_INFO("Creating device object: {0}", desc.name);
        if (device_object_sp)
            PPGE_WARN("Device object pointer provided to store the new device object is not null.");
#endif

        create_device_object_cb();

        PPGE_ASSERT(device_object_sp, "Device object creation has failed.");
    }

    template <typename... Args>
    void CreateBufferImpl(const BufferDesc &desc, std::shared_ptr<PPGEBuffer> &buffer_sp, Args &&...args)
    {
        CreateDeviceObject(desc, buffer_sp, [&]() {
            buffer_sp = std::make_shared<BufferImplType>(static_cast<DeviceImplType *>(this)->shared_from_this(), desc,
                                                         std::forward<Args>(args)...);
#ifdef PPGE_DEBUG
            if (!buffer_sp)
                PPGE_ERROR("Creating renderer buffer has failed.");
#endif
        });
    }

    template <typename PipelineCreateDescType, typename... Args>
    void CreatePipelineStateImpl(const PipelineCreateDescType &desc, std::shared_ptr<PPGEPipelineState> &PSO_sp,
                                 const Args &...args)
    {
        CreateDeviceObject(desc.desc, PSO_sp, [&]() {
            PSO_sp = std::make_shared<PipelineStateImplType>(static_cast<DeviceImplType *>(this)->shared_from_this(),
                                                             desc, std::forward<Args>(args)...);
#ifdef PPGE_DEBUG
            if (!PSO_sp)
                PPGE_ERROR("Creating renderer pipeline state has failed.");
#endif
        });
    }

    template <typename... Args>
    void CreateShaderImpl(const ShaderCreateDesc &desc, std::shared_ptr<PPGEShader> &shader_sp, const Args &...args)
    {
        CreateDeviceObject(desc.desc, shader_sp, [&]() {
            shader_sp = std::make_shared<ShaderImplType>(static_cast<DeviceImplType *>(this)->shared_from_this(), desc,
                                                         std::forward<Args>(args)...);
#ifdef PPGE_DEBUG
            if (!shader_sp)
                PPGE_ERROR("Creating shader has failed.");
#endif
        });
    }

    template <typename... Args>
    void CreateTextureImpl(const TextureCreateDesc &desc, std::shared_ptr<PPGETexture> &texture_sp, const Args &...args)
    {
        CreateDeviceObject(desc.desc, texture_sp, [&]() {
            texture_sp = std::make_shared<TextureImplType>(static_cast<DeviceImplType *>(this)->shared_from_this(),
                                                           desc, std::forward<Args>(args)...);
#ifdef PPGE_DEBUG
            if (!texture_sp)
                PPGE_ERROR("Creating shader has failed.");
#endif
        });
    }

    template <typename... Args>
    void CreateSamplerImpl(const SamplerDesc &desc, std::shared_ptr<PPGESampler> &sampler_sp, const Args &...args)
    {
        CreateDeviceObject(desc, sampler_sp, [&]() {
            sampler_sp = std::make_shared<SamplerImplType>(static_cast<DeviceImplType *>(this)->shared_from_this(),
                                                           desc, std::forward<Args>(args)...);
#ifdef PPGE_DEBUG
            if (!sampler_sp)
                PPGE_ERROR("Creating shader has failed.");
#endif
        });
    }

  protected:
    std::weak_ptr<DeviceContextImplType> m_device_context_wp;
};
} // namespace PPGE