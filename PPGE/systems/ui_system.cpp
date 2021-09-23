#include "PPGEpch.h"

#include "ui_system.h"

namespace PPGE
{
UISystem &UISystem::s_instance = UISystem();

void UISystem::StartUp(const UISystemProps &props)
{
}

void UISystem::Update()
{
}

void UISystem::ShutDown()
{
    for (UILayer *subsys : m_ManagedSubsytems)
    {
        subsys->OnDetach();
        delete subsys;
    }
}

void UISystem::PushFrontQueue(UILayer *subsystem)
{
    m_ManagedSubsytems.emplace(m_ManagedSubsytems.begin() + m_InsertIndex, subsystem);
    m_InsertIndex++;
}

void UISystem::PushBackQueue(UILayer *subsystem)
{
    m_ManagedSubsytems.emplace_back(subsystem);
}

void UISystem::PopFrontQueue(UILayer *subsystem)
{
    auto it = std::find(m_ManagedSubsytems.begin(), m_ManagedSubsytems.begin() + m_InsertIndex, subsystem);
    if (it != m_ManagedSubsytems.begin() + m_InsertIndex)
    {
        subsystem->OnDetach();
        m_ManagedSubsytems.erase(it);
        m_InsertIndex--;
    }
}

void UISystem::PopBackQueue(UILayer *subsystem)
{
    auto it = std::find(m_ManagedSubsytems.begin() + m_InsertIndex, m_ManagedSubsytems.end(), subsystem);
    if (it != m_ManagedSubsytems.end())
    {
        subsystem->OnDetach();
        m_ManagedSubsytems.erase(it);
    }
}
} // namespace PPGE