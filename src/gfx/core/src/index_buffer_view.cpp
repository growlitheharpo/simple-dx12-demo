#include "gfx/core/index_buffer_view.h"

#include "gfx/core/resource.h"

#include <d3d12.h>
#include <d3dx12.h>

IndexBufferView::~IndexBufferView()
{
	Destroy();
}

bool IndexBufferView::Create(Resource& vertexBuffer, ResourceFormat format, uint32 sizeInBytes)
{
	static_assert(sizeof(m_buffer) == sizeof(D3D12_INDEX_BUFFER_VIEW));

	D3D12_INDEX_BUFFER_VIEW& view = *GetRawViewHandle();
	view.BufferLocation = vertexBuffer.GetRawResourceHandle()->GetGPUVirtualAddress();
	view.Format = DXGI_FORMAT(format);
	view.SizeInBytes = sizeInBytes;

	return true;
}

void IndexBufferView::Destroy()
{
}
