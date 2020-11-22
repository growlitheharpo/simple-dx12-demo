#pragma once

#include "gfx/enums/heap_flags.h"
#include "gfx/enums/heap_type.h"
#include "gfx/enums/resource_flags.h"
#include "gfx/enums/resource_state.h"
#include "gfx/util.h"

#include <wrl.h>

struct ID3D12Resource;

struct ResourceCreateDesc
{
	size_t size = 0;

	struct
	{
		ResourceFlags flags = ResourceFlags::None;
		ResourceState state = ResourceState::CopyDest;
	} resource;

	struct
	{
		HeapType type = HeapType::Default;
		HeapFlags flags = HeapFlags::AllowAllBuffersAndTextures;
	} heap;
};

class Resource
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<ID3D12Resource> m_resource;

	ALLOW_GFX_ACCESS();

	ComPtr<ID3D12Resource>& GetRawResourceHandle()
	{
		return m_resource;
	}

	const ComPtr<ID3D12Resource>& GetRawResourceHandle() const
	{
		return m_resource;
	}

public:
	bool CreateCommittedResource(const Device& d, const ResourceCreateDesc& desc);
};
