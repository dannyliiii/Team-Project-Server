#pragma once

#include "Mesh.h"

class TrackChunkMesh : public Mesh
{
public:

	TrackChunkMesh();

	void Generate(unsigned int dataSize);

	void Bind() const;
	void Render() const;
	void Unbind() const;

private:

	void GenerateVertices();
	void GenerateTextureCoords();

	void GenerateGridIndices();

	unsigned int xSize, zSize;
};
