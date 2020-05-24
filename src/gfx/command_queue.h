#pragma once

#include "gfx/command_type.h"
#include "gfx/util.h"

#include <wrl.h>

#include <initializer_list>

struct ID3D12CommandQueue;

class Device;
class CommandList;

class CommandQueue
{
public:
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	
	ALLOW_GFX_ACCESS();
	ComPtr<ID3D12CommandQueue> GetRawCommandQueueHandle() const
	{
		return m_commandQueue;
	}

public:

	bool Create(const Device& d, CommandQueueType type);
	void Destroy();

	void Execute(std::initializer_list<CommandList*> lists);
};
