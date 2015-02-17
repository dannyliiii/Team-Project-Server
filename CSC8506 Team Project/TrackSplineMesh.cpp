#include "TrackSplineMesh.h"

void TrackSplineMesh::Generate()
{
	numVertices = 6u;

	GenerateVertices();

	BufferData();
}

void TrackSplineMesh::GenerateVertices()
{
	// TODO: Make this a Vector2?
	vertices = new Vector3[numVertices];

	vertices[0u] = Vector3(0.f, 0.f, 0.f);
	vertices[1u] = Vector3(1.f, 0.f, 0.f);
	vertices[2u] = Vector3(1.f, 1.f, 0.f);

	vertices[3u] = Vector3(0.f, 0.f, 0.f);
	vertices[4u] = Vector3(1.f, 1.f, 0.f);
	vertices[5u] = Vector3(0.f, 1.f, 0.f);
}

void TrackSplineMesh::Bind() const
{
	glBindVertexArray(arrayObject);
}

void TrackSplineMesh::Render() const
{
	glDrawArrays(type, 0u, numVertices);
}

void TrackSplineMesh::Unbind() const
{
	glBindVertexArray(0);
}
