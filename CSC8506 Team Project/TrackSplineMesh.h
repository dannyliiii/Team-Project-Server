#pragma once

#include "Mesh.h"

class TrackSplineMesh : public Mesh
{
public:

	void Generate();

	void Bind() const;
	void Render() const;
	void Unbind() const;

private:

	void GenerateVertices();
};
