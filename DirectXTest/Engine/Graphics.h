#pragma once

#include "JJWindows.h"
#include "BaseException.h"
#include "Debug/DxgiInfoManager.h"

#include <d3d11.h>

class Graphics
{
public:
	class HrException : public BaseException
	{
	private:
		HRESULT m_hr;
		std::string m_info;

	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {});

		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;

		virtual const char* what() const noexcept override;

		virtual const char* GetType() const noexcept override { return "HResult Exception"; }

		HRESULT GetErrorCode() const noexcept { return m_hr; }
		std::string GetErrorInfo() const noexcept { return m_info; }
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

	private:
		std::string m_reason;

	public:
		const char* GetType() const noexcept override { return "Device Removed Exception"; }
	};

private:
#ifndef NDEBUG
	DxgiInfoManager m_infoManager;
#endif

	Microsoft::WRL::ComPtr<ID3D11Device> m_device = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_target = nullptr;

public:
	Graphics(HWND hwnd);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	
	void ClearBuffer(float r, float g, float b) const noexcept; // ToDo: Change to our Colour struct.
	void Present();
};

