#pragma once

#include "Mesh.h"

class TrackHeightGenMesh : public Mesh
{
public:

	TrackHeightGenMesh();

	void Generate();

	void Render() const;

private:

	void GenerateVertices();
};
