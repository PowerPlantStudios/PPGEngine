#include "ui_system.h"

namespace PPGE
{
UISystem *UISystem::s_instance = new UISystem();

void UISystem::StartUp(const UISystemProps &props)
{
}

void UISystem::Update()
{
}

void UISystem::ShutDown()
{
    for (auto &&layer : m_managed_layers)
    {
        layer->OnDetach();
    }
}
} // namespace PPGE