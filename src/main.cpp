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
#include "gfx/command_list.h"
#include "gfx/command_queue.h"
#include "gfx/descriptor_heap.h"
#include "gfx/device.h"
#include "gfx/frame_ctx.h"
#include "gfx/swap_chain.h"
#include "gfx/window.h"

#include "types.h"
#include "using.h"

using milliseconds = std::chrono::milliseconds;

// Constants
constexpr uint8 NumFrames = 3;

// Globals
static bool s_useWARPAdapter = false;
static uint32 s_winWidth = 1280;
static uint32 s_winHeight = 720;
static bool s_isInitialized = false;

static uint32 s_currentBackBufferIndex = 0;

// DX12/windows globals
namespace
{
	RECT s_wndRect;

	ComPtr<ID3D12Fence> s_fence;
	uint64 s_fenceValue = 0;
	HANDLE s_fenceEvent;

	Window s_window;
	Device s_device;
	SwapChain s_swapChain;
	DescriptorHeap s_rtvHeap;
	FrameCtx s_frames[NumFrames];
	CommandQueue s_commandQueue;
	CommandList s_commandList;
}

bool s_useVsync = true;
bool s_tearingSupported = false;
bool s_fullScreen = false;

void EnableDebugLayer()
{
#if USING(DX12_DEBUG_LAYER)
	// Always enable the debug layer before doing anything DX12 related
	// so all possible errors generated while creating DX12 objects
	// are caught by the debug layer.
	ComPtr<ID3D12Debug> debugInterface;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
	{
		debugInterface->EnableDebugLayer();
	}
#endif
}

bool CreateFence(ComPtr<ID3D12Device> device, ComPtr<ID3D12Fence>* outFence, HANDLE* outEvent)
{
	if (!outFence || !outEvent)
		return false;

	ComPtr<ID3D12Fence> fence;
	HRESULT r = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (!SUCCEEDED(r))
		return false;

	HANDLE fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!fenceEvent)
		return false;

	*outFence = fence;
	*outEvent = fenceEvent;
	return true;
}

void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent, milliseconds duration = milliseconds::max())
{
	if (fence->GetCompletedValue() < fenceValue)
	{
		HRESULT r = fence->SetEventOnCompletion(fenceValue, fenceEvent);
		if (!SUCCEEDED(r))
			throw std::exception();

		DWORD r2 = ::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
		if (r2 == WAIT_TIMEOUT)
			throw std::exception("You messed up your fences");
	}
}

void Update()
{
}

void Render()
{
	auto device = s_device.GetRawDeviceHandle();
	auto& frame = s_frames[s_currentBackBufferIndex];
	auto allocator = frame.GetRawCommandAllocatorHandle();
	auto backBuffer = frame.GetRawBackBufferHandle();
	auto commandList = s_commandList.GetRawCommandListHandle();
	auto rtvHeap = s_rtvHeap.GetRawDescriptorHeapHandle();
	auto commandQueue = s_commandQueue.GetRawCommandQueueHandle();
	auto swapChain = s_swapChain.GetRawSwapChainHandle();

	auto rtvSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	allocator->Reset();
	commandList->Reset(allocator.Get(), nullptr);

	{
		// Transition from Present back to RTV
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		commandList->ResourceBarrier(1, &barrier);

		// Clear to blue
		float clearColor[] = {0.4f, 0.6f, 0.9f, 1.0f};
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(rtvHeap->GetCPUDescriptorHandleForHeapStart(), s_currentBackBufferIndex, rtvSize);
		commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);

		// Present
		barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandList->ResourceBarrier(1, &barrier);

		// Execute
		commandList->Close();
		ID3D12CommandList* const lists[] = {commandList.Get()};
		commandQueue->ExecuteCommandLists(1, lists);
	}

	// Present and signal
	{
		uint32 syncInterval = s_useVsync ? 1 : 0;
		uint32 presentFlags = s_swapChain.HasTearingSupport() && !s_useVsync ? DXGI_PRESENT_ALLOW_TEARING : 0;
		swapChain->Present(syncInterval, presentFlags);

		uint64_t fenceValueForSignal = ++s_fenceValue;
		commandQueue->Signal(s_fence.Get(), fenceValueForSignal);
		frame.SetFenceValue(fenceValueForSignal);
	}

	// Wait for new frame
	s_currentBackBufferIndex = swapChain->GetCurrentBackBufferIndex();
	WaitForFenceValue(s_fence, s_frames[s_currentBackBufferIndex].GetFenceValue(), s_fenceEvent, milliseconds(500));
}

LRESULT CALLBACK wndProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (s_isInitialized)
	{
		switch (message)
		{
		case WM_PAINT:
			Update();
			Render();
			break;
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
			switch (wParam)
			{
			case VK_ESCAPE:
				::PostQuitMessage(0);
				break;
			}
		}
		break;
		// The default window procedure will play a system notification sound
		// when pressing the Alt+Enter keyboard combination if this message is
		// not handled.
		case WM_SYSCHAR:
			break;
		case WM_SIZE: {
			RECT clientRect = {};
			::GetClientRect(s_window.GetRawWindowHandle(), &clientRect);

			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;

			//Resize(width, height);
		}
		break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		default:
			return ::DefWindowProcW(wnd, message, wParam, lParam);
		}
	}
	else
	{
		return ::DefWindowProcW(wnd, message, wParam, lParam);
	}

	return 0;
}

int wWinMain(HINSTANCE hInstanceExe, HINSTANCE, PTSTR pszCmdLine, int nCmdShow)
{
	EnableDebugLayer();
	s_window.Create(wndProc, hInstanceExe, s_winWidth, s_winHeight);

	s_device.Create();
	s_commandQueue.Create(s_device, CommandQueueType::Direct);
	s_swapChain.Create(s_window, s_commandQueue, s_winWidth, s_winHeight, NumFrames);

	s_currentBackBufferIndex = s_swapChain.GetRawSwapChainHandle()->GetCurrentBackBufferIndex();

	s_rtvHeap.Create(s_device, DescriptorHeapType::RTV, NumFrames);

	for (size_t i = 0; i < _countof(s_frames); ++i)
		s_frames[i].Create(i, s_device, s_swapChain, s_rtvHeap, CommandQueueType::Direct);

	s_commandList.Create(s_device, s_frames[s_currentBackBufferIndex], CommandQueueType::Direct);

	CreateFence(s_device.GetRawDeviceHandle(), &s_fence, &s_fenceEvent);
	s_isInitialized = true;

	s_window.Show();

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	::CloseHandle(s_fenceEvent);

	return 0;
}
