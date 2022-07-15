#include "gfx/core/fence.h"

#include "gfx/core/command_queue.h"
#include "gfx/core/device.h"

#include <heart/scope_exit.h>

#include <d3d12.h>

#include <Windows.h>

Fence::~Fence()
{
	Destroy();
}

bool Fence::Create(const Device& d)
{
	HeartScopeGuard destroyGuard([&] {
		Destroy();
	});

	auto device = d.GetRawDeviceHandle();
	HRESULT r = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (!SUCCEEDED(r))
		return false;

	m_fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_fenceEvent)
		return false;

	destroyGuard.Dismiss();
	return true;
}

void Fence::Destroy()
{
	if (m_fenceEvent)
	{
		::CloseHandle(m_fenceEvent);
		m_fenceEvent = nullptr;
	}

	m_fenceValue = 0;

	if (m_fence)
	{
		m_fence->Release();
		m_fence = nullptr;
	}
}

uint64 Fence::Signal(const CommandQueue& q)
{
	++m_fenceValue;

	auto cmdQueue = q.GetRawCommandQueueHandle();
	cmdQueue->Signal(m_fence, m_fenceValue);

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
