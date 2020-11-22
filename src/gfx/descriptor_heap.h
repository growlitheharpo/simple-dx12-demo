#pragma once

#include "gfx/enums/descriptor_heap_type.h"
#include "gfx/util.h"

#include "types.h"

#include <wrl.h>

struct ID3D12DescriptorHeap;
class Device;

class DescriptorHeap
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<ID3D12DescriptorHeap> m_heap;

	ALLOW_GFX_ACCESS();
	ComPtr<ID3D12DescriptorHeap> GetRawDescriptorHeapHandle() const
	{
		return m_heap;
	}

public:
	bool Create(const Device& d, DescriptorHeapType type, uint32 numDescriptors);
	void Destroy();
};
