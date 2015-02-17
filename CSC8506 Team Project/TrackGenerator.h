#pragma once

#include "TrackChunks.h"

#include "TrackHeightGenMesh.h"
#include "TrackSplineMesh.h"

#include "Vector2.h"
#include "Shader.h"

class TrackSettings;
class TrackSpline;

class TrackGenerator
{
public:

	TrackGenerator(TrackSettings const& settings, TrackChunks& chunks, TrackSpline& spline);

	~TrackGenerator();

	void Generate();

	void Advance();

private:

	TrackSettings const& settings;
	TrackChunks& chunks;
	TrackSpline& spline;

	// Chunks
	void SetupChunks();
	void RegenerateChunk(unsigned int index);

	Vector2 WorldToTextureCoordinates(Vector2 const& position2D, TrackChunks::Chunk const& chunk);
	float WorldToTextureOffset(float distance);
	float LookupHeight(Vector2 const& position2D, TrackChunks::Chunk const& chunk);

	// Splines
	void SetupSpline();
	void GenerateStraightSplineChunk(unsigned int index);

	// Heightmap smoothing
	void SmoothHeightMaps();
	void SmoothHeightMap(unsigned int index);


	// Stuff for height map generation
	Shader heightGenShader;
	Shader heightSmoothShader;
	TrackHeightGenMesh heightGenMesh;
	GLuint heightGenFramebuffer;
	TrackSplineMesh splineMesh;
};
