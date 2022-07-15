#pragma once

#include "gfx/fwd.h"

#include "types.h"

#include <chrono>

struct ID3D12Fence;
typedef void* HANDLE;

using milliseconds = std::chrono::milliseconds;

class Fence
{
private:
	ID3D12Fence* m_fence = nullptr;
	HANDLE m_fenceEvent = nullptr;
	uint64 m_fenceValue = 0;

public:
	Fence() = default;
	~Fence();

	bool Create(const Device& d);
	void Destroy();

	uint64 Signal(const CommandQueue& q);
	void WaitForFence(uint64 value, milliseconds waitDuration = milliseconds::max());

	void Flush(const CommandQueue& q)
	{
		WaitForFence(Signal(q));
	}
};
