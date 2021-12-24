#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/widget.h"
#include "system/isystem.h"
#include "ui/imgui_widget.h"

namespace PPGE
{
struct WidgetSystemProps
{
    bool enable_imgui_debug_layer = true;
};

class PPGE_API WidgetSystem : public ISystem<WidgetSystemProps>
{
    using WidgetPtr = std::unique_ptr<Widget>;
    using WidgetContainer = std::vector<WidgetPtr>;
    using ImGuiWidgetPtr = std::unique_ptr<ImGuiWidget>;

  public:
    WidgetSystem()
    {
    }
    ~WidgetSystem()
    {
    }

    PPGE_ITERABLE_CLASS(WidgetContainer, m_widgets)

    virtual void StartUp(const WidgetSystemProps &props) override;
    virtual void ShutDown() override;

    void Update(float delta_time);

    void OnInputEvent(InputEvent &input_event);
    void OnApplicationEvent(ApplicationEvent &application_event);

    template <class WidgetType, class... Args, std::enable_if_t<std::is_base_of_v<Widget, WidgetType>, int> = 0>
    void CreateWidget(Args &&... args)
    {
        WidgetPtr widget = std::make_unique<WidgetType>(std::forward<Args>(args)...);
        widget->OnAttach();
        m_widgets.push_back(std::move(widget));
    }

    void RemoveWidget(Widget *widget);

  private:
    WidgetContainer m_widgets;
    WidgetContainer m_widgets_to_remove;
    ImGuiWidgetPtr m_imgui_widget;
    bool b_imgui_layer_enabled = false;


  public:
    static void Initialize();

    inline static void Destroy()
    {
        SAFE_DELETE(s_instance)
    }

    inline static WidgetSystem &Get()
    {
        return *s_instance;
    }

  private:
    static WidgetSystem *s_instance;
};
} // namespace PPGE