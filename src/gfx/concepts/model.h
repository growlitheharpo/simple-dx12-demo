#pragma once

#include "gfx/core/fwd.h"

#include "gfx/core/resource.h"

class Model
{
private:
	Resource m_vertexBuffer;
	Resource m_indexBuffer;

public:
	Model() = default;
	~Model();

	void Create(const Device& d, DestroyQueue& destroyQueue, const CommandList& cmdList);
	void Destroy();
};
