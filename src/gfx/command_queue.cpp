#include "command_queue.h"

#include "gfx/device.h"
#include "gfx/util.h"

#include <d3d12.h>

DECLARE_EQUIVALENT_ENUM(CommandQueueType, D3D12_COMMAND_LIST_TYPE);
TEST_EQUIVALENT_ENUM(CommandQueueType::Direct, D3D12_COMMAND_LIST_TYPE_DIRECT);
TEST_EQUIVALENT_ENUM(CommandQueueType::Bundle, D3D12_COMMAND_LIST_TYPE_BUNDLE);
TEST_EQUIVALENT_ENUM(CommandQueueType::Compute, D3D12_COMMAND_LIST_TYPE_COMPUTE);
TEST_EQUIVALENT_ENUM(CommandQueueType::Copy, D3D12_COMMAND_LIST_TYPE_COPY);
TEST_EQUIVALENT_ENUM(CommandQueueType::VideoDecode, D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE);
TEST_EQUIVALENT_ENUM(CommandQueueType::VideoProcess, D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS);
TEST_EQUIVALENT_ENUM(CommandQueueType::VideoEncode, D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE);

bool CommandQueue::Create(const Device& d, CommandQueueType type)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE(type);
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	auto device = d.GetRawDeviceHandle();
	HRESULT r = device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_commandQueue));
	if (!SUCCEEDED(r))
		return false;

	return true;
}
