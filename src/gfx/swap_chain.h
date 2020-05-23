#pragma once

#include "types.h"

#include <wrl.h>

struct IDXGISwapChain4;
class CommandQueue;
class Window;

class SwapChain
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<IDXGISwapChain4> m_swapChain;
	int m_tearingSupport = -1;

	void CheckForTearingSupport();

public:
	bool Create(const Window& window, const CommandQueue& commandQueue, uint32 width, uint32 height, uint32 bufferCount);
	void Destroy();

	bool HasTearingSupport()
	{
		if (m_tearingSupport == -1)
			CheckForTearingSupport();

		return m_tearingSupport > 0;
	}

	ComPtr<IDXGISwapChain4> GetRawSwapChainHandle() const 
	{
		return m_swapChain;
	}
};
