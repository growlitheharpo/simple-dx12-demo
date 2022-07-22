#include "gfx/core/root_signature.h"

#include "gfx/core/device.h"

#include <heart/countof.h>
#include <heart/scope_exit.h>

#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>

RootSignature::~RootSignature()
{
	Destroy();
}

bool RootSignature::Create(Device& device)
{
	HeartScopeGuard failureGuard([&] {
		Destroy();
	});

	ID3DBlob* rootSignatureBlob = nullptr;
	HEART_SCOPE_EXIT([&] { if (rootSignatureBlob) rootSignatureBlob->Release(); });

	ID3DBlob* errorBlob = nullptr;
	HEART_SCOPE_EXIT([&] { if (errorBlob) errorBlob->Release(); });

	auto* deviceHandle = device.GetRawDeviceHandle();

	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

	HRESULT r = deviceHandle->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData));
	if (!SUCCEEDED(r))
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// TODO: Move root signature parameters to function parameter
	CD3DX12_ROOT_PARAMETER1 rootParameters[1] = {};
	rootParameters[0].InitAsConstants(sizeof(DirectX::XMMATRIX) / sizeof(int32), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	// TODO: samplers

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_1((uint32)HeartCountOf(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);

	r = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &rootSignatureBlob, &errorBlob);
	if (!SUCCEEDED(r))
		return false;

	r = deviceHandle->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	if (!SUCCEEDED(r))
		return false;

	failureGuard.Dismiss();
	return true;
}

void RootSignature::Destroy()
{
	if (m_rootSignature)
	{
		m_rootSignature->Release();
		m_rootSignature = nullptr;
	}
}
