#include "gfx/core/resource.h"

#include "gfx/core/command_list.h"
#include "gfx/core/device.h"
#include "gfx/core/subresource_data.h"
#include "gfx/core/util.h"

#include <heart/scope_exit.h>

#include <d3d12.h>
#include <d3dx12.h>

DECLARE_EQUIVALENT_ENUM(ResourceState, D3D12_RESOURCE_STATES);
TEST_EQUIVALENT_ENUM(ResourceState::Common, D3D12_RESOURCE_STATE_COMMON);
TEST_EQUIVALENT_ENUM(ResourceState::VertexAndConstantBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
TEST_EQUIVALENT_ENUM(ResourceState::IndexBuffer, D3D12_RESOURCE_STATE_INDEX_BUFFER);
TEST_EQUIVALENT_ENUM(ResourceState::RenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET);
TEST_EQUIVALENT_ENUM(ResourceState::UnorderedAccess, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
TEST_EQUIVALENT_ENUM(ResourceState::DepthWrite, D3D12_RESOURCE_STATE_DEPTH_WRITE);
TEST_EQUIVALENT_ENUM(ResourceState::DepthRead, D3D12_RESOURCE_STATE_DEPTH_READ);
TEST_EQUIVALENT_ENUM(ResourceState::NonPixelShaderResource, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
TEST_EQUIVALENT_ENUM(ResourceState::PixelShaderResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
TEST_EQUIVALENT_ENUM(ResourceState::StreamOut, D3D12_RESOURCE_STATE_STREAM_OUT);
TEST_EQUIVALENT_ENUM(ResourceState::IndirectArgument, D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);
TEST_EQUIVALENT_ENUM(ResourceState::CopyDest, D3D12_RESOURCE_STATE_COPY_DEST);
TEST_EQUIVALENT_ENUM(ResourceState::CopySource, D3D12_RESOURCE_STATE_COPY_SOURCE);
TEST_EQUIVALENT_ENUM(ResourceState::ResolveDest, D3D12_RESOURCE_STATE_RESOLVE_DEST);
TEST_EQUIVALENT_ENUM(ResourceState::ResolveSource, D3D12_RESOURCE_STATE_RESOLVE_SOURCE);
TEST_EQUIVALENT_ENUM(ResourceState::RaytacingAccelerationStructure, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);
TEST_EQUIVALENT_ENUM(ResourceState::ShadingRateSource, D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE);
TEST_EQUIVALENT_ENUM(ResourceState::GenericRead, D3D12_RESOURCE_STATE_GENERIC_READ);
TEST_EQUIVALENT_ENUM(ResourceState::Present, D3D12_RESOURCE_STATE_PRESENT);
TEST_EQUIVALENT_ENUM(ResourceState::Predication, D3D12_RESOURCE_STATE_PREDICATION);
TEST_EQUIVALENT_ENUM(ResourceState::VideoDecodeRead, D3D12_RESOURCE_STATE_VIDEO_DECODE_READ);
TEST_EQUIVALENT_ENUM(ResourceState::VideoDecodeWrite, D3D12_RESOURCE_STATE_VIDEO_DECODE_WRITE);
TEST_EQUIVALENT_ENUM(ResourceState::VideoProcessRead, D3D12_RESOURCE_STATE_VIDEO_PROCESS_READ);
TEST_EQUIVALENT_ENUM(ResourceState::VideoProcessWrite, D3D12_RESOURCE_STATE_VIDEO_PROCESS_WRITE);
TEST_EQUIVALENT_ENUM(ResourceState::VideoEncodeRead, D3D12_RESOURCE_STATE_VIDEO_ENCODE_READ);
TEST_EQUIVALENT_ENUM(ResourceState::VideoEncodeWrite, D3D12_RESOURCE_STATE_VIDEO_ENCODE_WRITE);

DECLARE_EQUIVALENT_ENUM(ResourceFlags, D3D12_RESOURCE_FLAGS);
TEST_EQUIVALENT_ENUM(ResourceFlags::None, D3D12_RESOURCE_FLAG_NONE);
TEST_EQUIVALENT_ENUM(ResourceFlags::AllowRenderTarget, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
TEST_EQUIVALENT_ENUM(ResourceFlags::AllowDepthStencil, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
TEST_EQUIVALENT_ENUM(ResourceFlags::AllowUnorderedAccess, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
TEST_EQUIVALENT_ENUM(ResourceFlags::DenyShaderResource, D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);
TEST_EQUIVALENT_ENUM(ResourceFlags::AllowCrossAdapter, D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER);
TEST_EQUIVALENT_ENUM(ResourceFlags::AllowSimultaneousAccess, D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS);
TEST_EQUIVALENT_ENUM(ResourceFlags::VideoDecodeReferenceOnly, D3D12_RESOURCE_FLAG_VIDEO_DECODE_REFERENCE_ONLY);

DECLARE_EQUIVALENT_ENUM(HeapType, D3D12_HEAP_TYPE);
TEST_EQUIVALENT_ENUM(HeapType::Default, D3D12_HEAP_TYPE_DEFAULT);
TEST_EQUIVALENT_ENUM(HeapType::Upload, D3D12_HEAP_TYPE_UPLOAD);
TEST_EQUIVALENT_ENUM(HeapType::Readback, D3D12_HEAP_TYPE_READBACK);
TEST_EQUIVALENT_ENUM(HeapType::Custom, D3D12_HEAP_TYPE_CUSTOM);

DECLARE_EQUIVALENT_ENUM(HeapFlags, D3D12_HEAP_FLAGS);
TEST_EQUIVALENT_ENUM(HeapFlags::None, D3D12_HEAP_FLAG_NONE);
TEST_EQUIVALENT_ENUM(HeapFlags::Shared, D3D12_HEAP_FLAG_SHARED);
TEST_EQUIVALENT_ENUM(HeapFlags::DenyBuffers, D3D12_HEAP_FLAG_DENY_BUFFERS);
TEST_EQUIVALENT_ENUM(HeapFlags::AllowDisplay, D3D12_HEAP_FLAG_ALLOW_DISPLAY);
TEST_EQUIVALENT_ENUM(HeapFlags::SharedCrossAdapter, D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER);
TEST_EQUIVALENT_ENUM(HeapFlags::DenyRtDsTextures, D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES);
TEST_EQUIVALENT_ENUM(HeapFlags::DenyNonRtDsTextures, D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES);
TEST_EQUIVALENT_ENUM(HeapFlags::HardwareProtected, D3D12_HEAP_FLAG_HARDWARE_PROTECTED);
TEST_EQUIVALENT_ENUM(HeapFlags::AllowWriteWatch, D3D12_HEAP_FLAG_ALLOW_WRITE_WATCH);
TEST_EQUIVALENT_ENUM(HeapFlags::AllowShaderAtomics, D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS);
TEST_EQUIVALENT_ENUM(HeapFlags::AllowAllBuffersAndTextures, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES);
TEST_EQUIVALENT_ENUM(HeapFlags::AllowOnlyBuffers, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS);
TEST_EQUIVALENT_ENUM(HeapFlags::AllowOnlyNonRtDsTextures, D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES);
TEST_EQUIVALENT_ENUM(HeapFlags::AllowOnlyRtDsTextures, D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES);

Resource::~Resource()
{
	Destroy();
}

bool Resource::CreateCommittedResource(const Device& d, const ResourceCreateDesc& desc)
{
	HeartScopeGuard destroyGuard([&] {
		Destroy();
	});

	auto device = d.GetRawDeviceHandle();
	auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE(desc.heap.type));
	auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(desc.size, D3D12_RESOURCE_FLAGS(desc.resource.flags));

	HRESULT r = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAGS(desc.heap.flags),
		&resourceDesc,
		D3D12_RESOURCE_STATES(desc.resource.state),
		nullptr,
		IID_PPV_ARGS(&m_resource));

	if (!SUCCEEDED(r))
		return false;

	destroyGuard.Dismiss();
	return true;
}

void Resource::Destroy()
{
	if (m_resource)
	{
		m_resource->Release();
		m_resource = nullptr;
	}
}

void UpdateBufferResource(
	const Device& d,
	const CommandList& cmdList,
	Resource& destinationResource,
	Resource& intermediateResource,
	size_t numElements,
	size_t elementSize,
	const void* bufferData,
	ResourceFlags flags)
{
	if (!bufferData)
		return;

	size_t totalSize = numElements * elementSize;

	// Create committed destination resource
	{
		ResourceCreateDesc desc;
		desc.size = totalSize;
		desc.resource.flags = flags;

		if (!destinationResource.CreateCommittedResource(d, desc))
			return;
	}

	// Create intermediate resource
	{
		ResourceCreateDesc desc;
		desc.size = totalSize;
		desc.resource.flags = flags;
		desc.resource.state = ResourceState::GenericRead;
		desc.heap.type = HeapType::Upload;

		if (!intermediateResource.CreateCommittedResource(d, desc))
			return;
	}

	SubresourceData data;
	data.data = bufferData;
	data.rowPitch = totalSize;
	data.slicePitch = totalSize;

	UpdateSubresourceData updateSubresource;
	updateSubresource.intermediateOffset = 0;
	updateSubresource.firstSubresource = 0;
	updateSubresource.numSubresources = 1;
	updateSubresource.srcData = &data;

	cmdList.UpdateSubresource(destinationResource, intermediateResource, updateSubresource);
}
