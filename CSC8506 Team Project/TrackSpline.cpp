#include "TrackSpline.h"

#include "TrackSettings.h"

#include <cassert>

TrackSpline::TrackSpline(TrackSettings const& settings):
	settings(settings),
	splineStartOffset(1.5f),
	splinePointsRemoved(0u) { }

void TrackSpline::DestroySplineChunk()
{
	assert(!splinePoints.empty());

	unsigned int index = splinePoints[0].ChunkIndex;

	for (SplinePointsT::iterator i = splinePoints.begin(); i != splinePoints.end();)
	{
		if (i->ChunkIndex != index)
			break;

		i = splinePoints.erase(i);

		++splinePointsRemoved;
	}
}

float TrackSpline::GetLowerBound() const
{
	return static_cast<float>(splinePointsRemoved);
}

float TrackSpline::GetUpperBound() const
{
	float upperBound = splinePointsRemoved +
		static_cast<float>(settings.NumberOfChunks) * static_cast<float>(settings.SplinePointsPerChunk);

	// The splineStartOffset is added to all GetFooAt() distances, so we need to 
	// take that much off the upper bound to prevent running over the end.
	
	// Requests can't be in the last spline segment, hence taking away 2.f (distance is 
	// basically an index, so -1 would be the last point in the spline array, and -2 would be 
	// the end point of the last valid segment).
	
	// This bound is not inclusive (i.e. would fail if GetFooAt(UpperBound()) was requested).
	// So take 0.01f away to make it an approximate inclusive range.

	float adjustment = splineStartOffset + 2.f + 0.01f;

	return upperBound - adjustment;
}

float TrackSpline::GetAdjustedDistance(float distance) const
{
	return (distance + splineStartOffset) - splinePointsRemoved;
}

Vector3 TrackSpline::GetPositionAt(float distance) const
{
	return GetPositionAtAdjustedDistance(GetAdjustedDistance(distance));
}

Quaternion TrackSpline::GetRotationAt(float distance) const
{
	// TEMP!
	// TODO: create normal map, get normals at spline points...
	return Quaternion();
}

float TrackSpline::GetTrackHalfWidthAt(float distance) const
{
	return GetTrackHalfWidthAtAdjustedDistance(GetAdjustedDistance(distance));
}

namespace
{

	// TODO: put this somewhere else?
	template<class Value>
	Value CatmullRomSpline(Value const& P0, Value const& P1, Value const& P2, Value const& P3, float t)
	{
		return 0.5f * (
			(2.f * P1) +
			(-P0 + P2) * t +
			(2.f * P0 - 5.f * P1 + 4.f * P2 - P3) * t * t +
			(-P0 + 3.f * P1 - 3.f * P2 + P3) * t * t * t);
	}

} // unnamed

Vector3 TrackSpline::GetPositionAtAdjustedDistance(float distance) const
{
	assert(splinePoints.size() > 4u);

	unsigned int index = static_cast<unsigned int>(distance);

	assert(index >= 1u);
	assert(index <= splinePoints.size() - 3u);

	float t = distance - static_cast<float>(index);

	SplinePoint const& point0 = splinePoints[index - 1u];
	SplinePoint const& point1 = splinePoints[index];
	SplinePoint const& point2 = splinePoints[index + 1u];
	SplinePoint const& point3 = splinePoints[index + 2u];

	Vector3 const& p0 = point0.Position;
	Vector3 const& p1 = point1.Position;
	Vector3 const& p2 = point2.Position;
	Vector3 const& p3 = point3.Position;

	return CatmullRomSpline(p0, p1, p2, p3, t);
}

float TrackSpline::GetTrackHalfWidthAtAdjustedDistance(float distance) const
{
	assert(splinePoints.size() > 4u);

	unsigned int index = static_cast<unsigned int>(distance);

	assert(index >= 1u);
	assert(index <= splinePoints.size() - 3u);

	float t = distance - static_cast<float>(index);

	SplinePoint const& point0 = splinePoints[index - 1u];
	SplinePoint const& point1 = splinePoints[index];
	SplinePoint const& point2 = splinePoints[index + 1u];
	SplinePoint const& point3 = splinePoints[index + 2u];

	float const& p0 = point0.TrackHalfWidth;
	float const& p1 = point1.TrackHalfWidth;
	float const& p2 = point2.TrackHalfWidth;
	float const& p3 = point3.TrackHalfWidth;

	return CatmullRomSpline(p0, p1, p2, p3, t);
}
