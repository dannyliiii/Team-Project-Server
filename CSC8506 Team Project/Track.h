#pragma once

#include "TrackSettings.h"

#include "TrackChunks.h"
#include "TrackSpline.h"

#include "TrackGenerator.h"
#include "TrackRenderer.h"

#include "TrackHeightGenMesh.h"

#include "Vector3.h"
#include "Quaternion.h"

class Track {
public:

	Track();

	void Generate(TrackSettings const& settings = TrackSettings());

	// Regenerate the last chunk, and put it at the front.
	void Advance();

	// Returns the spline distance for the first chunk of currently loaded track section.
	float GetLowerBound() const;
	// Returns the spline distane for the last chunk of current loaded track section.
	float GetUpperBound() const;

	// Spline functions:
	Vector3 GetPositionAt(float distance) const;
	Quaternion GetRotationAt(float distance) const;

	float GetTrackHalfWidthAt(float distance) const;


private:

	unsigned int GetChunkIndex(unsigned int index);

	TrackSettings settings;


	TrackChunks chunks;
	TrackSpline spline;

	TrackGenerator generator;
	TrackRenderer renderer;
};
