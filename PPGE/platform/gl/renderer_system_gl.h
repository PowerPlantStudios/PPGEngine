#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/os_helper.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

class GLFWwindow;

namespace PPGE
{
class PPGE_API RendererSystemGL : public RendererSystem
{
  public:
    RendererSystemGL()
    {
    }
    ~RendererSystemGL()
    {
    }

    void StartUp(const RendererSystemProps &props) override;
    void Update() override;
    void ShutDown() override;

    void OnResize() override;

    RendererAPIType GetRendererAPI() override
    {
        return RendererAPIType::OpenGL;
    }

    PPGEDevice *GetDevice() override final;

    PPGEDeviceContext *GetImmediateContext() override final;

    PPGESwapChain *GetSwapChain() override final;

  private:
};
} // namespace PPGE
