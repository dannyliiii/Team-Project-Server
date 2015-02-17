#pragma once

#include "SceneNode.h"

class TrackRenderer;

// It's easier to have the Track class do the rendering itself.
// Which means this basically just forwards the draw call.

class OGLRenderer;

class TrackSplineSceneNode : public SceneNode
{
public:

	TrackSplineSceneNode(TrackRenderer& trackRenderer):
		trackRenderer(trackRenderer) { }

	void Connect();
	void Disconnect();

	void Draw(const OGLRenderer& renderer) override;

private:

	TrackRenderer& trackRenderer;
};
