#pragma once

#include "../Engine/Game.h"

class TestGame : public Game
{
public:
	TestGame();
	virtual ~TestGame() override;

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
};

