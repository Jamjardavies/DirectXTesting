#pragma once
#include "Window.h"

class Game
{
private:
	

protected:
	Window Window;

public:
	Game(const std::string& windowTitle);
	virtual ~Game() = default;

	int Run();

	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
};

