#pragma once

#include "gfx/core/fwd.h"
#include "gfx/core/util.h"

#include "gfx/core/enums/format.h"
#include "gfx/core/enums/pipeline_state_subobject_type.h"

#include "types.h"

// TODO: hrt::span ?
#include <span>

template <typename T>
class HeartStreamWriter;

struct PipelineStateInputElementDesc
{
	enum Classification
	{
		PerVertexData,
		PerInstanceData,
	};

	const char* name = nullptr;
	uint32 index = 0;
	ResourceFormat format = ResourceFormat::Unknown;
	uint32 inputSlot = 0;
	uint32 alignedByteOffset = 0xffffffffu;
	Classification classification = PerVertexData;
	uint32 instanceDataStepRate = 0;
};

struct PipelineStateStreamEntry
{
	PipelineStateSubobjectType subobjectType;

	union {
		RootSignature* rootSignature;
		Shader* shader;
		struct
		{
			PipelineStateInputElementDesc* inputElements;
			size_t inputElementCount;
		} inputLayout;
		ResourceFormat depthStencilFormat;
		uint32 topology; // TODO
		struct
		{
			ResourceFormat renderTargetFormats[8];
			uint32 numRenderTargets;
		};
	};
};

struct ID3D12PipelineState;

class PipelineStateObject
{
private:
	ID3D12PipelineState* m_pipeline = nullptr;

	ALLOW_GFX_ACCESS();
	ID3D12PipelineState* GetRawPipelineHandle() const
	{
		return m_pipeline;
	}

	void MeasureStreamSizeRequirement(
		std::span<PipelineStateStreamEntry> stream,
		size_t& pipelineStreamSize,
		size_t& additionalStorageSize);

	void ConvertStreamEntry(
		HeartStreamWriter<size_t>& outStream,
		HeartStreamWriter<size_t>& additionalStorage,
		const PipelineStateStreamEntry& entry);

public:
	PipelineStateObject() = default;
	~PipelineStateObject();

	bool Create(Device& device, std::span<PipelineStateStreamEntry> stream);
	void Destroy();
};
