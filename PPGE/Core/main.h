#pragma once

#ifdef _PPGE_PLATFORM_WIN


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