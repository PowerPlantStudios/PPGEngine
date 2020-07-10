#pragma once

#if _PPGE_PLATFORM_WIN || _PPGE_PLATFORM_MAC


extern PPGE::Application* PPGE::CreateApplication();

int main(int argc, char** argv)
{
	PPGE::Logger::Initialize();
	int a = 0;
	PPGE_INFO("PPGE is initialized. {0}", a);

	PPGE::Application* application = PPGE::CreateApplication();
	application->Run();
	delete application;

	return 0;
}


#endif