#include "gfx/core/pipeline_state_object.h"

#include "gfx/core/device.h"
#include "gfx/core/root_signature.h"
#include "gfx/core/shader.h"

#include <heart/countof.h>
#include <heart/scope_exit.h>
#include <heart/stl/vector.h>
#include <heart/stream.h>

#include <d3d12.h>
#include <d3dx12.h>

DECLARE_EQUIVALENT_ENUM(PipelineStateSubobjectType, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE);
TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::RootSignature, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE);
TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::VertexShader, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VS);
TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::PixelShader, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::DS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DS);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::HullShader, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_HS);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::GeometryShader, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_GS);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::ComputeShader, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CS);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::StreamOutput, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_STREAM_OUTPUT);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::Blend, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_BLEND);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::SampleMask, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_MASK);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::Rasterizer, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::DepthStencil, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL);
TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::InputLayout, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_INPUT_LAYOUT);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::IBStripCutValue, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_IB_STRIP_CUT_VALUE);
TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::PrimitiveTopology, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY);
TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::RenderTargetFormats, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS);
TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::DepthStencilFormat, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL_FORMAT);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::SampleDesc, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_DESC);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::NodeMask,D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_NODE_MASK);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::CachedPipelineStateObject, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CACHED_PSO);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::Flags, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_FLAGS);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::DepthStencil1, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL1);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::ViewInstancing, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VIEW_INSTANCING);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::AS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_AS);
// TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::MS, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MS);
TEST_EQUIVALENT_ENUM(PipelineStateSubobjectType::MaxValid, D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MAX_VALID);

void PipelineStateObject::MeasureStreamSizeRequirement(
	std::span<PipelineStateStreamEntry> stream,
	size_t& pipelineStreamSize,
	size_t& additionalStorageSize)
{
	pipelineStreamSize = 0;
	additionalStorageSize = 0;

	for (const PipelineStateStreamEntry& entry : stream)
	{
		switch (entry.subobjectType)
		{
		case PipelineStateSubobjectType::RootSignature:
			pipelineStreamSize += sizeof(CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE);
			additionalStorageSize += 0;
			break;

		case PipelineStateSubobjectType::VertexShader:
			pipelineStreamSize += sizeof(CD3DX12_PIPELINE_STATE_STREAM_VS);
			additionalStorageSize += 0;
			break;

		case PipelineStateSubobjectType::PixelShader:
			pipelineStreamSize += sizeof(CD3DX12_PIPELINE_STATE_STREAM_PS);
			additionalStorageSize += 0;
			break;

		case PipelineStateSubobjectType::Reserved03:
		case PipelineStateSubobjectType::Reserved04:
		case PipelineStateSubobjectType::Reserved05:
		case PipelineStateSubobjectType::Reserved06:
		case PipelineStateSubobjectType::Reserved07:
		case PipelineStateSubobjectType::Reserved08:
		case PipelineStateSubobjectType::Reserved09:
		case PipelineStateSubobjectType::Reserved10:
		case PipelineStateSubobjectType::Reserved11:
			HEART_ASSERT(false);
			break;

		case PipelineStateSubobjectType::InputLayout:
			pipelineStreamSize += sizeof(CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT);
			additionalStorageSize += sizeof(D3D12_INPUT_ELEMENT_DESC) * entry.inputLayout.inputElementCount;
			break;

		case PipelineStateSubobjectType::Reserved13:
			HEART_ASSERT(false);
			break;

		case PipelineStateSubobjectType::PrimitiveTopology:
			pipelineStreamSize += sizeof(CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY);
			additionalStorageSize += 0;
			break;

		case PipelineStateSubobjectType::RenderTargetFormats:
			pipelineStreamSize += sizeof(CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS);
			additionalStorageSize += 0;
			break;

		case PipelineStateSubobjectType::DepthStencilFormat:
			pipelineStreamSize += sizeof(CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT);
			additionalStorageSize += 0;
			break;

		case PipelineStateSubobjectType::Reserved17:
		case PipelineStateSubobjectType::Reserved18:
		case PipelineStateSubobjectType::Reserved19:
		case PipelineStateSubobjectType::Reserved20:
		case PipelineStateSubobjectType::Reserved21:
		case PipelineStateSubobjectType::Reserved22:
		case PipelineStateSubobjectType::Reserved23:
		case PipelineStateSubobjectType::Reserved24:
		case PipelineStateSubobjectType::Reserved25:
		default:
			HEART_ASSERT(false);
			break;
		}
	}
}

