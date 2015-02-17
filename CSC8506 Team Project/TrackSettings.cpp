#include "TrackSettings.h"

TrackSettings::TrackSettings():
NumberOfChunks(6u),
	ChunkWorldSize(2048.f),
	HeightMapVertexCount(256u),
	TerrainBaseHeight(0.f),
	TerrainScale(100.f),
	NoiseScale(4.f),
	SplinePointsPerChunk(10u),
	SplineMeshQuadsPerSection(11u) { }
