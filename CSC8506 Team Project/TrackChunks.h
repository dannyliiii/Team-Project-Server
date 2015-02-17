#pragma once

#include "Vector3.h"

#include <vector>

class TrackSettings;

class TrackChunks
{
public:

	TrackChunks(TrackSettings const& settings);

	~TrackChunks();

	void CreateChunks(unsigned int count);

	class Chunk {
	public:

		Chunk():
			Index(0u),
			Origin(0.f, 0.f, 0.f),
			TextureID(0u) { }

		unsigned int Index;

		Vector3 Origin;

		unsigned int TextureID;
	};

	typedef std::vector<Chunk> ChunksT;

	ChunksT const& GetChunks() const;
	ChunksT& GetChunks();

	unsigned int GetNextIndex();

	unsigned int GetStartIndex() const;
	void AdvanceStartIndex();

private:

	void CreateHeightMapTexture(unsigned int width, unsigned int height, unsigned int& id);

	TrackSettings const& settings;

	ChunksT chunks;

	unsigned int LastChunkIndex;
	unsigned int StartingChunk;
};
