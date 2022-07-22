#pragma once

#include "gfx/core/fwd.h"
#include "gfx/core/util.h"

#include "gfx/core/enums/format.h"

#include "types.h"

struct D3D12_INDEX_BUFFER_VIEW;

class IndexBufferView
{
private:
	uint8 m_buffer[16];

	ALLOW_GFX_ACCESS()
	const D3D12_INDEX_BUFFER_VIEW* GetRawViewHandle() const
	{
		return reinterpret_cast<const D3D12_INDEX_BUFFER_VIEW*>(m_buffer);
	}

	D3D12_INDEX_BUFFER_VIEW* GetRawViewHandle()
	{
		return reinterpret_cast<D3D12_INDEX_BUFFER_VIEW*>(m_buffer);
	}

public:
	IndexBufferView() = default;
	~IndexBufferView();

	bool Create(Resource& vertexBuffer, ResourceFormat format, uint32 sizeInBytes);
	void Destroy();
};
