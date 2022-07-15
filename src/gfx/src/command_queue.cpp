#include "gfx/command_queue.h"

#include "gfx/command_list.h"
#include "gfx/device.h"
#include "gfx/util.h"

#include <heart/scope_exit.h>

#include <d3d12.h>

#include <algorithm>
#include <iterator>
#include <vector>

DECLARE_EQUIVALENT_ENUM(CommandQueueType, D3D12_COMMAND_LIST_TYPE);
TEST_EQUIVALENT_ENUM(CommandQueueType::Direct, D3D12_COMMAND_LIST_TYPE_DIRECT);
TEST_EQUIVALENT_ENUM(CommandQueueType::Bundle, D3D12_COMMAND_LIST_TYPE_BUNDLE);
TEST_EQUIVALENT_ENUM(CommandQueueType::Compute, D3D12_COMMAND_LIST_TYPE_COMPUTE);
TEST_EQUIVALENT_ENUM(CommandQueueType::Copy, D3D12_COMMAND_LIST_TYPE_COPY);
TEST_EQUIVALENT_ENUM(CommandQueueType::VideoDecode, D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE);
TEST_EQUIVALENT_ENUM(CommandQueueType::VideoProcess, D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS);
TEST_EQUIVALENT_ENUM(CommandQueueType::VideoEncode, D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE);

CommandQueue::~CommandQueue()
{
	Destroy();
}

bool CommandQueue::Create(const Device& d, CommandQueueType type)
{
	HeartScopeGuard destroyGuard([&] {
		Destroy();
	});

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE(type);
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	auto device = d.GetRawDeviceHandle();
	HRESULT r = device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_commandQueue));
	if (!SUCCEEDED(r))
		return false;

	destroyGuard.Dismiss();
	return true;
}

void CommandQueue::Destroy()
{
	if (m_commandQueue)
	{
		m_commandQueue->Release();
		m_commandQueue = nullptr;
	}
}

void CommandQueue::Execute(std::initializer_list<CommandList*> wrappedLists)
{
	std::vector<ID3D12CommandList*> lists;
	std::transform(wrappedLists.begin(), wrappedLists.end(), std::back_inserter(lists), [](CommandList* l) {
		return l->GetRawCommandListHandle();
	});

	m_commandQueue->ExecuteCommandLists(uint32(lists.size()), lists.data());
}
