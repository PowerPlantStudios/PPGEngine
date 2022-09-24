#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/input.h"
#include "core/os_helper.h"
#include "event/application_event.h"
#include "event/input_event.h"
#include "system/isystem.h"
#include "system/logger_system.h"

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
    virtual void Update() = 0;

    virtual void *GetNativeDisplayPtr() const = 0;

    virtual std::tuple<float, float> GetMousePosition() = 0;
    virtual bool IsKeyPressed(const KeyCode code) = 0;
    virtual bool IsMouseButtonPressed(const MouseCode code) = 0;
    virtual float GetMouseX() = 0;
    virtual float GetMouseY() = 0;

    virtual WindowProps::AttributeValue GetWindowAttribute(WindowProps::AttributeTag attribute) const = 0;
    virtual void SetWindowAttribute(WindowProps::AttributeTag attribute, WindowProps::AttributeValue value) = 0;
    virtual WindowProps::WindowMode GetWindowMode() const = 0;
    virtual void SetWindowMode(WindowProps::WindowMode mode) = 0;

    virtual void EnableVsync() = 0;
    virtual void DisableVsync() = 0;
    virtual bool IsVsyncEnabled() const = 0;

    virtual bool IsMaximized() const = 0;
    virtual bool IsMinimized() const = 0;

    virtual void SetInputEventCallback(WindowProps::InputEventCallback callback) = 0;
    virtual void SetApplicationEventCallback(WindowProps::ApplicationEventCallback callback) = 0;
    virtual std::string_view GetTitle() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetWidth() const = 0;

  public:
    static void Initialize(WindowAPIType api);

    inline static void Destroy()
    {
        SAFE_DELETE(s_instance)
    }

    inline static DisplaySystem &Get()
    {
        return *s_instance;
    }

  private:
    static DisplaySystem *s_instance;
};
} // namespace PPGE
