#pragma once

<<<<<<< HEAD
#ifdef PPGE_PLATFORM_WIN
=======
#if _PPGE_PLATFORM_WIN || _PPGE_PLATFORM_MAC
>>>>>>> 4cb0b822951fcd761c4c838b1fcc708d1f7b5467


extern PPGE::Application* PPGE::CreateApplication();

int main(int argc, char** argv)
{
	PPGE::Logger::Initialize();
	PPGE_INFO("PPGE is initialized.");

	PPGE::Application* application = PPGE::CreateApplication();
	application->Run();
	delete application;

	return 0;
}


#endif