void PipelineStateObject::ConvertStreamEntry(
	HeartStreamWriter<size_t>& outStream,
	HeartStreamWriter<size_t>& additionalStorage,
	const PipelineStateStreamEntry& entry)
{
	switch (entry.subobjectType)
	{
	case PipelineStateSubobjectType::RootSignature: {
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE rootSignature = entry.rootSignature->GetRawRootSignatureHandle();
		outStream.Write(rootSignature, HeartStreamWriter<size_t>::EnforceAddressof {});
		break;
	}

	case PipelineStateSubobjectType::VertexShader: {
		CD3DX12_PIPELINE_STATE_STREAM_VS vertexShader = CD3DX12_SHADER_BYTECODE(entry.shader->GetRawBlobHandle());
		outStream.Write(vertexShader, HeartStreamWriter<size_t>::EnforceAddressof {});
		break;
	}

	case PipelineStateSubobjectType::PixelShader: {
		CD3DX12_PIPELINE_STATE_STREAM_PS vertexShader = CD3DX12_SHADER_BYTECODE(entry.shader->GetRawBlobHandle());
		outStream.Write(vertexShader, HeartStreamWriter<size_t>::EnforceAddressof {});
		break;
	}

	case PipelineStateSubobjectType::Reserved03:
	case PipelineStateSubobjectType::Reserved04:
	case PipelineStateSubobjectType::Reserved05:
	case PipelineStateSubobjectType::Reserved06:
	case PipelineStateSubobjectType::Reserved07:
	case PipelineStateSubobjectType::Reserved08:
	case PipelineStateSubobjectType::Reserved09:
	case PipelineStateSubobjectType::Reserved10:
	case PipelineStateSubobjectType::Reserved11:
		HEART_ASSERT(false);
		break;

	case PipelineStateSubobjectType::InputLayout: {
		D3D12_INPUT_ELEMENT_DESC* startPosition = reinterpret_cast<D3D12_INPUT_ELEMENT_DESC*>(additionalStorage.GetCurrentHead());

		auto inputDescSpan = std::span(entry.inputLayout.inputElements, entry.inputLayout.inputElementCount);
		for (PipelineStateInputElementDesc& inputElement : inputDescSpan)
		{
			D3D12_INPUT_ELEMENT_DESC desc = {};
			desc.SemanticName = inputElement.name;
			desc.SemanticIndex = inputElement.index;
			desc.Format = DXGI_FORMAT(inputElement.format);
			desc.InputSlot = inputElement.inputSlot;
			desc.AlignedByteOffset = inputElement.alignedByteOffset;
			desc.InputSlotClass = inputElement.classification == PipelineStateInputElementDesc::PerVertexData ? D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA : D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
			desc.InstanceDataStepRate = inputElement.instanceDataStepRate;

			additionalStorage.Write(desc);
		}

		CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT inputLayout = D3D12_INPUT_LAYOUT_DESC {startPosition, uint32(entry.inputLayout.inputElementCount)};
		outStream.Write(inputLayout, HeartStreamWriter<size_t>::EnforceAddressof {});
		break;
	}

	case PipelineStateSubobjectType::Reserved13:
		HEART_ASSERT(false);
		break;

	case PipelineStateSubobjectType::PrimitiveTopology: {
		CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY primitiveTopology = D3D12_PRIMITIVE_TOPOLOGY_TYPE(entry.topology);
		outStream.Write(primitiveTopology, HeartStreamWriter<size_t>::EnforceAddressof {});
		break;
	}

	case PipelineStateSubobjectType::RenderTargetFormats: {
		D3D12_RT_FORMAT_ARRAY formatArray;
		formatArray.NumRenderTargets = entry.numRenderTargets;
		for (int i = 0; i < HeartCountOf(formatArray.RTFormats); ++i)
			formatArray.RTFormats[i] = DXGI_FORMAT(entry.renderTargetFormats[i]);

		CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS rtFormat = formatArray;
		outStream.Write(rtFormat, HeartStreamWriter<size_t>::EnforceAddressof {});
		break;
	}

	case PipelineStateSubobjectType::DepthStencilFormat: {
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT dsFormat = DXGI_FORMAT(entry.depthStencilFormat);
		outStream.Write(dsFormat, HeartStreamWriter<size_t>::EnforceAddressof {});
		break;
	}

	case PipelineStateSubobjectType::Reserved17:
	case PipelineStateSubobjectType::Reserved18:
	case PipelineStateSubobjectType::Reserved19:
	case PipelineStateSubobjectType::Reserved20:
	case PipelineStateSubobjectType::Reserved21:
	case PipelineStateSubobjectType::Reserved22:
	case PipelineStateSubobjectType::Reserved23:
	case PipelineStateSubobjectType::Reserved24:
	case PipelineStateSubobjectType::Reserved25:
	default:
		HEART_ASSERT(false);
		break;
	}
}

PipelineStateObject::~PipelineStateObject()
{
	Destroy();
}

bool PipelineStateObject::Create(Device& device, std::span<PipelineStateStreamEntry> stream)
{
	HeartScopeGuard failureGuard([&] {
		Destroy();
	});

	size_t convertedStreamSize = 0;
	size_t additionalStorageSize = 0;
	MeasureStreamSizeRequirement(stream, convertedStreamSize, additionalStorageSize);

	hrt::vector<byte> convertedStreamBuffer(convertedStreamSize, 0);
	size_t convertedStreamWriteHead = 0;
	HeartStreamWriter<size_t> convertedStreamWriter(convertedStreamBuffer.data(), convertedStreamBuffer.size(), convertedStreamWriteHead);

	hrt::vector<byte> additionalStorageBuffer(additionalStorageSize, 0);
	size_t additionalStorageWriteHead = 0;
	HeartStreamWriter<size_t> additionalStorageWriter(additionalStorageBuffer.data(), additionalStorageBuffer.size(), additionalStorageWriteHead);

	for (const auto& entry : stream)
		ConvertStreamEntry(convertedStreamWriter, additionalStorageWriter, entry);

	D3D12_PIPELINE_STATE_STREAM_DESC pipelineStreamDesc = {};
	pipelineStreamDesc.pPipelineStateSubobjectStream = convertedStreamBuffer.data();
	pipelineStreamDesc.SizeInBytes = convertedStreamBuffer.size();

	HRESULT r = device.GetRawDeviceHandle()->CreatePipelineState(&pipelineStreamDesc, IID_PPV_ARGS(&m_pipeline));
	if (!SUCCEEDED(r))
		return false;

	failureGuard.Dismiss();
	return true;
}

void PipelineStateObject::Destroy()
{
}
