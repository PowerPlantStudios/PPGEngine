#include <PPGE.h>

class Game : public PPGE::Application
{
public:
	Game()
	{
		APP_INFO("Application is initialized.");
	}

	~Game()
	{

	}


};


PPGE::Application* PPGE::CreateApplication()
{
	return new Game();
}