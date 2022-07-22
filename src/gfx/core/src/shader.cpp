#include "gfx/core/shader.h"

#include <heart/scope_exit.h>

#include <stdio.h>
#include <string.h>

#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3dx12.h>

Shader::~Shader()
{
	Destroy();
}

bool Shader::Create(const char* filePath)
{
	HeartScopeGuard failureGuard([&] {
		Destroy();
	});

	wchar_t wideFilePath[2048];
	int written = swprintf_s(wideFilePath, L"%S", filePath);
	if (written < 0)
		return false;

	HRESULT r = D3DReadFileToBlob(wideFilePath, &m_blob);
	if (!SUCCEEDED(r))
		return false;

	failureGuard.Dismiss();
	return true;
}

void Shader::Destroy()
{
	if (m_blob)
	{
		m_blob->Release();
		m_blob = nullptr;
	}
}
