#include "TrackGenerator.h"

#include "TrackSettings.h"

#include "TrackChunks.h"
#include "TrackSpline.h"

#include <cassert>

TrackGenerator::TrackGenerator(TrackSettings const& settings, TrackChunks& chunks, TrackSpline& spline):
	settings(settings),
	chunks(chunks), spline(spline),
	heightGenShader(
		SHADERDIR"TrackHeightGenVertex.glsl",
		SHADERDIR"TrackHeightGenFragment.glsl"),
	heightSmoothShader(
		SHADERDIR"TrackHeightSmoothVertex.glsl",
		SHADERDIR"TrackHeightSmoothFragment.glsl")
{
	assert(heightGenShader.LinkProgram());
	assert(heightSmoothShader.LinkProgram());

	glGenFramebuffers(1, &heightGenFramebuffer);
}

TrackGenerator::~TrackGenerator()
{
	glDeleteFramebuffers(1u, &heightGenFramebuffer);
}

void TrackGenerator::Generate()
{
	heightGenMesh.Generate();
	splineMesh.Generate();

	SetupChunks();
	SetupSpline();
	SmoothHeightMaps();
}

void TrackGenerator::Advance()
{
	spline.DestroySplineChunk();

	unsigned int startIndex = chunks.GetStartIndex();

	RegenerateChunk(startIndex);
	GenerateStraightSplineChunk(startIndex);
	SmoothHeightMap(startIndex);
}

void TrackGenerator::SetupChunks() {

	chunks.CreateChunks(settings.NumberOfChunks);

	for (unsigned int i = 0u; i != settings.NumberOfChunks; ++i)
		RegenerateChunk(i);
}

void TrackGenerator::RegenerateChunk(unsigned int index) {

	assert(!chunks.GetChunks().empty());
	assert(index < chunks.GetChunks().size());

	TrackChunks::Chunk& chunk = chunks.GetChunks()[index];

	chunk.Index = chunks.GetNextIndex();
	chunk.Origin = Vector3(-settings.ChunkWorldSize * 0.5f, 0.f, chunk.Index * -settings.ChunkWorldSize);

	// TODO: abstract this into a separate class!

	glBindFramebuffer(GL_FRAMEBUFFER, heightGenFramebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, chunk.TextureID, 0);

	glViewport(0, 0, settings.HeightMapVertexCount, settings.HeightMapVertexCount);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_BLEND);

	glUseProgram(heightGenShader.GetProgram());

	Matrix4 projectionMatrix = Matrix4::Orthographic(-1.f, 1.f,
		static_cast<float>(settings.HeightMapVertexCount), 0.f, 
		static_cast<float>(settings.HeightMapVertexCount), 0.f);

	glUniformMatrix4fv(
		heightGenShader.GetUniformLocation("projMatrix"), 1u, false,
		&projectionMatrix.values[0]);

	glUniform2f(
		heightGenShader.GetUniformLocation("textureSize"),
		static_cast<float>(settings.HeightMapVertexCount), 
		static_cast<float>(settings.HeightMapVertexCount));

	glUniform2f(
		heightGenShader.GetUniformLocation("noiseOrigin"),
		0.f, static_cast<float>(chunk.Index));

	glUniform2f(
		heightGenShader.GetUniformLocation("noiseScale"),
		settings.NoiseScale, settings.NoiseScale);

	heightGenMesh.Render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0u);

	glUseProgram(0u);

	// HACK! TODO:
	// Better way of managing OpenGL states???
	glEnable(GL_BLEND);

	// HACK!!!
	// Not actually sure that this is the right viewport size...
	Vector2 screenSize = Window::GetWindow().GetScreenSize();
	glViewport(0, 0, static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y));
}

Vector2 TrackGenerator::WorldToTextureCoordinates(Vector2 const& position2D, TrackChunks::Chunk const& chunk)
{
	Vector2 coords = position2D - Vector2(-settings.ChunkWorldSize * 0.5f, chunk.Origin.z);
	coords.y = -coords.y;
	coords /= Vector2(settings.ChunkWorldSize, settings.ChunkWorldSize);
	coords *= static_cast<float>(settings.HeightMapVertexCount);

	return coords;
}

float TrackGenerator::WorldToTextureOffset(float distance)
{
	return (distance / settings.ChunkWorldSize) * settings.HeightMapVertexCount;
}

float TrackGenerator::LookupHeight(Vector2 const& position2D, TrackChunks::Chunk const& chunk)
{
	Vector2 coords = WorldToTextureCoordinates(position2D, chunk);

	float height = 0.f;
	glReadPixels(
		static_cast<int>(coords.x), static_cast<int>(coords.y), 1u, 1u, 
		GL_RED, GL_FLOAT, &height);

	return settings.TerrainBaseHeight + height * settings.TerrainScale;
}

void TrackGenerator::SetupSpline()
{
	assert(settings.NumberOfChunks > 0u);

	for (unsigned int i = 0u; i != settings.NumberOfChunks; ++i)
		GenerateStraightSplineChunk(i);
}

