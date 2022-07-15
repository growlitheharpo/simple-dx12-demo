#include "gfx/core/window.h"

#include <Windows.h> // kill me

// I hate Windows.h
#if defined(CreateWindow)
#undef CreateWindow
#endif

#include <algorithm>

bool Window::Create(WndProc wndProc, HINSTANCE instance, uint32 width, uint32 height)
{
	auto windowClassName = TEXT("DX12DemoWindowClass");
	auto windowTitle = TEXT("DX12DemoWindow");

	WNDCLASSEXW windowClass = {};

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = wndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = instance;
	windowClass.hIcon = ::LoadIcon(instance, NULL);
	windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = ::LoadIcon(instance, NULL);

	WORD result = ::RegisterClassExW(&windowClass);
	if (result <= 0)
		return false;

	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	m_width = windowRect.right - windowRect.left;
	m_height = windowRect.bottom - windowRect.top;

	// Center the window within the screen. Clamp to 0, 0 for the top-left corner.
	int windowX = std::max<int>(0, (screenWidth - m_width) / 2);
	int windowY = std::max<int>(0, (screenHeight - m_height) / 2);

	HWND hWnd = ::CreateWindowExW(
		NULL,
		windowClassName,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		windowX,
		windowY,
		m_width,
		m_height,
		NULL,
		NULL,
		instance,
		nullptr);

	DWORD x = GetLastError();

	if (!hWnd)
		return false;

	m_window = hWnd;
	return true;
}

void Window::Show()
{
	::ShowWindow(m_window, SW_SHOW);
}
