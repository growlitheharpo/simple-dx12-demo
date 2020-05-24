#pragma once

#include "gfx/util.h"

#include "types.h"

// fuck you, Windows.h
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;

typedef intptr(__stdcall* WndProc)(HWND, uint32, uint64, int64);

class Window
{
private:
	HWND m_window;

	ALLOW_GFX_ACCESS();
	HWND GetRawWindowHandle() const
	{
		return m_window;
	}

public:
	bool Create(WndProc winProc, HINSTANCE instance, uint32 width, uint32 height);
	void Destroy();

	void Show();
};
