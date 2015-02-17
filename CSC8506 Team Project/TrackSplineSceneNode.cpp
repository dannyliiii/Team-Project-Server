#include "TrackSplineSceneNode.h"

#include "TrackRenderer.h"

#include "Renderer.h"

void TrackSplineSceneNode::Connect()
{
	Renderer::GetRenderer().AddNode(this);
}

void TrackSplineSceneNode::Disconnect()
{
	Renderer::GetRenderer().RemoveNode(this);
}

void TrackSplineSceneNode::Draw(const OGLRenderer& renderer)
{
	trackRenderer.RenderSpline();
}
