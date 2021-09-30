#pragma once
#include <GLFW/glfw3.h>

#include "core/defines.h"
#include "system/display_system.h"
#include "system/logger_system.h"

namespace PPGE
{
class PPGE_API DisplaySystemGLFW : public DisplaySystem
{
  public:
    DisplaySystemGLFW()
    {
    }
    ~DisplaySystemGLFW()
    {
    }

    virtual void StartUp(const DisplaySystemProps &props) override;
    virtual void Update() override;
    virtual void ShutDown() override;

    virtual void *GetNativeDisplayPtr() const override;

    virtual WindowProps::AttributeValue GetWindowAttribute(WindowProps::AttributeTag attribute) const override;
    virtual void SetWindowAttribute(WindowProps::AttributeTag attribute, WindowProps::AttributeValue value) override;
    virtual WindowProps::WindowMode GetWindowMode() const override;
    virtual void SetWindowMode(WindowProps::WindowMode mode) override;

    virtual void EnableVsync() override;
    virtual void DisableVsync() override;
    virtual bool IsVsyncEnabled() const override;
    virtual bool IsMaximized() const override;
    virtual bool IsMinimized() const override;

    virtual WindowProps::InputEventCallback GetInputEventCallback() const override;
    virtual void SetInputEventCallback(WindowProps::InputEventCallback callback) override;
    virtual WindowProps::ApplicationEventCallback GetApplicationEventCallback() const override;
    virtual void SetApplicationEventCallback(WindowProps::ApplicationEventCallback callback) override;

    virtual std::string_view GetTitle() const override;
    virtual void SetTitle(const std::string &title) override;
    virtual uint32_t GetHeight() const override;
    virtual void SetHeight(uint32_t height) override;
    virtual uint32_t GetWidth() const override;
    virtual void SetWidth(uint32_t wdith) override;

  private:
    DisplaySystemProps m_props;
    GLFWwindow *m_window_ptr = nullptr;
};
} // namespace PPGE
