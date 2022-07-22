#include "gfx/concepts/model.h"

#include "gfx/core/command_list.h"
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

void Model::Create(const Device& d, DestroyQueue& destroyQueue, CommandList& cmdList)
{
	Resource intermediateVertexBuffer;
	UpdateBufferResource(d,
		cmdList,
		m_vertexBuffer,
		intermediateVertexBuffer,
		HeartCountOf(s_verts),
		sizeof(s_verts[0]),
		s_verts);

	Resource intermediateIndexBuffer;
	UpdateBufferResource(d,
		cmdList,
		m_indexBuffer,
		intermediateIndexBuffer,
		HeartCountOf(s_indices),
		sizeof(s_indices[0]),
		s_indices);

	m_vertexView.Create(m_vertexBuffer, sizeof(s_verts), sizeof(s_verts[0]));
	m_indexView.Create(m_indexBuffer, ResourceFormat::R16_Uint, sizeof(s_indices));

	cmdList.Transition(m_vertexBuffer, ResourceState::CopyDest, ResourceState::GenericRead);
	cmdList.Transition(m_indexBuffer, ResourceState::CopyDest, ResourceState::IndexBuffer);

	destroyQueue.DestroyResource(intermediateVertexBuffer);
	destroyQueue.DestroyResource(intermediateIndexBuffer);
}

void Model::Destroy()
{
	m_indexBuffer.Destroy();
	m_vertexBuffer.Destroy();
}

void Model::Draw(CommandList& cmdList)
{
	cmdList.SetVertexBuffer(m_vertexView);
	cmdList.SetIndexBuffer(m_indexView);

	// Need depth buffer work first
	// cmdList.DrawIndexedInstanced((uint32)HeartCountOf(s_indices), 1);
}
