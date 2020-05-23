// ************************************************************
// https://www.3dgep.com/learning-directx-12-1/
// ************************************************************

// Windows headers
#include <Windows.h> // kill me
#include <shellapi.h>

// I hate Windows.h
#if defined(CreateWindow)
#undef CreateWindow
#endif

#include <wrl.h>
using namespace Microsoft::WRL;

// DirectX headers
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>

// D3D12 extension library
#include <d3dx12.h>

// STL
#include <algorithm>
#include <chrono>

// Mine
#include "gfx/command_queue.h"
#include "gfx/device.h"
#include "gfx/swap_chain.h"
#include "gfx/window.h"
#include "types.h"
#include "using.h"

inline void ThrowIfFailed(HRESULT h)
{
	if (FAILED(h))
	{
		throw std::exception();
	}
}

// Constants
constexpr uint8 NumFrames = 3;

// Globals
static bool s_useWARPAdapter = false;
static uint32 s_winWidth = 1280;
static uint32 s_winHeight = 720;
static bool s_isInitialized = false;

// DX12/windows globals
namespace
{
	HWND s_wnd;
	RECT s_wndRect;

	struct Frame
	{
		ComPtr<ID3D12Resource> backBuffer;
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		uint64 fenceValue = 0;
	};

	ComPtr<ID3D12Device2> s_device;
	ComPtr<ID3D12CommandQueue> s_commandQueue;
	ComPtr<IDXGISwapChain4> s_swapChain;
	ComPtr<ID3D12GraphicsCommandList> s_commandList;
	Frame s_frames[NumFrames];
	ComPtr<ID3D12DescriptorHeap> s_rtvDescriptorHeap;

	ComPtr<ID3D12Fence> s_fence;
	uint64 s_fenceValue = 0;
	HANDLE s_fenceEvent;
}

bool s_useVsync = true;
bool s_tearingSupported = false;
bool s_fullScreen = false;

LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM)
{
	return NULL;
}

void EnableDebugLayer()
{
#if USING(DEBUGGING)
	// Always enable the debug layer before doing anything DX12 related
	// so all possible errors generated while creating DX12 objects
	// are caught by the debug layer.
	ComPtr<ID3D12Debug> debugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	debugInterface->EnableDebugLayer();
#endif
}

int wWinMain(HINSTANCE hInstanceExe, HINSTANCE, PTSTR pszCmdLine, int nCmdShow)
{
	EnableDebugLayer();
	Window w;
	w.Create(wndProc, hInstanceExe, s_winWidth, s_winHeight);

	Device d;
	d.Create();

	CommandQueue q;
	q.Create(d, CommandQueueType::Direct);

	SwapChain sc;
	sc.Create(w, q, s_winWidth, s_winHeight, 1);

	return 0;
}
