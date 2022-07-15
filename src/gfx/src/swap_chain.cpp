#include "gfx/swap_chain.h"

#include "using.h"

#include "gfx/command_queue.h"
#include "gfx/window.h"

#include <heart/scope_exit.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

void SwapChain::CheckForTearingSupport()
{
	BOOL allowTearing = FALSE;

	// Rather than create the DXGI 1.5 factory interface directly, we create the
	// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the
	// graphics debugging tools which will not support the 1.5 factory interface
	// until a future update.
	Microsoft::WRL::ComPtr<IDXGIFactory4> factory4;
	if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
	{
		Microsoft::WRL::ComPtr<IDXGIFactory5> factory5;
		if (SUCCEEDED(factory4.As(&factory5)))
		{
			if (FAILED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing))))
			{
				allowTearing = FALSE;
			}
		}
	}

	m_tearingSupport = int(allowTearing);
}

SwapChain::~SwapChain()
{
	Destroy();
}

bool SwapChain::Create(const Window& window, const CommandQueue& commandQueue, uint32 width, uint32 height, uint32 bufferCount)
{
	HeartScopeGuard destroyGuard([&] {
		Destroy();
	});

	Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory4 = nullptr;
	UINT createFactoryFlags = 0;

#if USING(DEBUGGING)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	HRESULT r = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4));
	if (!SUCCEEDED(r))
		return false;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc = {1, 0};
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = bufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = HasTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	auto wnd = window.GetRawWindowHandle();
	auto cmdQueue = commandQueue.GetRawCommandQueueHandle();
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
	r = dxgiFactory4->CreateSwapChainForHwnd(cmdQueue, wnd, &swapChainDesc, nullptr, nullptr, &swapChain1);
	if (!SUCCEEDED(r))
		return false;

	// Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen will be handled manually.
	dxgiFactory4->MakeWindowAssociation(wnd, DXGI_MWA_NO_ALT_ENTER);

	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain4;
	r = swapChain1.As(&swapChain4);
	if (!SUCCEEDED(r))
		return false;

	// Moving out of COM, add a ref so we don't Release the object
	m_swapChain = swapChain4.Get();
	m_swapChain->AddRef();

	destroyGuard.Dismiss();
	return true;
}

void SwapChain::Destroy()
{
	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}
}

uint32 SwapChain::GetCurrentFrameIndex() const
{
	return m_swapChain->GetCurrentBackBufferIndex();
}

void SwapChain::Present(bool useVsync)
{
	uint32 syncInterval = useVsync ? 1 : 0;
	uint32 presentFlags = HasTearingSupport() && !useVsync ? DXGI_PRESENT_ALLOW_TEARING : 0;
	m_swapChain->Present(syncInterval, presentFlags);
}
