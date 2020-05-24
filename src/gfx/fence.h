#pragma once

#include "types.h"

#include <wrl.h>

#include <chrono>

struct ID3D12Fence;
typedef void* HANDLE;

class CommandQueue;
class Device;

using milliseconds = std::chrono::milliseconds;

class Fence
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<ID3D12Fence> m_fence;
	HANDLE m_fenceEvent = nullptr;
	uint64 m_fenceValue = 0;

public:
	bool Create(const Device& d);
	void Destroy();

	uint64 Signal(const CommandQueue& q);
	void WaitForFence(uint64 value, milliseconds waitDuration = milliseconds::max());

	void Flush(const CommandQueue& q)
	{
		WaitForFence(Signal(q));
	}
};
