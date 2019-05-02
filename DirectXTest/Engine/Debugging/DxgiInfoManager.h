#pragma once

#include "JJWindows.h"

#include <wrl.h>
#include <vector>
#include <dxgidebug.h>

class DxgiInfoManager
{
private:
	unsigned long long m_next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue>  m_dxgiInfoQueue = nullptr;

public:
	DxgiInfoManager();
	~DxgiInfoManager() = default;

	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;

	void Set() noexcept;
	std::vector<std::string> GetMessages() const;
};

