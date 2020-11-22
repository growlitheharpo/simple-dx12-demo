#include "command_list.h"

#include "gfx/descriptor_heap.h"
#include "gfx/device.h"
#include "gfx/frame_ctx.h"
#include "gfx/swap_chain.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

bool CommandList::Create(const Device& d, const FrameCtx& fc, CommandQueueType type)
{
	auto device = d.GetRawDeviceHandle();
	auto commandAllocator = fc.GetRawCommandAllocatorHandle();

	HRESULT r = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE(type), commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList));
	if (!SUCCEEDED(r))
		return false;

	r = m_commandList->Close();
	if (!SUCCEEDED(r))
		return false;

	return true;
}

void CommandList::Reset(const FrameCtx& fc)
{
	m_commandList->Reset(fc.GetRawCommandAllocatorHandle().Get(), nullptr);
}

void CommandList::Transition(const Resource& r, ResourceState from, ResourceState to)
{
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		r.GetRawResourceHandle().Get(),
		D3D12_RESOURCE_STATES(from),
		D3D12_RESOURCE_STATES(to));
	m_commandList->ResourceBarrier(1, &barrier);
}

void CommandList::Clear(const Device& d, const DescriptorHeap& heap, const SwapChain& c, Vector4f color)
{
	auto rtvHeap = heap.GetRawDescriptorHeapHandle();
	auto device = d.GetRawDeviceHandle();
	auto rtvSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(rtvHeap->GetCPUDescriptorHandleForHeapStart(), c.GetCurrentFrameIndex(), rtvSize);
	m_commandList->ClearRenderTargetView(rtv, color.data_, 0, nullptr);
}

void CommandList::UpdateSubresource(const Resource& destination, const Resource& intermediate, UpdateSubresourceData data) const
{
	auto dest = destination.GetRawResourceHandle().Get();
	auto interm = intermediate.GetRawResourceHandle().Get();
	auto cmdList = this->GetRawCommandListHandle().Get();

	// Static assert that the reinterpret_cast will "succeed"
	// TODO: There *must* be a better way to do this without exposing everything
	static_assert(sizeof(SubresourceData) == sizeof(D3D12_SUBRESOURCE_DATA));
	static_assert(offsetof(SubresourceData, data) == offsetof(D3D12_SUBRESOURCE_DATA, pData));
	static_assert(offsetof(SubresourceData, rowPitch) == offsetof(D3D12_SUBRESOURCE_DATA, RowPitch));
	static_assert(offsetof(SubresourceData, slicePitch) == offsetof(D3D12_SUBRESOURCE_DATA, SlicePitch));

	::UpdateSubresources(
		cmdList,
		dest,
		interm,
		data.intermediateOffset,
		data.firstSubresource,
		data.numSubresources,
		reinterpret_cast<D3D12_SUBRESOURCE_DATA*>(data.srcData));
}

void CommandList::Close()
{
	m_commandList->Close();
}
