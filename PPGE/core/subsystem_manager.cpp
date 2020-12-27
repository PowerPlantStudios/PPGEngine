#include "PPGEpch.h"

#include "subsystem_manager.h"

namespace PPGE
{
	SubsystemManager::~SubsystemManager()
	{
		for (Subsystem* subsys : m_ManagedSubsytems)
		{
			subsys->OnDetach();
			delete subsys;
		}
	}

	void SubsystemManager::PushFrontQueue(Subsystem* subsystem)
	{
		m_ManagedSubsytems.emplace(m_ManagedSubsytems.begin() + m_InsertIndex, subsystem);
		m_InsertIndex++;
	}

	void SubsystemManager::PushBackQueue(Subsystem* subsystem)
	{
		m_ManagedSubsytems.emplace_back(subsystem);
	}

	void SubsystemManager::PopFrontQueue(Subsystem* subsystem)
	{
		auto it = std::find(m_ManagedSubsytems.begin(), m_ManagedSubsytems.begin() + m_InsertIndex, subsystem);
		if (it != m_ManagedSubsytems.begin() + m_InsertIndex)
		{
			subsystem->OnDetach();
			m_ManagedSubsytems.erase(it);
			m_InsertIndex--;
		}
	}

	void SubsystemManager::PopBackQueue(Subsystem* subsystem)
	{
		auto it = std::find(m_ManagedSubsytems.begin() + m_InsertIndex, m_ManagedSubsytems.end(), subsystem);
		if (it != m_ManagedSubsytems.end())
		{
			subsystem->OnDetach();
			m_ManagedSubsytems.erase(it);
		}
	}
}