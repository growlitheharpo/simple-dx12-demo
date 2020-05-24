// ************************************************************
// https://www.3dgep.com/learning-directx-12-1/
// ************************************************************

// Mine
#include "gfx/command_list.h"
#include "gfx/command_queue.h"
#include "gfx/descriptor_heap.h"
#include "gfx/device.h"
#include "gfx/fence.h"
#include "gfx/frame_ctx.h"
#include "gfx/swap_chain.h"
#include "gfx/window.h"

#include "types.h"
#include "using.h"

// Unfortunately, ComPtr requires us to include the internals :(
#include <d3d12.h>
#include <dxgi1_6.h>

// Constants
constexpr uint8 NumFrames = 3;

// Globals
static uint32 s_winWidth = 1280;
static uint32 s_winHeight = 720;
static bool s_isInitialized = false;

// DX12/windows globals
namespace
{
	Window s_window;
	Device s_device;
	Fence s_fence;
	SwapChain s_swapChain;
	DescriptorHeap s_rtvHeap;
	FrameCtx s_frames[NumFrames];
	CommandQueue s_commandQueue;
	CommandList s_commandList;
}

bool s_useVsync = true;

void Update()
{
}

void Render()
{
	uint32 currentBackBufferIndex = s_swapChain.GetCurrentFrameIndex();
	auto& frame = s_frames[currentBackBufferIndex];

	frame.ResetCommandAllocator();
	s_commandList.Reset(frame);

	// Transition from Present back to RTV
	s_commandList.Transition(frame.GetBackBuffer(), ResourceState::Present, ResourceState::RenderTarget);

	// Clear to blue
	s_commandList.Clear(s_device, s_rtvHeap, s_swapChain, Vector4f(0.4f, 0.6f, 0.9f, 1.0f));

	// Present
	s_commandList.Transition(frame.GetBackBuffer(), ResourceState::RenderTarget, ResourceState::Present);

	// Execute
	s_commandList.Close();
	s_commandQueue.Execute({&s_commandList});

	// Present and signal
	{
		s_swapChain.Present(s_useVsync);
		frame.SetFenceValue(s_fence.Signal(s_commandQueue));
	}

	// Wait for new frame
	currentBackBufferIndex = s_swapChain.GetCurrentFrameIndex();
	s_fence.WaitForFence(s_frames[currentBackBufferIndex].GetFenceValue());
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
			::GetClientRect(wnd, &clientRect);

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
	s_window.Create(wndProc, hInstanceExe, s_winWidth, s_winHeight);

	s_device.Create(false);
	s_commandQueue.Create(s_device, CommandQueueType::Direct);
	s_swapChain.Create(s_window, s_commandQueue, s_winWidth, s_winHeight, NumFrames);

	s_rtvHeap.Create(s_device, DescriptorHeapType::RTV, NumFrames);

	for (size_t i = 0; i < _countof(s_frames); ++i)
		s_frames[i].Create(i, s_device, s_swapChain, s_rtvHeap, CommandQueueType::Direct);

	uint32 currentBackBufferIndex = s_swapChain.GetCurrentFrameIndex();
	s_commandList.Create(s_device, s_frames[currentBackBufferIndex], CommandQueueType::Direct);

	s_fence.Create(s_device);

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

	s_fence.Flush(s_commandQueue);
	s_fence.Destroy();

	return 0;
}
