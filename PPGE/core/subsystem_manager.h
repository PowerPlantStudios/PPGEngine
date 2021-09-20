#pragma once

#include "core/defines.h"
#include "systems/subsystem.h"

namespace PPGE {

	class PPGE_API SubsystemManager
	{
	public:
		SubsystemManager() = default;
		~SubsystemManager();

		void PushFrontQueue(Subsystem* subsystem);
		void PushBackQueue(Subsystem* subsystem);
		void PopFrontQueue(Subsystem* subsystem);
		void PopBackQueue(Subsystem* subsystem);

		std::vector<Subsystem*>::iterator begin() { return m_ManagedSubsytems.begin(); }
		std::vector<Subsystem*>::iterator end() { return m_ManagedSubsytems.end(); }
		std::vector<Subsystem*>::reverse_iterator rbegin() { return m_ManagedSubsytems.rbegin(); }
		std::vector<Subsystem*>::reverse_iterator rend() { return m_ManagedSubsytems.rend(); }

		std::vector<Subsystem*>::const_iterator begin() const { return m_ManagedSubsytems.begin(); }
		std::vector<Subsystem*>::const_iterator end()	const { return m_ManagedSubsytems.end(); }
		std::vector<Subsystem*>::const_reverse_iterator rbegin() const { return m_ManagedSubsytems.rbegin(); }
		std::vector<Subsystem*>::const_reverse_iterator rend() const { return m_ManagedSubsytems.rend(); }
	
	private:
		std::vector<Subsystem*> m_ManagedSubsytems;
		unsigned int m_InsertIndex = 0;
	};

}