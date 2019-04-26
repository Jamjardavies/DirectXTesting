#include "Window.h"

// Window Class stuff.
Window::WindowClass Window::WindowClass::WndClass;

Window::WindowClass::WindowClass() noexcept
	: m_hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(WndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return WndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return WndClass.m_hInst;
}


// Window stuff.

Window::Window(const int width, const int height, const char* name) noexcept
	: m_width(width), m_height(height)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	AdjustWindowRect(&wr, WINDOW_STYLE, false);

	// Create window and get hWnd.
	m_hwnd = CreateWindow(WindowClass::GetName(), name, WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, 
		wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::GetInstance(), this);

	// Show the window.
	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(m_hwnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg != WM_NCCREATE)
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	// Get the param passed in from CreateWidnow() to store window class pointer.
	const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
	Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

	// Set WinAPI managed user data to store ptr to window class.
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

	// Set message proc to normal (non-setup) handler now setup is complete.
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

	// Forward message to window class handler.
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Get the ptr to our Window class.
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Forward the message.
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
{
	switch (msg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		default:
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}