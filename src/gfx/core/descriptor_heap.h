#pragma once

#include "gfx/core/fwd.h"

#include "gfx/core/enums/descriptor_heap_type.h"
#include "gfx/core/util.h"

#include "types.h"

struct ID3D12DescriptorHeap;

class DescriptorHeap
{
private:
	ID3D12DescriptorHeap* m_heap = nullptr;

	ALLOW_GFX_ACCESS();
	ID3D12DescriptorHeap* GetRawDescriptorHeapHandle() const
	{
		return m_heap;
	}

public:
	DescriptorHeap() = default;
	~DescriptorHeap();

	bool Create(const Device& d, DescriptorHeapType type, uint32 numDescriptors);
	void Destroy();
};
