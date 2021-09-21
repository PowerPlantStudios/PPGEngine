#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/input/application_event.h"
#include "core/input/input_event.h"
#include "core/smart_ptr.h"
#include "systems/isystem.h"
#include "systems/logger_system.h"

namespace PPGE
{
namespace WindowProps
{
enum class WindowMode
{
    WINDOWED,
    MINIMIZED,
    MAXIMIZED,
    FULLSCREEN
};
enum class AttributeTag
{
    ALWAYS_ON_TOP = 0,
    DECORATED,
    RESIZABLE,
    FOCUSED,
    COUNT,
};
using AttributeValue = uint64_t;
using WindowAttributes = std::array<WindowProps::AttributeValue, static_cast<size_t>(WindowProps::AttributeTag::COUNT)>;
using InputEventCallback = std::function<void(InputEvent &)>;
using ApplicationEventCallback = std::function<void(ApplicationEvent &)>;
}; // namespace WindowProps

struct DisplaySystemProps
{
    std::string title = "Power Plant Game Engine";
    uint32_t height = 720;
    uint32_t width = 1280;
    uint32_t posX = 120;
    uint32_t posY = 120;
    bool vSync = true;
    WindowProps::WindowMode window_mode = WindowProps::WindowMode::WINDOWED;
    WindowProps::WindowAttributes window_attributes = {1ull, 0ull, 1ull, 1ull};
    WindowProps::InputEventCallback input_event_callback = nullptr;
    WindowProps::ApplicationEventCallback application_event_callback = nullptr;
};

class PPGE_API DisplaySystem : public ISystem<DisplaySystemProps>
{
  public:
    virtual void *GetNativeDisplayPtr() const = 0;

    virtual WindowProps::AttributeValue GetWindowAttribute(WindowProps::AttributeTag attribute) const = 0;
    virtual void SetWindowAttribute(WindowProps::AttributeTag attribute, WindowProps::AttributeValue value) = 0;
    virtual WindowProps::WindowMode GetWindowMode() const = 0;
    virtual void SetWindowMode(WindowProps::WindowMode mode) = 0;

    virtual void EnableVsync() = 0;
    virtual void DisableVsync() = 0;
    virtual bool IsVsyncEnabled() const = 0;
    virtual bool IsMaximized() const = 0;
    virtual bool IsMinimized() const = 0;

    virtual WindowProps::InputEventCallback GetInputEventCallback() const = 0;
    virtual void SetInputEventCallback(WindowProps::InputEventCallback callback) = 0;
    virtual WindowProps::ApplicationEventCallback GetApplicationEventCallback() const = 0;
    virtual void SetApplicationEventCallback(WindowProps::ApplicationEventCallback callback) = 0;
    virtual std::string_view GetTitle() const = 0;
    virtual void SetTitle(const std::string &title) = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual void SetHeight(uint32_t height) = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual void SetWidth(uint32_t wdith) = 0;

    inline static DisplaySystem &GetDisplaySystem()
    {
        return s_instance;
    }

  private:
    static DisplaySystem &s_instance;
};
} // namespace PPGE
