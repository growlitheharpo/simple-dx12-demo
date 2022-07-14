#include "frame_ctx.h"

#include "gfx/descriptor_heap.h"
#include "gfx/device.h"
#include "gfx/swap_chain.h"

#include <heart/scope_exit.h>

#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

FrameCtx::~FrameCtx()
{
	Destroy();
}

bool FrameCtx::Create(size_t index, const Device& d, const SwapChain& sc, const DescriptorHeap& h, CommandQueueType allocatorType)
{
	HeartScopeGuard destroyGuard([&] {
		Destroy();
	});

	auto device = d.GetRawDeviceHandle();
	auto swapChain = sc.GetRawSwapChainHandle();
	auto descriptorHeap = h.GetRawDescriptorHeapHandle();

	auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());
	rtvHandle.Offset(int32(index * rtvDescriptorSize));

	auto& rawBackbuffer = m_backBuffer.GetRawResourceRef();
	HRESULT r = swapChain->GetBuffer(uint32(index), IID_PPV_ARGS(&rawBackbuffer));
	if (!SUCCEEDED(r))
		return false;

	device->CreateRenderTargetView(rawBackbuffer, nullptr, rtvHandle);

	r = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE(allocatorType), IID_PPV_ARGS(&m_commandAllocator));
	if (!SUCCEEDED(r))
		return false;

	destroyGuard.Dismiss();
	return true;
}

void FrameCtx::Destroy()
{
	if (m_commandAllocator)
	{
		m_commandAllocator->Release();
		m_commandAllocator = nullptr;
	}

	m_backBuffer.Destroy();
}

void FrameCtx::ResetCommandAllocator()
{
	m_commandAllocator->Reset();
}
