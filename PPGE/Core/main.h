#pragma once

#ifdef PPGE_PLATFORM_WIN


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