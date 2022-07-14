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
#include "vector.h"

#include "Windows.h"

// Constants
constexpr uint8 NumFrames = 3;

// Globals
static uint32 s_winWidth = 1280;
static uint32 s_winHeight = 720;
static bool s_isInitialized = false;

// DX12/windows globals
static Window s_window;
static Device s_device;
static Fence s_fence;
static SwapChain s_swapChain;
static DescriptorHeap s_rtvHeap;
static FrameCtx s_frames[NumFrames];
static CommandQueue s_commandQueue;
static CommandList s_commandList;

#pragma pack(push, 1)
struct VertexPosColor
{
	Vector3fp position;
	Vector3fp color;
};
#pragma pack(pop)

static VertexPosColor s_verts[] = {
	{Vector3fp(Vector3f(-1.0f, -1.0f, -1.0f)), Vector3fp(Vector3f(0.0f, 0.0f, 0.0f))},
	{Vector3fp(Vector3f(-1.0f, 1.0f, -1.0f)), Vector3fp(Vector3f(0.0f, 1.0f, 0.0f))},
	{Vector3fp(Vector3f(1.0f, 1.0f, -1.0f)), Vector3fp(Vector3f(1.0f, 1.0f, 0.0f))},
	{Vector3fp(Vector3f(1.0f, -1.0f, -1.0f)), Vector3fp(Vector3f(1.0f, 0.0f, 0.0f))},
	{Vector3fp(Vector3f(-1.0f, -1.0f, 1.0f)), Vector3fp(Vector3f(0.0f, 0.0f, 1.0f))},
	{Vector3fp(Vector3f(-1.0f, 1.0f, 1.0f)), Vector3fp(Vector3f(0.0f, 1.0f, 1.0f))},
	{Vector3fp(Vector3f(1.0f, 1.0f, 1.0f)), Vector3fp(Vector3f(1.0f, 1.0f, 1.0f))},
	{Vector3fp(Vector3f(1.0f, -1.0f, 1.0f)), Vector3fp(Vector3f(1.0f, 0.0f, 1.0f))},
};

// clang-format off
static uint16 s_indices[] = {
	0, 1, 2, 0, 2, 3,
	4, 6, 5, 4, 7, 6,
	4, 5, 1, 4, 1, 0,
	3, 2, 6, 3, 6, 7,
	1, 5, 6, 1, 6, 2,
	4, 0, 3, 4, 3, 7,
};
// clang-format on

void UpdateBufferResource(
	const Device& d,
	const CommandList& cmdList,
	Resource& destinationResource,
	Resource& intermediateResource,
	size_t numElements,
	size_t elementSize,
	const void* bufferData,
	ResourceFlags flags = ResourceFlags::None)
{
	if (!bufferData)
		return;

	size_t totalSize = numElements * elementSize;

	// Create committed destination resource
	{
		ResourceCreateDesc desc;
		desc.size = totalSize;
		desc.resource.flags = flags;

		if (!destinationResource.CreateCommittedResource(d, desc))
			return;
	}

	// Create intermediate resource
	{
		ResourceCreateDesc desc;
		desc.size = totalSize;
		desc.resource.flags = flags;
		desc.resource.state = ResourceState::GenericRead;
		desc.heap.type = HeapType::Upload;

		if (!intermediateResource.CreateCommittedResource(d, desc))
			return;
	}

	SubresourceData data;
	data.data = bufferData;
	data.rowPitch = totalSize;
	data.slicePitch = totalSize;

	UpdateSubresourceData updateSubresource;
	updateSubresource.intermediateOffset = 0;
	updateSubresource.firstSubresource = 0;
	updateSubresource.numSubresources = 1;
	updateSubresource.srcData = &data;

	cmdList.UpdateSubresource(destinationResource, intermediateResource, updateSubresource);
}

Resource s_vertexBuffer;

void LoadContent(
	const Device& d,
	const CommandList& cmdList,
	const CommandQueue& cmdQueue)
{
	Resource intermediateVertexBuffer;
	UpdateBufferResource(
		d,
		cmdList,
		s_vertexBuffer,
		intermediateVertexBuffer,
		_countof(s_verts),
		sizeof(s_verts[0]),
		s_verts);
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
	// test
	//Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob;
	//HRESULT r = D3DReadFileToBlob(TEXT("shaders/simple_vs.hlsl.cso"), &vertexShaderBlob);
	//if (!SUCCEEDED(r))
	//	abort();
	//
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
