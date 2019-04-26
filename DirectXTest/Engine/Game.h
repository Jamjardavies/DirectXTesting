#pragma once

class Game
{
public:
	Game();
	virtual ~Game();

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(float deltaTime) = 0;
};

