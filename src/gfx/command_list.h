#pragma once

#include "gfx/command_type.h"

#include <wrl.h>

struct ID3D12GraphicsCommandList;

class Device;
class FrameCtx;

class CommandList
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

public:
	bool Create(const Device& d, const FrameCtx& fc, CommandQueueType type);
	void Destroy();

	ComPtr<ID3D12GraphicsCommandList> GetRawCommandListHandle() const
	{
		return m_commandList;
	}
};
