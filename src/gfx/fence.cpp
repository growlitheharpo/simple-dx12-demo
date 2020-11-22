#include "fence.h"

#include "gfx/command_queue.h"
#include "gfx/device.h"

#include <d3d12.h>

#include <Windows.h>

bool Fence::Create(const Device& d)
{
	auto device = d.GetRawDeviceHandle();
	HRESULT r = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (!SUCCEEDED(r))
		return false;

	m_fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_fenceEvent)
		return false;

	return true;
}

void Fence::Destroy()
{
	::CloseHandle(m_fenceEvent);
	m_fence = nullptr;
	m_fenceValue = 0;
}

uint64 Fence::Signal(const CommandQueue& q)
{
	++m_fenceValue;

	auto cmdQueue = q.GetRawCommandQueueHandle();
	cmdQueue->Signal(m_fence.Get(), m_fenceValue);

	return m_fenceValue;
}

void Fence::WaitForFence(uint64 value, milliseconds waitDuration)
{
	if (m_fence->GetCompletedValue() < value)
	{
		HRESULT r = m_fence->SetEventOnCompletion(value, m_fenceEvent);
		if (!SUCCEEDED(r))
			abort();

		::WaitForSingleObject(m_fenceEvent, DWORD(waitDuration.count()));
	}
}
