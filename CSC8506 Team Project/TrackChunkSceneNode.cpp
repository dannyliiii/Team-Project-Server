#include "TrackChunkSceneNode.h"

#include "TrackRenderer.h"

#include "Renderer.h"

void TrackChunkSceneNode::Connect()
{
	Renderer::GetRenderer().AddNode(this);
}

void TrackChunkSceneNode::Disconnect()
{
	Renderer::GetRenderer().RemoveNode(this);
}

void TrackChunkSceneNode::Draw(const OGLRenderer& renderer)
{
	trackRenderer.RenderChunks();
}
