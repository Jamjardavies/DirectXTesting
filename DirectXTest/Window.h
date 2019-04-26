#pragma once

#include "JJWindows.h"

#define WINDOW_STYLE	WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU

class Window
{
private:
	class WindowClass
	{
	private:
		static constexpr const char* WndClassName = "JJ Engine Window";
		static WindowClass WndClass;
		HINSTANCE m_hInst;

		WindowClass() noexcept;
		~WindowClass();

	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
	};

	int m_width;
	int m_height;
	HWND m_hwnd;

	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Window(int width, int height, const char* name) noexcept;
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
};

