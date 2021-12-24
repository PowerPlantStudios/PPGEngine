#include "widget.h"

#include "system/widget_system.h"

namespace PPGE
{
Widget::Widget(const std::string &debug_name) : m_debug_name(debug_name)
{
}

void Widget::Destroy()
{
    WidgetSystem::Get().RemoveWidget(this);
}
} // namespace PPGE
