#include "Game.h"
#include "Timer.h"

Game::Game(const std::string& windowTitle)
	: Window(800, 600, windowTitle.c_str())
{
}

int Game::Run()
{
	Timer timer;
	float lastTime = 0;

	std::optional<int> result;

	while (!((result = Window::ProcessMessages())))
	{
		const float mark = timer.Peek();
		const float deltaTime = mark - lastTime;
		lastTime = mark;

		Update(deltaTime);
		Draw();
	}

	return result.value();
}
