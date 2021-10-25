#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/display_system.h"

namespace PPGE
{
class PPGE_API DisplaySystemWin32 : public DisplaySystem
{
  public:
    DisplaySystemWin32()
    {
    }
    ~DisplaySystemWin32()
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

    virtual void SetInputEventCallback(WindowProps::InputEventCallback callback) override;
    virtual void SetApplicationEventCallback(WindowProps::ApplicationEventCallback callback) override;

    virtual std::string_view GetTitle() const override;
    virtual uint32_t GetHeight() const override;
    virtual uint32_t GetWidth() const override;

  private:
    static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
    LRESULT HandleMessage(UINT msg, WPARAM w_param, LPARAM l_param);
    
    DisplaySystemProps m_props;
    HWND m_hwnd = nullptr;
    HINSTANCE m_hmodule = nullptr;
    MSG m_msg;
};
} // namespace PPGE
