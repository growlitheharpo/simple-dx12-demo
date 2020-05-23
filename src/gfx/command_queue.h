#pragma once

#include <wrl.h>

struct ID3D12CommandQueue;
class Device;

enum class CommandQueueType : int
{
	Direct,
	Bundle,
	Compute,
	Copy,
	VideoDecode,
	VideoProcess,
	VideoEncode,
};

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
