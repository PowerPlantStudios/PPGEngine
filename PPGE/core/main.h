#pragma once

extern PPGE::ApplicationBase *PPGE::CreateApplication();

#if defined(PPGE_PLATFORM_WIN)
int WINAPI wWinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, PWSTR p_cmdline, int n_cmd_show)
{
#if defined(PPGE_DEBUG)
    AllocConsole();
#endif
#else
int main(int argc, char **argv)
{
#endif
    PPGE::ApplicationBase *application = PPGE::CreateApplication();
    application->StartUp();
    application->Run();
    application->ShutDown();
    delete application;
#if defined(PPGE_PLATFORM_WIN) && defined(PPGE_DEBUG)
    FreeConsole();
#endif
    return 0;
}
