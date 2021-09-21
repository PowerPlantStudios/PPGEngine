#include "PPGEpch.h"

#include "layer_system.h"

namespace PPGE
{
LayerSystem &LayerSystem::s_instance = LayerSystem();

void LayerSystem::StartUp(const LayerSystemProps &props)
{
}

void LayerSystem::Update()
{
}

void LayerSystem::ShutDown()
{
    for (Layer *subsys : m_ManagedSubsytems)
    {
        subsys->OnDetach();
        delete subsys;
    }
}

void LayerSystem::PushFrontQueue(Layer *subsystem)
{
    m_ManagedSubsytems.emplace(m_ManagedSubsytems.begin() + m_InsertIndex, subsystem);
    m_InsertIndex++;
}

void LayerSystem::PushBackQueue(Layer *subsystem)
{
    m_ManagedSubsytems.emplace_back(subsystem);
}

void LayerSystem::PopFrontQueue(Layer *subsystem)
{
    auto it = std::find(m_ManagedSubsytems.begin(), m_ManagedSubsytems.begin() + m_InsertIndex, subsystem);
    if (it != m_ManagedSubsytems.begin() + m_InsertIndex)
    {
        subsystem->OnDetach();
        m_ManagedSubsytems.erase(it);
        m_InsertIndex--;
    }
}

void LayerSystem::PopBackQueue(Layer *subsystem)
{
    auto it = std::find(m_ManagedSubsytems.begin() + m_InsertIndex, m_ManagedSubsytems.end(), subsystem);
    if (it != m_ManagedSubsytems.end())
    {
        subsystem->OnDetach();
        m_ManagedSubsytems.erase(it);
    }
}
} // namespace PPGE