#pragma once

#include "types.h"

#include <wrl.h>

struct ID3D12DescriptorHeap;
class Device;

enum class DescriptorHeapType : int
{
	Cbv_Srv_Uav,
	Sampler,
	RTV,
	DSV,

	Count,
};

class DescriptorHeap
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<ID3D12DescriptorHeap> m_heap;

public:
	bool Create(const Device& d, DescriptorHeapType type, uint32 numDescriptors);
	void Destroy();

	ComPtr<ID3D12DescriptorHeap> GetRawDescriptorHeapHandle() const
	{
		return m_heap;
	}
};
