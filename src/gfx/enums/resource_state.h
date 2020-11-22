#pragma once

#include "types.h"

// Equivalent to D3D12_RESOURCE_STATES
enum class ResourceState : uint32
{
	Common = 0,

	VertexAndConstantBuffer = 1 << 0,
	IndexBuffer = 1 << 1,
	RenderTarget = 1 << 2,
	UnorderedAccess = 1 << 3,
	DepthWrite = 1 << 4,
	DepthRead = 1 << 5,
	NonPixelShaderResource = 1 << 6,
	PixelShaderResource = 1 << 7,
	StreamOut = 1 << 8,
	IndirectArgument = 1 << 9,
	CopyDest = 1 << 10,
	CopySource = 1 << 11,
	ResolveDest = 1 << 12,
	ResolveSource = 1 << 13,

	RaytacingAccelerationStructure = 0x400000,
	ShadingRateSource = 0x1000000,

	GenericRead = (VertexAndConstantBuffer | IndexBuffer | NonPixelShaderResource | PixelShaderResource | IndirectArgument | CopySource),

	Present = 0,
	Predication = 1 << 9,

	VideoDecodeRead = 1 << 16,
	VideoDecodeWrite = 1 << 17,
	VideoProcessRead = 1 << 18,
	VideoProcessWrite = 1 << 19,
	VideoEncodeRead = 1 << 21,
	VideoEncodeWrite = 1 << 23,
};
