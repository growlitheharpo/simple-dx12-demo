#include "gfx/core/destroy_queue.h"

#include "gfx/core/command_list.h"
#include "gfx/core/command_queue.h"
#include "gfx/core/descriptor_heap.h"
#include "gfx/core/fence.h"
#include "gfx/core/resource.h"
#include "gfx/core/swap_chain.h"

#include <limits>

#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

void DestroyQueue::ProcessSingleDestroy(Node& n)
{
	switch ((DestroyQueueTag)n.tag)
	{
	case DestroyQueueTag::CommandList:
		reinterpret_cast<ID3D12GraphicsCommandList*>(n.object)->Release();
		break;
	case DestroyQueueTag::CommandQueue:
		reinterpret_cast<ID3D12CommandQueue*>(n.object)->Release();
		break;
	case DestroyQueueTag::DescriptorHeap:
		reinterpret_cast<ID3D12DescriptorHeap*>(n.object)->Release();
		break;
	case DestroyQueueTag::Fence:
		reinterpret_cast<ID3D12Fence*>(n.object)->Release();
		break;
	case DestroyQueueTag::Resource:
		reinterpret_cast<ID3D12Resource*>(n.object)->Release();
		break;
	case DestroyQueueTag::SwapChain:
		reinterpret_cast<IDXGISwapChain4*>(n.object)->Release();
		break;
	}

	n.object = nullptr;
	n.tag = DestroyQueueTag::Empty;
}

DestroyQueue::DestroyQueue()
{
	for (auto& node : m_destroyNodePool)
	{
		m_freeList.PushBack(&node);
	}
}

DestroyQueue::~DestroyQueue()
{
	Flush();
}

void DestroyQueue::Setup(uint32 frameCount)
{
	m_frameCount = frameCount;
}

void DestroyQueue::NextFrame()
{
	HeartLockGuard lock(m_listMutex);

	m_currentFrame = (m_currentFrame + 1) % m_frameCount;
	while (!m_frames[m_currentFrame].IsEmpty())
	{
		Node* n = m_frames[m_currentFrame].PopFront();
		ProcessSingleDestroy(*n);
		m_freeList.PushBack(n);
	}
}

void DestroyQueue::Flush()
{
	uint32 startFrame = m_currentFrame;
	do
	{
		NextFrame();
	} while (m_currentFrame != startFrame);
}

void DestroyQueue::DestroyCommandList(CommandList& c)
{
	HeartLockGuard lock(m_listMutex);

	Node* n = m_freeList.PopFront();
	n->tag = DestroyQueueTag::CommandList;
	n->object = c.m_commandList;

	m_frames[m_currentFrame].PushBack(n);

	c.m_commandList = nullptr;
}

void DestroyQueue::DestroyCommandQueue(CommandQueue& c)
{
	HeartLockGuard lock(m_listMutex);

	Node* n = m_freeList.PopFront();
	n->tag = DestroyQueueTag::CommandQueue;
	n->object = c.m_commandQueue;

	m_frames[m_currentFrame].PushBack(n);

	c.m_commandQueue = nullptr;
}

void DestroyQueue::DestroyDescriptorHeap(DescriptorHeap& h)
{
	HeartLockGuard lock(m_listMutex);

	Node* n = m_freeList.PopFront();
	n->tag = DestroyQueueTag::DescriptorHeap;
	n->object = h.m_heap;

	m_frames[m_currentFrame].PushBack(n);

	h.m_heap = nullptr;
}

void DestroyQueue::DestroyFence(Fence& f)
{
	HeartLockGuard lock(m_listMutex);

	Node* n = m_freeList.PopFront();
	n->tag = DestroyQueueTag::Fence;
	n->object = f.m_fence;

	m_frames[m_currentFrame].PushBack(n);

	f.m_fence = nullptr;
}

void DestroyQueue::DestroyResource(Resource& r)
{
	HeartLockGuard lock(m_listMutex);

	Node* n = m_freeList.PopFront();
	n->tag = DestroyQueueTag::Resource;
	n->object = r.m_resource;

	m_frames[m_currentFrame].PushBack(n);

	r.m_resource = nullptr;
}

void DestroyQueue::DestroySwapChain(SwapChain& c)
{
	HeartLockGuard lock(m_listMutex);

	Node* n = m_freeList.PopFront();
	n->tag = DestroyQueueTag::SwapChain;
	n->object = c.m_swapChain;

	m_frames[m_currentFrame].PushBack(n);

	c.m_swapChain = nullptr;
}
