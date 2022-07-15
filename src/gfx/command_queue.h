#pragma once

#include "gfx/fwd.h"

#include "gfx/enums/command_type.h"
#include "gfx/util.h"

#include <initializer_list>

struct ID3D12CommandQueue;

class CommandQueue
{
private:
	ID3D12CommandQueue* m_commandQueue = nullptr;

	ALLOW_GFX_ACCESS();
	ID3D12CommandQueue* GetRawCommandQueueHandle() const
	{
		return m_commandQueue;
	}

public:
	CommandQueue() = default;
	~CommandQueue();

	bool Create(const Device& d, CommandQueueType type);
	void Destroy();

	void Execute(std::initializer_list<CommandList*> lists);
};
