#pragma once

#include "types.h"

// Equivalent to D3D12_HEAP_TYPE
enum class HeapType : uint32
{
	Default = 1,
	Upload = 2,
	Readback = 3,
	Custom = 4,
};
