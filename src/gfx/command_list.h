#pragma once

#include "gfx/command_type.h"
#include "gfx/resource_state.h"
#include "gfx/util.h"

#include "vector.h"

#include <wrl.h>

struct ID3D12GraphicsCommandList;

class Device;
class Resource;
class FrameCtx;
class DescriptorHeap;
class SwapChain;

class CommandList
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	ALLOW_GFX_ACCESS();
	ComPtr<ID3D12GraphicsCommandList> GetRawCommandListHandle() const
	{
		return m_commandList;
	}

public:
	bool Create(const Device& d, const FrameCtx& fc, CommandQueueType type);
	void Destroy();

	void Reset(const FrameCtx& fc);

	void Transition(const Resource& r, ResourceState from, ResourceState to);
	void Clear(const Device& d, const DescriptorHeap& heap, const SwapChain& c, Vector4f color);

	void Close();
};
