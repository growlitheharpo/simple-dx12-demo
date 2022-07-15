#pragma once

#include "types.h"

// Equivalent to D3D12_HEAP_FLAGS
enum class HeapFlags : uint32
{
	None = 0,
	Shared = 1 << 0,

	DenyBuffers = 1 << 2,
	AllowDisplay = 1 << 3,

	SharedCrossAdapter = 1 << 5,
	DenyRtDsTextures = 1 << 6,
	DenyNonRtDsTextures = 1 << 7,
	HardwareProtected = 1 << 8,
	AllowWriteWatch = 1 << 9,
	AllowShaderAtomics = 1 << 10,

	AllowAllBuffersAndTextures = (None),
	AllowOnlyBuffers = (DenyRtDsTextures | DenyNonRtDsTextures),
	AllowOnlyNonRtDsTextures = (DenyRtDsTextures | DenyBuffers),
	AllowOnlyRtDsTextures = (DenyNonRtDsTextures | DenyBuffers),
};
