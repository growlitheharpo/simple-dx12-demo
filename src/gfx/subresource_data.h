#pragma once

#include "gfx/fwd.h"

#include "types.h"

struct SubresourceData
{
	const void* data;
	intptr rowPitch;
	intptr slicePitch;
};

struct UpdateSubresourceData
{
	uint64 intermediateOffset;
	uint32 firstSubresource;
	uint32 numSubresources;
	SubresourceData* srcData;
};
