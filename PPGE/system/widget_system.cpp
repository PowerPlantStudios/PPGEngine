#include "widget_system.h"

namespace PPGE
{
WidgetSystem *WidgetSystem::s_instance = nullptr;

void WidgetSystem::StartUp(const WidgetSystemProps &props)
{
    b_imgui_layer_enabled = props.enable_imgui_debug_layer;
    if (b_imgui_layer_enabled)
    {
        m_imgui_widget = ImGuiWidget::CreateImGuiLayer();
        m_imgui_widget->OnAttach();
    }
}

void WidgetSystem::ShutDown()
{
    for (auto it = m_widgets_to_remove.begin(); it != m_widgets_to_remove.end();)
    {
        (*it)->OnDetach();
        it = m_widgets_to_remove.erase(it);
    }

    WidgetContainer().swap(m_widgets_to_remove);

    for (auto it = m_widgets.begin(); it != m_widgets.end();)
    {
        (*it)->OnDetach();
        it = m_widgets.erase(it);
    }

    WidgetContainer().swap(m_widgets);

    if (b_imgui_layer_enabled)
    {
        m_imgui_widget->OnDetach();
        m_imgui_widget.reset(nullptr);
    }
}

void WidgetSystem::Update(float delta_time)
{
    for (auto it = m_widgets_to_remove.begin(); it != m_widgets_to_remove.end();)
    {
        (*it)->OnDetach();
        it = m_widgets_to_remove.erase(it);
    }

    for (auto it = m_widgets.begin(); it != m_widgets.end();)
    {
        if (!(*it))
        {
            it = m_widgets.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto &widget : m_widgets)
    {
        widget->OnUpdate(delta_time);
        widget->OnRender();
    }

    if (b_imgui_layer_enabled)
    {
        m_imgui_widget->ImGuiBegin();
        for (auto &widget : m_widgets)
        {
            widget->OnImGui();
        }
        m_imgui_widget->ImGuiEnd();
    }
}

void WidgetSystem::OnInputEvent(InputEvent &input_event)
{
    if (b_imgui_layer_enabled)
    {
        m_imgui_widget->OnInputEvent(input_event);
    }

    if (!input_event.Handled())
    {
        for (auto &widget : m_widgets)
        {
            widget->OnInputEvent(input_event);

            if (input_event.Handled())
                break;
        }
    }
}

void WidgetSystem::OnApplicationEvent(ApplicationEvent &application_event)
{
    if (b_imgui_layer_enabled)
    {
        m_imgui_widget->OnApplicationEvent(application_event);
    }

    if (!application_event.Handled())
    {
        for (auto &widget : m_widgets)
        {
            widget->OnApplicationEvent(application_event);

            if (application_event.Handled())
                break;
        }
    }
}

void WidgetSystem::RemoveWidget(Widget *widget)
{
    auto search = std::find_if(m_widgets.begin(), m_widgets.end(),
                               [&](const WidgetPtr &u_widget) { return u_widget.get() == widget; });
    if (search != m_widgets.end())
    {
        m_widgets_to_remove.push_back(std::move(*search));
    }
}

void WidgetSystem::Initialize()
{
    if (!s_instance)
        s_instance = new WidgetSystem();
}
} // namespace PPGE