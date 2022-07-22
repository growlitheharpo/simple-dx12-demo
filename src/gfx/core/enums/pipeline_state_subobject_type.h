#pragma once

enum class PipelineStateSubobjectType
{
	RootSignature = 0,
	VertexShader,
	PixelShader,
	Reserved03, // DS,
	Reserved04, // HullShader,
	Reserved05, // GeometryShader,
	Reserved06, // ComputeShader,
	Reserved07, // StreamOutput,
	Reserved08, // Blend,
	Reserved09, // SampleMask,
	Reserved10, // Rasterizer,
	Reserved11, // DepthStencil,
	InputLayout,
	Reserved13, // IBStripCutValue,
	PrimitiveTopology,
	RenderTargetFormats,
	DepthStencilFormat,
	Reserved17, // SampleDesc,
	Reserved18, // NodeMask,
	Reserved19, // CachedPipelineStateObject,
	Reserved20, // Flags,
	Reserved21, // DepthStencil1,
	Reserved22, // ViewInstancing,

	Reserved23,
	Reserved24, // AS,
	Reserved25, // MS,

	MaxValid,
};
