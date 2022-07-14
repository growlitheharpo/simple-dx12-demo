#pragma once

#include "gfx/util.h"

#include "types.h"

struct IDXGIAdapter4;
struct ID3D12Device2;

class Device
{
private:
	IDXGIAdapter4* m_adapterHandle = nullptr;
	ID3D12Device2* m_deviceHandle = nullptr;

	static IDXGIAdapter4* GetAdapter(bool useWarp);
	static ID3D12Device2* GetDevice(IDXGIAdapter4* adapter);

	ALLOW_GFX_ACCESS();
	ID3D12Device2* GetRawDeviceHandle() const
	{
		return m_deviceHandle;
	}

public:
	Device() = default;
	~Device();

	bool Create(bool useWarp);
	void Destroy();
};
