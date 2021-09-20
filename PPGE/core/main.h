#pragma once

extern PPGE::Application* PPGE::CreateApplication();

#if DISPLAY_API_WIN32
// TODO : When either DX11 or DX12 is implemented this main routine has to be resdigned
//        Just changing function decleration is not going to solve this problem. Some other
//        other routines has to be preformed as well. Maybe with some good abstraction, we 
//        can get away with. In addition to that CMake application target definition has to be
//        fixed to allow compiling as Win32 application, rather than the console application.
//
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
	PPGE::Logger::Initialize();
	PPGE_INFO("PPGE is initialized.");

	PPGE::Application* application = PPGE::CreateApplication();
	application->Run();
	delete application;

	return 0;
}
