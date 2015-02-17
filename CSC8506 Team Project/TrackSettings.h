#pragma once

class TrackSettings
{
public:

	TrackSettings();

	unsigned int NumberOfChunks;
	float ChunkWorldSize;

	unsigned int HeightMapVertexCount;

	float TerrainBaseHeight;
	float TerrainScale;
	float NoiseScale;

	unsigned int SplinePointsPerChunk;
	unsigned int SplineMeshQuadsPerSection;
};