void TrackGenerator::GenerateStraightSplineChunk(unsigned int index)
{
	assert(!chunks.GetChunks().empty());
	assert(index < chunks.GetChunks().size());
	assert(settings.SplinePointsPerChunk >= 4u);

	TrackChunks::Chunk& chunk = chunks.GetChunks()[index];

	float startZ = chunk.Index * -settings.ChunkWorldSize;
	float stepSize = settings.ChunkWorldSize / settings.SplinePointsPerChunk;

	glBindFramebuffer(GL_FRAMEBUFFER, heightGenFramebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, chunk.TextureID, 0);

	TrackSpline::SplinePointsT& splinePoints = spline.GetPoints();

	for (unsigned int i = 0u; i != settings.SplinePointsPerChunk; ++i)
	{
		TrackSpline::SplinePoint point = TrackSpline::SplinePoint();

		Vector2 position2D = Vector2(0.f, startZ + i * -stepSize);

		float height = LookupHeight(position2D, chunk);

		point.Position = Vector3(position2D.x, height, position2D.y);
		point.Rotation = Quaternion();

		point.ChunkIndex = chunk.Index;

		splinePoints.push_back(std::move(point));
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0u);
}

void TrackGenerator::SmoothHeightMaps()
{
	assert(settings.NumberOfChunks > 0u);

	for (unsigned int i = 0u; i != settings.NumberOfChunks; ++i)
		SmoothHeightMap(i);
}

void TrackGenerator::SmoothHeightMap(unsigned int index)
{
	assert(!chunks.GetChunks().empty());
	assert(index < chunks.GetChunks().size());

	TrackChunks::Chunk& chunk = chunks.GetChunks()[index];

	glBindFramebuffer(GL_FRAMEBUFFER, heightGenFramebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, chunk.TextureID, 0);

	glViewport(0, 0, settings.HeightMapVertexCount, settings.HeightMapVertexCount);

	glDisable(GL_BLEND);

	// TODO:
	// Can we use alpha blending with a one channel texture?
	// If so then blend smoothly with distance from the track

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(heightSmoothShader.GetProgram());

	Matrix4 projectionMatrix = Matrix4::Orthographic(
		-1.f, 1.f,
		static_cast<float>(settings.HeightMapVertexCount), 0.f, 
		static_cast<float>(settings.HeightMapVertexCount), 0.f);

	glUniformMatrix4fv(
		heightSmoothShader.GetUniformLocation("projMatrix"), 1u, false,
		&projectionMatrix.values[0]);

	splineMesh.Bind();

	TrackSpline::SplinePointsT const& splinePoints = spline.GetPoints();

	assert(splinePoints.size() > 4u);
	assert(settings.SplineMeshQuadsPerSection > 1u);

	float subdivision = 1.f / (settings.SplineMeshQuadsPerSection - 1u);

	for (unsigned int pointIndex = 1u; pointIndex != splinePoints.size() - 3u; ++pointIndex)
	{
		// Is there a better way of doing this?
		if (splinePoints[pointIndex].ChunkIndex != index)
			continue;

		for (unsigned int step = 0u; step != settings.SplineMeshQuadsPerSection - 1u; ++step)
		{
			float distance = pointIndex + step * subdivision;
			float nextDistance = pointIndex + (step + 1u) * subdivision;

			Vector3 position1 = spline.GetPositionAtAdjustedDistance(distance);
			Vector2 coords1 = WorldToTextureCoordinates(Vector2(position1.x, position1.z), chunk);

			glUniform2f(
				heightSmoothShader.GetUniformLocation("splineCoords[0]"),
				coords1.x, coords1.y);

			Vector3 position2 = spline.GetPositionAtAdjustedDistance(nextDistance);
			Vector2 coords2 = WorldToTextureCoordinates(Vector2(position2.x, position2.z), chunk);

			glUniform2f(
				heightSmoothShader.GetUniformLocation("splineCoords[1]"),
				coords2.x, coords2.y);

			float heights1 = (position1.y - settings.TerrainBaseHeight) / settings.TerrainScale;

			glUniform1f(
				heightSmoothShader.GetUniformLocation("splineHeights[0]"),
				heights1);

			float heights2 = (position2.y - settings.TerrainBaseHeight) / settings.TerrainScale;

			glUniform1f(
				heightSmoothShader.GetUniformLocation("splineHeights[1]"),
				heights2);

			float width1 = spline.GetTrackHalfWidthAtAdjustedDistance(distance);
			float coordWidth1 = WorldToTextureOffset(width1);

			glUniform1f(
				heightSmoothShader.GetUniformLocation("splineWidth[0]"),
				coordWidth1 * 3.f); // TEMP!

			float width2 = spline.GetTrackHalfWidthAtAdjustedDistance(distance);
			float coordWidth2 = WorldToTextureOffset(width2);

			glUniform1f(
				heightSmoothShader.GetUniformLocation("splineWidth[1]"),
				coordWidth2 * 3.f); // TEMP!

			splineMesh.Render();
		}
	}

	splineMesh.Unbind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0u);

	glUseProgram(0u);

	// HACK! TODO:
	// Better way of managing OpenGL states???
	glEnable(GL_BLEND);

	// HACK!!!
	// Not actually sure that this is the right viewport size...
	Vector2 screenSize = Window::GetWindow().GetScreenSize();
	glViewport(0, 0, static_cast<unsigned int>(screenSize.x), static_cast<unsigned int>(screenSize.y));
}
