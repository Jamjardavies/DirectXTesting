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

}
