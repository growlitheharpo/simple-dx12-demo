#include "descriptor_heap.h"

#include "gfx/device.h"
#include "gfx/util.h"

#include <heart/scope_exit.h>

#include <d3d12.h>

DECLARE_EQUIVALENT_ENUM(DescriptorHeapType, D3D12_DESCRIPTOR_HEAP_TYPE);
TEST_EQUIVALENT_ENUM(DescriptorHeapType::Cbv_Srv_Uav, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
TEST_EQUIVALENT_ENUM(DescriptorHeapType::Sampler, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
TEST_EQUIVALENT_ENUM(DescriptorHeapType::RTV, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
TEST_EQUIVALENT_ENUM(DescriptorHeapType::DSV, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
TEST_EQUIVALENT_ENUM(DescriptorHeapType::Count, D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES);

DescriptorHeap::~DescriptorHeap()
{
	Destroy();
}

bool DescriptorHeap::Create(const Device& d, DescriptorHeapType type, uint32 numDescriptors)
{
	HeartScopeGuard destroyGuard([&] {
		Destroy();
	});

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = numDescriptors;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE(type);

	auto device = d.GetRawDeviceHandle();
	auto r = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap));
	if (!SUCCEEDED(r))
		return false;

	destroyGuard.Dismiss();
	return true;
}

void DescriptorHeap::Destroy()
{
	if (m_heap)
	{
		m_heap->Release();
		m_heap = nullptr;
	}
}
