#include "gfx/core/vertex_buffer_view.h"

#include "gfx/core/resource.h"

#include <d3d12.h>
#include <d3dx12.h>

VertexBufferView::~VertexBufferView()
{
	Destroy();
}

bool VertexBufferView::Create(Resource& vertexBuffer, uint32 sizeInBytes, uint32 strideInBytes)
{
	static_assert(sizeof(m_buffer) == sizeof(D3D12_VERTEX_BUFFER_VIEW));

	D3D12_VERTEX_BUFFER_VIEW& view = *GetRawViewHandle();
	view.BufferLocation = vertexBuffer.GetRawResourceHandle()->GetGPUVirtualAddress();
	view.SizeInBytes = sizeInBytes;
	view.StrideInBytes = strideInBytes;

	return true;
}

void VertexBufferView::Destroy()
{
}
