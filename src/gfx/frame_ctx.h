#pragma once

#include "gfx/command_type.h"
#include "gfx/resource.h"
#include "gfx/util.h"

#include "types.h"

#include <wrl.h>

class Device;
class SwapChain;
class DescriptorHeap;

struct ID3D12CommandAllocator;

class FrameCtx
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	Resource m_backBuffer;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	uint64 m_fenceValue = 0;

	ALLOW_GFX_ACCESS();
	ComPtr<ID3D12CommandAllocator> GetRawCommandAllocatorHandle() const
	{
		return m_commandAllocator;
	}

public:
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
