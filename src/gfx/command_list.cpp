#include "command_list.h"

#include "gfx/device.h"
#include "gfx/frame_ctx.h"

#include <d3d12.h>

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
