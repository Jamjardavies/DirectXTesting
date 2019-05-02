#include "Debug/DxgiInfoManager.h"
#include "Window.h"
#include "Graphics.h"

#include <dxgidebug.h>
#include <memory>

#pragma comment(lib, "dxguid.lib")

#define GFX_THROW_NOINFO(hrCall) if (FAILED(hr = hrCall)) throw Graphics::HrException(__LINE__, __FILE__, hr)

DxgiInfoManager::DxgiInfoManager()
{
	// Define the function signature of DXGIGetDebugInterface.
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// Load the DLL that contains the function DXGIGetDebugInterface.
	const HMODULE hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

	if (hModDxgiDebug == nullptr)
	{
		throw HWND_LAST_EXCEPT();
	}

	const DXGIGetDebugInterface DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));

	if (DxgiGetDebugInterface == nullptr)
	{
		throw HWND_LAST_EXCEPT();
	}

	HRESULT hr;
	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &m_dxgiInfoQueue));
}

void DxgiInfoManager::Set() noexcept
{
	m_next = m_dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const UINT64 end = m_dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for (auto i = m_next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;

		GFX_THROW_NOINFO(m_dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		GFX_THROW_NOINFO(m_dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));

		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}
