#pragma once

#include "gfx/core/fwd.h"
#include "gfx/core/util.h"

#include "types.h"

struct IDXGISwapChain4;

class SwapChain
{
private:
	IDXGISwapChain4* m_swapChain = nullptr;
	int m_tearingSupport = -1;

	void CheckForTearingSupport();

	ALLOW_GFX_ACCESS();
	IDXGISwapChain4* GetRawSwapChainHandle() const
	{
		return m_swapChain;
	}

public:
	SwapChain() = default;
	~SwapChain();

	bool Create(const Window& window, const CommandQueue& commandQueue, uint32 width, uint32 height, uint32 bufferCount);
	void Destroy();

	bool HasTearingSupport()
	{
		if (m_tearingSupport == -1)
			CheckForTearingSupport();

		return m_tearingSupport > 0;
	}

	uint32 GetCurrentFrameIndex() const;

	void Present(bool useVsync);
};
