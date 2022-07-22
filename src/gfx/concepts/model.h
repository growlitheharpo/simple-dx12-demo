#pragma once

#include "gfx/core/fwd.h"

#include "gfx/core/index_buffer_view.h"
#include "gfx/core/resource.h"
#include "gfx/core/vertex_buffer_view.h"

struct D3D12_VERTEX_BUFFER_VIEW;
struct D3D12_INDEX_BUFFER_VIEW;

class Model
{
private:
	Resource m_vertexBuffer;
	Resource m_indexBuffer;

	VertexBufferView m_vertexView;
	IndexBufferView m_indexView;

public:
	Model() = default;
	~Model();

	void Create(const Device& d, DestroyQueue& destroyQueue, CommandList& cmdList);
	void Destroy();

	void Draw(CommandList& cmdList);
};
