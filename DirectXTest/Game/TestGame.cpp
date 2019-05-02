#include "TestGame.h"

TestGame::TestGame()
	: Game("Test Game")
{

}

TestGame::~TestGame()
{

}

float time = 0;

void TestGame::Update(const float deltaTime)
{
	time += deltaTime;
	Window.SetTitle(std::to_string(time));
}

void TestGame::Draw()
{
	const float c = sin(time) * 0.5f + 0.5f;

	Window.Gfx().ClearBuffer(c, c * 0.5f, 1.0);
	Window.Gfx().Present();
}
