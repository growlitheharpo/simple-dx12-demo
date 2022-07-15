#pragma once

#include "gfx/core/fwd.h"

#include <heart/memory/intrusive_list.h>
#include <heart/sync/mutex.h>

#include "types.h"

class DestroyQueue
{
private:
	enum class DestroyQueueTag : uint8
	{
		// clang-format off
		Empty,
		ALL_GFX_CLASSES(, , )
		TagCount,
		// clang-format on
	};

	struct Node
	{
		HeartIntrusiveListLink link = {};

		void* object = nullptr;
		DestroyQueueTag tag = DestroyQueueTag::Empty;
	};

	typedef HeartIntrusiveList<Node, &Node::link> NodeList;

	NodeList m_freeList;
	NodeList m_frames[MaxBufferedFrames];
	HeartMutex m_listMutex;

	uint32 m_currentFrame = 0;
	uint32 m_frameCount = 0;

	Node m_destroyNodePool[1024];

	void ProcessSingleDestroy(Node& n);

public:
	DestroyQueue();
	~DestroyQueue();

	void Setup(uint32 frameCount = MaxBufferedFrames);

	void NextFrame();
	void Flush();

	void DestroyCommandList(CommandList& c);
	void DestroyCommandQueue(CommandQueue& c);
	void DestroyDescriptorHeap(DescriptorHeap& h);
	void DestroyFence(Fence& f);
	void DestroyResource(Resource& r);
	void DestroySwapChain(SwapChain& c);
};
