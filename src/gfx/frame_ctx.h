#pragma once

#include "gfx/command_type.h"

#include "types.h"

#include <wrl.h>

class Device;
class SwapChain;
class DescriptorHeap;

struct ID3D12Resource;
struct ID3D12CommandAllocator;

class FrameCtx
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<ID3D12Resource> m_backBuffer;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	uint64 m_fenceValue = 0;

public:
	bool Create(size_t index, const Device& d, const SwapChain& sc, const DescriptorHeap& h, CommandQueueType allocatorType);
	void Destroy();

	void SetFenceValue(uint64 v)
	{
		m_fenceValue = v;
	}

	uint64 GetFenceValue() const
	{
		return m_fenceValue;
	}

	ComPtr<ID3D12Resource> GetRawBackBufferHandle() const
	{
		return m_backBuffer;
	}

	ComPtr<ID3D12CommandAllocator> GetRawCommandAllocatorHandle() const
	{
		return m_commandAllocator;
	}
};
