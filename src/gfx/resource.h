#pragma once

#include "gfx/resource_state.h"
#include "gfx/util.h"

#include <wrl.h>

struct ID3D12Resource;

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
};
