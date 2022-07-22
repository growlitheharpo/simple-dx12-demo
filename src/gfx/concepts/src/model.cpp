#include "gfx/concepts/model.h"

#include "gfx/core/destroy_queue.h"
#include "gfx/formats/vertex.h"

#include <heart/countof.h>

// TODO: loading models
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

Model::~Model()
{
	Destroy();
}

void Model::Create(const Device& d, DestroyQueue& destroyQueue, const CommandList& cmdList)
{
	Resource intermediateVertexBuffer;
	UpdateBufferResource(d,
		cmdList,
		m_vertexBuffer,
		intermediateVertexBuffer,
		HeartCountOf(s_verts),
		sizeof(s_verts[0]),
		s_verts);

	destroyQueue.DestroyResource(intermediateVertexBuffer);
}

void Model::Destroy()
{
	m_indexBuffer.Destroy();
	m_vertexBuffer.Destroy();
}
