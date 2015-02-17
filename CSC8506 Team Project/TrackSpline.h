#pragma once

#include "TrackSettings.h"

#include "Vector3.h"
#include "Quaternion.h"

#include <deque>

class TrackSpline
{
public:

	TrackSpline(TrackSettings const& settings);

	void DestroySplineChunk();


	float GetLowerBound() const;
	float GetUpperBound() const;


	Vector3 GetPositionAt(float distance) const;
	Quaternion GetRotationAt(float distance) const;
	float GetTrackHalfWidthAt(float distance) const;


	Vector3 GetPositionAtAdjustedDistance(float distance) const;
	float GetTrackHalfWidthAtAdjustedDistance(float distance) const;

	
	class SplinePoint
	{
	public:

		SplinePoint():
			Position(0.f, 0.f, 0.f),
			Rotation(),
			TrackHalfWidth(5.f),
			ChunkIndex(0u) { }

		Vector3 Position;
		Quaternion Rotation;
		float TrackHalfWidth;

		unsigned int ChunkIndex;
	};

	typedef std::deque<SplinePoint> SplinePointsT;

	SplinePointsT const& GetPoints() const
	{
		return splinePoints;
	}

	SplinePointsT& GetPoints()
	{
		return splinePoints;
	}

private:

	TrackSettings const& settings;

	float GetAdjustedDistance(float distance) const;

	SplinePointsT splinePoints;

	float splineStartOffset;
	unsigned int splinePointsRemoved;
};
