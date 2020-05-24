#pragma once

#include "gfx/util.h"

#include "types.h"

#include <wrl.h>

struct IDXGIAdapter4;
struct ID3D12Device2;

class Device
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<IDXGIAdapter4> m_adapterHandle;
	ComPtr<ID3D12Device2> m_deviceHandle;

	static ComPtr<IDXGIAdapter4> GetAdapter(bool useWarp);
	static ComPtr<ID3D12Device2> GetDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter);

	ALLOW_GFX_ACCESS();
	ComPtr<ID3D12Device2> GetRawDeviceHandle() const
	{
		return m_deviceHandle;
	}

public:
	bool Create(bool useWarp);
	void Destroy();
};
