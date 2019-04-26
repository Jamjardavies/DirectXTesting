#include "Window.h"
#include "Game.h"

#include <optional>

#include "../Game/TestGame.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		Game* game = new TestGame();

		Window wnd(800, 600, "My Window");

		std::optional<int> result;

		while (!((result = Window::ProcessMessages())))
		{
			game->Update(0.0f);
			game->Draw(0.0f);
		}

		delete game;

		return result.value();
	}
	catch (const BaseException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}
