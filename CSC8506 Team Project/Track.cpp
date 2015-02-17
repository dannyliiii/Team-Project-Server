
#include "Track.h"

#include "OGLRenderer.h"
#include "Renderer.h"

#include <cassert>

Track::Track():

	chunks(settings), spline(settings),
	generator(settings, chunks, spline),
	renderer(settings, chunks, spline) { }


void Track::Generate(TrackSettings const& settings) {

	this->settings = settings;

	assert(settings.NumberOfChunks > 0u);

	renderer.Setup();
	generator.Generate();
}

void Track::Advance() {

	assert(settings.NumberOfChunks > 0u);

	generator.Advance();
	chunks.AdvanceStartIndex();
}

float Track::GetUpperBound() const
{
	return spline.GetUpperBound();
}

float Track::GetLowerBound() const
{
	return spline.GetLowerBound();
}

Vector3 Track::GetPositionAt(float distance) const
{
	return spline.GetPositionAt(distance);
}

Quaternion Track::GetRotationAt(float distance) const
{
	return spline.GetRotationAt(distance);
}

float Track::GetTrackHalfWidthAt(float distance) const
{
	return spline.GetTrackHalfWidthAt(distance);
}
