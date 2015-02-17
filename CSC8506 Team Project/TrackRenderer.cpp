#include "TrackRenderer.h"

#include "TrackChunks.h"
#include "TrackSpline.h"

#include <cassert>

TrackRenderer::TrackRenderer(TrackSettings const& settings, TrackChunks const& chunks, TrackSpline const& spline):
	settings(settings),
	chunks(chunks), spline(spline),
	chunkSceneNode(*this),
	splineSceneNode(*this),
	chunkShader(
		SHADERDIR"TrackChunkVertex.glsl",
		SHADERDIR"TrackChunkFragment.glsl"),
	chunkMaterial(&chunkShader),
	splineShader(
		SHADERDIR"TrackSplineVertex.glsl",
		SHADERDIR"TrackSplineFragment.glsl"),
	splineMaterial(&splineShader),
	diffuseTexture(0u)
{
	assert(chunkShader.LinkProgram());
	assert(splineShader.LinkProgram());

	// Temp...
	diffuseTexture = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
}

TrackRenderer::~TrackRenderer()
{
	// Temp...
	glDeleteTextures(1u, &diffuseTexture);

	chunkSceneNode.Disconnect();
	splineSceneNode.Disconnect();
}

void TrackRenderer::Setup()
{
	splineMesh.Generate();
	chunkMesh.Generate(settings.HeightMapVertexCount);

	// Hacky...
	chunkSceneNode.SetMesh(&chunkMesh);
	chunkSceneNode.SetShader(&chunkShader);
	chunkSceneNode.SetMaterial(&chunkMaterial);
	chunkSceneNode.Connect();

	// Hacky...
	splineSceneNode.SetMesh(&splineMesh);
	splineSceneNode.SetShader(&splineShader);
	splineSceneNode.SetMaterial(&splineMaterial);
	splineSceneNode.Connect();
}

void TrackRenderer::RenderChunks() {

	chunkMesh.Bind();

	glUniform2f(
		chunkShader.GetUniformLocation("chunkSize"),
		settings.ChunkWorldSize, settings.ChunkWorldSize);

	glUniform1f(
		chunkShader.GetUniformLocation("terrainBaseHeight"),
		settings.TerrainBaseHeight);

	glUniform1f(
		chunkShader.GetUniformLocation("terrainScale"),
		settings.TerrainScale);


	// Temp...
	glUniform1i(
		chunkShader.GetUniformLocation("diffuseTex"),
		0u);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);


	for (TrackChunks::ChunksT::const_iterator chunk = chunks.GetChunks().begin();
		chunk != chunks.GetChunks().end(); ++chunk) {

		glUniform1i(
			chunkShader.GetUniformLocation("heightTex"),
			1u);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, chunk->TextureID);

		glUniform2f(
			chunkShader.GetUniformLocation("chunkOrigin"),
			chunk->Origin.x, chunk->Origin.z);

		// ...

		chunkMesh.Render();
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0u);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0u);

	chunkMesh.Unbind();
}

void TrackRenderer::RenderSpline()
{
	splineMesh.Bind();

	TrackSpline::SplinePointsT const& splinePoints = spline.GetPoints();

	assert(splinePoints.size() > 4u);
	assert(settings.SplineMeshQuadsPerSection > 1u);

	float subdivision = 1.f / (settings.SplineMeshQuadsPerSection - 1u);

	for (unsigned int pointIndex = 1u; pointIndex != splinePoints.size() - 3u; ++pointIndex)
	{
		for (unsigned int step = 0u; step != settings.SplineMeshQuadsPerSection - 1u; ++step)
		{
			float distance = pointIndex + step * subdivision;
			float nextDistance = pointIndex + (step + 1u) * subdivision;

			Vector3 position1 = spline.GetPositionAtAdjustedDistance(distance);

			glUniform3f(
				splineShader.GetUniformLocation("splinePoint[0]"),
				position1.x, position1.y, position1.z);

			Vector3 position2 = spline.GetPositionAtAdjustedDistance(nextDistance);

			glUniform3f(
				splineShader.GetUniformLocation("splinePoint[1]"),
				position2.x, position2.y, position2.z);

			float width1 = spline.GetTrackHalfWidthAtAdjustedDistance(distance);

			glUniform1f(
				splineShader.GetUniformLocation("splineWidth[0]"),
				width1);

			float width2 = spline.GetTrackHalfWidthAtAdjustedDistance(distance);

			glUniform1f(
				splineShader.GetUniformLocation("splineWidth[1]"),
				width2);

			splineMesh.Render();
		}
	}

	splineMesh.Unbind();
}
