#include "TrackChunkMesh.h"

#include <cassert>

TrackChunkMesh::TrackChunkMesh():
	xSize(0u), zSize(0u) { }

void TrackChunkMesh::Generate(unsigned int dataSize)
{
	xSize = dataSize;
	zSize = dataSize;

	assert(xSize > 0u);
	assert(zSize > 0u);

	numVertices = xSize * zSize;

	GenerateVertices();
	GenerateTextureCoords();

	numIndices = (xSize - 1u) * (zSize - 1u) * 6u;

	GenerateGridIndices();

	GenerateNormals();
	GenerateTangents();
	BufferData();
}

void TrackChunkMesh::GenerateVertices()
{
	vertices = new Vector3[numVertices];

	float xStep = 1.f / (xSize - 1u);
	float zStep = 1.f / (zSize - 1u);

	for (int x = 0; x != xSize; ++x) {
		for (int z = 0; z != zSize; ++z) {

			unsigned int index = x * xSize + z;

			vertices[index] = Vector3(x * xStep, 0.f, -z * zStep);
		}
	}
}

void TrackChunkMesh::GenerateTextureCoords()
{
	textureCoords = new Vector2[numVertices];

	float xTexel = 1.f / xSize;
	float zTexel = 1.f / zSize;

	float xHalfTexel = xTexel * 0.5f;
	float zHalfTexel = zTexel * 0.5f;

	float xStep = (1.f - xTexel) / (xSize - 1u);
	float zStep = (1.f - zTexel) / (zSize - 1u);

	for (int x = 0; x != xSize; ++x) {
		for (int z = 0; z != zSize; ++z) {

			unsigned int index = x * xSize + z;

			textureCoords[index] = Vector2(
				xHalfTexel + x * xStep, 
				zHalfTexel + z * zStep);
		}
	}
}

void TrackChunkMesh::GenerateGridIndices()
{
	indices = new GLuint[numIndices];

	unsigned int index = 0u;

	for (unsigned int x = 0u; x != xSize - 1u; ++x) {
		for (unsigned int z = 0u; z != zSize - 1u; ++z) {

			int a = (x * (xSize)) + z;
			int b = ((x + 1) * (xSize)) + z;
			int c = ((x + 1) * (xSize)) + (z + 1);
			int d = (x * (xSize)) + (z + 1);

			indices[index++] = a;
			indices[index++] = b;
			indices[index++] = c;
			
			indices[index++] = a;
			indices[index++] = c;
			indices[index++] = d;
		}
	}
}

void TrackChunkMesh::Bind() const
{
	glBindVertexArray(arrayObject);
}

void TrackChunkMesh::Render() const
{
	glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
}

void TrackChunkMesh::Unbind() const
{
	glBindVertexArray(0);
}
