#pragma once

#include "gfx/fwd.h"

#include "gfx/enums/command_type.h"
#include "gfx/enums/resource_state.h"
#include "gfx/util.h"

#include "vector.h"

struct ID3D12GraphicsCommandList;

class CommandList
{
private:
	ID3D12GraphicsCommandList* m_commandList = nullptr;

	ALLOW_GFX_ACCESS();
	ID3D12GraphicsCommandList* GetRawCommandListHandle() const
	{
		return m_commandList;
	}

public:
	CommandList() = default;
	~CommandList();

	bool Create(const Device& d, const FrameCtx& fc, CommandQueueType type);
	void Destroy();

	void Reset(const FrameCtx& fc);

	void Transition(Resource& r, ResourceState from, ResourceState to);
	void Clear(const Device& d, const DescriptorHeap& heap, const SwapChain& c, Vector4f color);

	void UpdateSubresource(Resource& destination, Resource& intermediate, const UpdateSubresourceData& data) const;

	void Close();
};
