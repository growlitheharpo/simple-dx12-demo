#pragma once

#include "gfx/enums/heap_flags.h"
#include "gfx/enums/heap_type.h"
#include "gfx/enums/resource_flags.h"
#include "gfx/enums/resource_state.h"
#include "gfx/util.h"

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
private:
	ID3D12Resource* m_resource = nullptr;

	ALLOW_GFX_ACCESS();

	ID3D12Resource* GetRawResourceHandle()
	{
		return m_resource;
	}

	const ID3D12Resource* GetRawResourceHandle() const
	{
		return m_resource;
	}

	ID3D12Resource*& GetRawResourceRef()
	{
		return m_resource;
	}

public:
	Resource() = default;
	~Resource();

	bool CreateCommittedResource(const Device& d, const ResourceCreateDesc& desc);

	void Destroy();
};
