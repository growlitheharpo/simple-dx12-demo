// ************************************************************
// https://www.3dgep.com/learning-directx-12-1/
// ************************************************************

// Mine
#include "gfx/core/command_list.h"
#include "gfx/core/command_queue.h"
#include "gfx/core/descriptor_heap.h"
#include "gfx/core/destroy_queue.h"
#include "gfx/core/device.h"
#include "gfx/core/fence.h"
#include "gfx/core/frame_ctx.h"
#include "gfx/core/pipeline_state_object.h"
#include "gfx/core/root_signature.h"
#include "gfx/core/shader.h"
#include "gfx/core/subresource_data.h"
#include "gfx/core/swap_chain.h"
#include "gfx/core/window.h"

#include "gfx/concepts/model.h"
#include "gfx/formats/vertex.h"

#include "types.h"
#include "using.h"

#include <heart/countof.h>
#include <heart/stl/vector.h>

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
static DestroyQueue s_destroyQueue;
static Fence s_fence;
static SwapChain s_swapChain;
static DescriptorHeap s_rtvHeap;
static FrameCtx s_frames[NumFrames];
static CommandQueue s_commandQueue;
static CommandList s_commandList;

static RootSignature s_rootSignature;
static Shader s_vertexShader;
static Shader s_pixelShader;
static PipelineStateObject s_pso;

static Model s_model;
bool s_useVsync = true;

void Update()
{
}

void Render()
{
	uint32 currentBackBufferIndex = s_swapChain.GetCurrentFrameIndex();
	auto& endingFrame = s_frames[currentBackBufferIndex];

	// Transition from Present back to RTV
	s_commandList.Transition(endingFrame.GetBackBuffer(), ResourceState::Present, ResourceState::RenderTarget);

	// Clear to blue
	s_commandList.Clear(s_device, s_rtvHeap, s_swapChain, Vector4f(0.4f, 0.6f, 0.9f, 1.0f));

	// Draw the model
	s_commandList.SetRootSignature(s_rootSignature);
	s_commandList.SetPipelineState(s_pso);
	s_commandList.SetPrimitiveTopology(5);
	s_model.Draw(s_commandList);

	// Present
	s_commandList.Transition(endingFrame.GetBackBuffer(), ResourceState::RenderTarget, ResourceState::Present);

	// Execute
	s_commandList.Close();
	s_commandQueue.Execute({&s_commandList});

	// Present and signal
	{
		s_swapChain.Present(s_useVsync);
		endingFrame.SetFenceValue(s_fence.Signal(s_commandQueue));
	}

	// Wait for new frame
	currentBackBufferIndex = s_swapChain.GetCurrentFrameIndex();
	auto& beginningFrame = s_frames[currentBackBufferIndex];
	s_fence.WaitForFence(beginningFrame.GetFenceValue());

	// Prepare new frame
	s_destroyQueue.NextFrame();
	beginningFrame.ResetCommandAllocator();
	s_commandList.Reset(beginningFrame);
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

			// Resize(width, height);
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
	// Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob;
	// HRESULT r = D3DReadFileToBlob(TEXT("shaders/simple_vs.hlsl.cso"), &vertexShaderBlob);
	// if (!SUCCEEDED(r))
	//	abort();
	//
	s_window.Create(wndProc, hInstanceExe, s_winWidth, s_winHeight);

	s_device.Create(false);

	s_destroyQueue.Setup(NumFrames);

	s_commandQueue.Create(s_device, CommandQueueType::Direct);
	s_swapChain.Create(s_window, s_commandQueue, s_winWidth, s_winHeight, NumFrames);

	s_rtvHeap.Create(s_device, DescriptorHeapType::RTV, NumFrames);

	for (size_t i = 0; i < _countof(s_frames); ++i)
		s_frames[i].Create(i, s_device, s_swapChain, s_rtvHeap, CommandQueueType::Direct);

	s_rootSignature.Create(s_device);
	s_vertexShader.Create("shaders/simple_vs.hlsl.cso");
	s_pixelShader.Create("shaders/simple_ps.hlsl.cso");

	{
		hrt::vector<PipelineStateStreamEntry> pipelineStream;

		pipelineStream.emplace_back(PipelineStateSubobjectType::RootSignature).rootSignature = &s_rootSignature;

		auto& inputLayout = pipelineStream.emplace_back(PipelineStateSubobjectType::InputLayout);
		PipelineStateInputElementDesc inputLayouts[] = {
			PipelineStateInputElementDesc {"POSITION", 0, ResourceFormat::R32G32B32_Float},
			PipelineStateInputElementDesc {"COLOR", 0, ResourceFormat::R32G32B32_Float},
		};
		inputLayout.inputLayout.inputElements = inputLayouts;
		inputLayout.inputLayout.inputElementCount = HeartCountOf(inputLayouts);

		pipelineStream.emplace_back(PipelineStateSubobjectType::PrimitiveTopology).topology = 3;
		pipelineStream.emplace_back(PipelineStateSubobjectType::VertexShader).shader = &s_vertexShader;
		pipelineStream.emplace_back(PipelineStateSubobjectType::PixelShader).shader = &s_pixelShader;
		pipelineStream.emplace_back(PipelineStateSubobjectType::DepthStencilFormat).depthStencilFormat = ResourceFormat::D32_Float;

		auto& rtvs = pipelineStream.emplace_back(PipelineStateSubobjectType::RenderTargetFormats);
		rtvs.renderTargetFormats[0] = ResourceFormat::R8G8B8A8_Unorm;
		rtvs.numRenderTargets = 1;

		s_pso.Create(s_device, std::span(pipelineStream.data(), pipelineStream.size()));
	}

	uint32 currentBackBufferIndex = s_swapChain.GetCurrentFrameIndex();
	s_commandList.Create(s_device, s_frames[currentBackBufferIndex], CommandQueueType::Direct);

	s_fence.Create(s_device);

	s_model.Create(s_device, s_destroyQueue, s_commandList);

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

	s_destroyQueue.Flush();

	return 0;
}
