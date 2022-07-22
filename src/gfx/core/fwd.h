#pragma once

#ifndef GFX_FWD
#define GFX_FWD

// clang-format off
#define ALL_GFX_CLASSES(prefix, ...)  \
	prefix CommandList __VA_ARGS__            \
	prefix CommandQueue __VA_ARGS__           \
	prefix DescriptorHeap __VA_ARGS__         \
	prefix DestroyQueue __VA_ARGS__           \
	prefix Device __VA_ARGS__                 \
	prefix Fence __VA_ARGS__                  \
	prefix FrameCtx __VA_ARGS__               \
	prefix Resource __VA_ARGS__               \
	prefix SwapChain __VA_ARGS__              \
	prefix Window __VA_ARGS__

#define ALL_GFX_STRUCTS(prefix, ...)        \
	prefix ResourceCreateDesc __VA_ARGS__           \
	prefix SubresourceData __VA_ARGS__              \
	prefix UpdateSubresourceData __VA_ARGS__
// clang-format on

#define ALL_GFX_TYPES(prefix) \
	ALL_GFX_CLASSES(prefix);  \
	ALL_GFX_STRUCTS(prefix)

// Forward declare all classes
ALL_GFX_CLASSES(class, ;)

// Forward declare all structs
ALL_GFX_STRUCTS(struct, ;)

static constexpr size_t MaxBufferedFrames = 3;

#endif
