#pragma once

#ifndef GFX_FWD
#define GFX_FWD

#define ALL_GFX_CLASSES(prefix) \
	prefix CommandList;         \
	prefix CommandQueue;        \
	prefix DescriptorHeap;      \
	prefix Device;              \
	prefix Fence;               \
	prefix FrameCtx;            \
	prefix Resource;            \
	prefix SwapChain;           \
	prefix Window;

#define ALL_GFX_STRUCTS(prefix) \
	prefix ResourceCreateDesc;  \
	prefix SubresourceData;     \
	prefix UpdateSubresourceData;

#define ALL_GFX_TYPES(prefix) \
	ALL_GFX_CLASSES(prefix);  \
	ALL_GFX_STRUCTS(prefix)

// Forward declare all classes
ALL_GFX_CLASSES(class);

// Forward declare all structs
ALL_GFX_STRUCTS(struct);

#endif
