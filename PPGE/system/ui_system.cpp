#include "ui_system.h"

namespace PPGE
{
UISystem *UISystem::s_instance = nullptr;

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
    // invalidate all shared ptr references.
    // since we only allow weak pointer ownership
    // this should be ok. But there are still potential
    // misusages. We will think of something better (e.g. using id)
    m_managed_layers.clear();
    // to guarantee zero capacity for the container. 
    layer_container().swap(m_managed_layers);
}

void UISystem::Initialize()
{
    if (!s_instance)
        s_instance = new UISystem();
}
} // namespace PPGE