#pragma once

#include "gfx/core/enums/command_type.h"
#include "gfx/core/resource.h"
#include "gfx/core/util.h"

#include "types.h"

class Device;
class SwapChain;
class DescriptorHeap;

struct ID3D12CommandAllocator;

class FrameCtx
{
private:
	Resource m_backBuffer;
	ID3D12CommandAllocator* m_commandAllocator = nullptr;
	uint64 m_fenceValue = 0;

	ALLOW_GFX_ACCESS();
	ID3D12CommandAllocator* GetRawCommandAllocatorHandle() const
	{
		return m_commandAllocator;
	}

public:
	FrameCtx() = default;
	~FrameCtx();

	bool Create(size_t index, const Device& d, const SwapChain& sc, const DescriptorHeap& h, CommandQueueType allocatorType);
	void Destroy();

	void ResetCommandAllocator();

	void SetFenceValue(uint64 v)
	{
		m_fenceValue = v;
	}

	uint64 GetFenceValue() const
	{
		return m_fenceValue;
	}

	Resource& GetBackBuffer()
	{
		return m_backBuffer;
	}

	const Resource& GetBackBuffer() const
	{
		return m_backBuffer;
	}
};
