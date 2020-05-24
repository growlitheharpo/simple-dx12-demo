#pragma once

#include "gfx/command_type.h"

#include <wrl.h>

struct ID3D12CommandQueue;
class Device;

class CommandQueue
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<ID3D12CommandQueue> m_commandQueue;

public:

	bool Create(const Device& d, CommandQueueType type);
	void Destroy();

	ComPtr<ID3D12CommandQueue> GetRawCommandQueueHandle() const
	{
		return m_commandQueue;
	}
};
