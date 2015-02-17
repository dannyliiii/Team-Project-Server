#include "TrackChunks.h"

#include "TrackSettings.h"

// This isn't really what we want to include...
#include "OGLRenderer.h"

TrackChunks::TrackChunks(TrackSettings const& settings):
	settings(settings),
	LastChunkIndex(0u),
	StartingChunk(0u) { }

TrackChunks::~TrackChunks()
{
	for (ChunksT::const_iterator chunk = chunks.begin(); chunk != chunks.end(); ++chunk)
		glDeleteTextures(1u, &chunk->TextureID);
}

void TrackChunks::CreateHeightMapTexture(unsigned int width, unsigned int height, unsigned int& id)
{
	glGenTextures(1u, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Do we actually need linear filtering?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_FLOAT, nullptr);
}

void TrackChunks::CreateChunks(unsigned int count)
{
	for (unsigned int i = 0u; i != count; ++i)
		chunks.push_back(Chunk());

	glActiveTexture(GL_TEXTURE0);

	for (unsigned int i = 0u; i != count; ++i)
		CreateHeightMapTexture(settings.HeightMapVertexCount, settings.HeightMapVertexCount, chunks[i].TextureID);

	glBindTexture(GL_TEXTURE_2D, 0u);

}

TrackChunks::ChunksT const& TrackChunks::GetChunks() const
{
	return chunks;
}

TrackChunks::ChunksT& TrackChunks::GetChunks()
{
	return chunks;
}

unsigned int TrackChunks::GetNextIndex()
{
	return LastChunkIndex++;
}

unsigned int TrackChunks::GetStartIndex() const
{
	return StartingChunk;
}

void TrackChunks::AdvanceStartIndex()
{
	StartingChunk = (StartingChunk + 1u) % settings.NumberOfChunks;
}
