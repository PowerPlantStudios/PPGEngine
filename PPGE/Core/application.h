#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{
	class PPGE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// Decleration to be implemented on client code
	Application* CreateApplication();
}
