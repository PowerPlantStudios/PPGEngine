#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/display_system.h"
#include "system/logger_system.h"

class GLFWwindow;

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

    void StartUp(const DisplaySystemProps &props) override;
    void Update() override;
    void ShutDown() override;

    void *GetNativeDisplayPtr() const override;

    std::tuple<float, float> GetMousePosition() override;
    bool IsKeyPressed(const KeyCode code) override;
    bool IsMouseButtonPressed(const MouseCode code) override;
    float GetMouseX() override;
    float GetMouseY() override;

    WindowProps::AttributeValue GetWindowAttribute(WindowProps::AttributeTag attribute) const override;
    void SetWindowAttribute(WindowProps::AttributeTag attribute, WindowProps::AttributeValue value) override;
    WindowProps::WindowMode GetWindowMode() const override;
    void SetWindowMode(WindowProps::WindowMode mode) override;

    void EnableVsync() override;
    void DisableVsync() override;
    bool IsVsyncEnabled() const override;

    bool IsMaximized() const override;
    bool IsMinimized() const override;

    void SetInputEventCallback(WindowProps::InputEventCallback callback) override;
    void SetApplicationEventCallback(WindowProps::ApplicationEventCallback callback) override;

    std::string_view GetTitle() const override;
    uint32_t GetHeight() const override;
    uint32_t GetWidth() const override;

  private:
    DisplaySystemProps m_props;
    GLFWwindow *m_window_ptr = nullptr;
};
} // namespace PPGE
