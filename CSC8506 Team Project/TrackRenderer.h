#pragma once

#include "TrackSettings.h"

#include "TrackChunkSceneNode.h"
#include "TrackSplineSceneNode.h"

#include "TrackChunkMesh.h"
#include "TrackSplineMesh.h"

#include "Shader.h"

class TrackChunks;
class TrackSpline;

class TrackRenderer
{
public:

	TrackRenderer(TrackSettings const& settings, TrackChunks const& chunks, TrackSpline const& spline);
	~TrackRenderer();

	void Setup();

	void RenderChunks();
	void RenderSpline();

private:

	TrackSettings const& settings;

	TrackChunks const& chunks;
	TrackSpline const& spline;

	// Stuff for chunk rendering.
	TrackChunkSceneNode chunkSceneNode;
	Material chunkMaterial;
	Shader chunkShader;
	TrackChunkMesh chunkMesh;

	// Stuff for spline rendering.
	TrackSplineSceneNode splineSceneNode;
	Material splineMaterial;
	Shader splineShader;
	TrackSplineMesh splineMesh;

	unsigned int diffuseTexture;
};
