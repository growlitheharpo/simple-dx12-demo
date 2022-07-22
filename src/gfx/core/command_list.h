#pragma once

#include "gfx/core/fwd.h"

#include "gfx/core/enums/command_type.h"
#include "gfx/core/enums/resource_state.h"
#include "gfx/core/util.h"

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

	void SetPipelineState(PipelineStateObject& pso);

	void SetRootSignature(RootSignature& rootSignature);

	void SetPrimitiveTopology(uint32 todo);

	void SetVertexBuffer(VertexBufferView& view);

	void SetIndexBuffer(IndexBufferView& view);

	void DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 instanceCount, uint32 startIndexLocation = 0, int32 baseVertexLocation = 0, uint32 startInstanceLocation = 0);

	void Close();
};
