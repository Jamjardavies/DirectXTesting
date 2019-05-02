#pragma once

#include "JJWindows.h"
#include "BaseException.h"
#include "Graphics.h"

#include <memory>
#include <optional>

#define WINDOW_STYLE			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU

// Error Exception Helper Macro
#define HWND_EXCEPT(hr)			Window::Exception(__LINE__, __FILE__, hr)
#define HWND_LAST_EXCEPT()		Window::Exception(__LINE__, __FILE__, GetLastError())

class Window
{
public:
	class Exception : public BaseException
	{
	private:
		HRESULT m_hr;

	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;

		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		virtual const char* what() const noexcept override;

		HRESULT GetErrorCode() const noexcept { return m_hr; }
		std::string GetErrorString() const noexcept { return TranslateErrorCode(m_hr); }
		virtual const char* GetType() const noexcept override { return "Window Exception"; }
	};

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

	std::unique_ptr<Graphics> m_graphics;

	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Window(int width, int height, const char* name);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetTitle(const std::string& title) const;

	HWND Handle() const { return m_hwnd; }
	Graphics& Gfx() const { return *m_graphics; }

	static std::optional<int> ProcessMessages(HWND hwnd);
};