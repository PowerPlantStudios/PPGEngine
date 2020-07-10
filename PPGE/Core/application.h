#pragma once

#include "defines.h"

namespace PPGE
{
    class _PPGE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// Decleration to be implemented on client code
	Application* CreateApplication();
}
