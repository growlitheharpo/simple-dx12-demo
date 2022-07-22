#pragma once

#include "gfx/core/enums/heap_flags.h"
#include "gfx/core/enums/heap_type.h"
#include "gfx/core/enums/resource_flags.h"
#include "gfx/core/enums/resource_state.h"
#include "gfx/core/util.h"

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

void UpdateBufferResource(
	const Device& d,
	const CommandList& cmdList,
	Resource& destinationResource,
	Resource& intermediateResource,
	size_t numElements,
	size_t elementSize,
	const void* bufferData,
	ResourceFlags flags = ResourceFlags::None);
