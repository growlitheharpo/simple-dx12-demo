#pragma once

#include "gfx/core/fwd.h"
#include "gfx/core/util.h"

#include "types.h"

struct ID3D12RootSignature;

class RootSignature
{
private:
	ID3D12RootSignature* m_rootSignature = nullptr;

	ALLOW_GFX_ACCESS();
	ID3D12RootSignature* GetRawRootSignatureHandle() const
	{
		return m_rootSignature;
	}

public:
	RootSignature() = default;
	~RootSignature();

	bool Create(Device& device);
	void Destroy();
};
