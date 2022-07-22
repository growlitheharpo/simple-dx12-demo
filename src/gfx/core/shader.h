#pragma once

#include "gfx/core/fwd.h"
#include "gfx/core/util.h"

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

class Shader
{
private:
	ID3DBlob* m_blob = nullptr;

	ALLOW_GFX_ACCESS();
	ID3DBlob* GetRawBlobHandle() const
	{
		return m_blob;
	}

public:
	Shader() = default;
	~Shader();

	bool Create(const char* filePath);
	void Destroy();
};
