#pragma once

#include "gfx/core/fwd.h"
#include "gfx/core/util.h"

#include "types.h"

struct D3D12_VERTEX_BUFFER_VIEW;

class VertexBufferView
{
private:
	uint8 m_buffer[16];

	ALLOW_GFX_ACCESS()
	const D3D12_VERTEX_BUFFER_VIEW* GetRawViewHandle() const
	{
		return reinterpret_cast<const D3D12_VERTEX_BUFFER_VIEW*>(m_buffer);
	}

	D3D12_VERTEX_BUFFER_VIEW* GetRawViewHandle()
	{
		return reinterpret_cast<D3D12_VERTEX_BUFFER_VIEW*>(m_buffer);
	}

public:
	VertexBufferView() = default;
	~VertexBufferView();

	bool Create(Resource& vertexBuffer, uint32 sizeInBytes, uint32 strideInBytes);
	void Destroy();
};
