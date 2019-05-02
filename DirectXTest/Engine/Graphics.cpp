#include "Graphics.h"

#include <sstream>

#pragma comment(lib, "d3d11.lib")

#define GFX_EXCEPT_NOINFO(hr)			Graphics::HrException(__LINE__, __FILE__, hr);
#define GFX_THROW_NOINFO(hrCall)		if (FAILED(hr = hrCall)) throw Graphics::HrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG

#define GFX_EXCEPT(hr)					Graphics::HrException(__LINE__, __FILE__, hr, m_infoManager.GetMessages())
#define GFX_THROW_INFO(hrCall)			m_infoManager.Set(); if (FAILED(hr = hrCall)) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr)	Graphics::DeviceRemovedException(__LINE__, __FILE__, hr, m_infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call)		m_infoManager.Set(); (call); { auto v = m_infoManager.GetMessages(); if(!v.empty()) { throw Graphics::InfoException( __LINE__,__FILE__,v); } }

#else

#define GFX_EXCEPT(hr)					Graphics::HrException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO(hrCall)			GFX_THROW_NOINFO(hrCall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr)	Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO_ONLY(call)		call

#endif

Graphics::HrException::HrException(const int line, const char* file, const HRESULT hr, std::vector<std::string> infoMsgs)
	: BaseException(line, file), m_hr(hr)
{
	// Join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		m_info += m;
		m_info.push_back('\n');
	}

	// Remove final newline if exists
	if (!m_info.empty())
	{
		m_info.pop_back();
	}
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	char* pMsgBuf = nullptr;
	const DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, m_hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

	if (nMsgLen == 0)
	{
		return "Unidentified Error Code!";
	}

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);

	return errorString;
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	return GetErrorString();
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;

	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << std::endl
		//<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;

	if (!m_info.empty())
	{
		oss << std::endl << "[Error Info]" << std::endl << GetErrorInfo() << std::endl << std::endl;
	}

	oss << GetOriginString();

	m_whatBuffer = oss.str();

	return m_whatBuffer.c_str();
}

Graphics::Graphics(const HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};

	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr;

	UINT swapCreateFlags = 0u;

#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	GFX_THROW_INFO(
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, swapCreateFlags,
			nullptr, 0, D3D11_SDK_VERSION, &sd, &m_swap, &m_device, nullptr, &m_context));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

	GFX_THROW_INFO(m_swap->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
	GFX_THROW_INFO(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_target));
}

void Graphics::ClearBuffer(const float r, const float g, const float b) const noexcept
{
	const float colour[] = { r, g, b, 1.0f };

	m_context->ClearRenderTargetView(m_target.Get(), colour);
}

void Graphics::Present()
{
	HRESULT hr;

#ifndef NDEBUG
	m_infoManager.Set();
#endif

	if (FAILED(hr = m_swap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(m_device->GetDeviceRemovedReason());
		}
		
		throw GFX_EXCEPT(hr);
	}
}
