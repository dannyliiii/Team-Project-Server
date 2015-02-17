#pragma once

#include "../nclgl/SceneNode.h"

class Skybox : public SceneNode {
public:
	Skybox() : SceneNode(Mesh::GenerateQuad()) {
		shader = new Shader(SHADERDIR"SkyboxVertex.glsl", SHADERDIR"SkyboxFragment.glsl");
		shader->LinkProgram();
		std::string cubefiles[6];
		cubefiles[0] = "rusted_west.jpg"; cubefiles[1] = "rusted_east.jpg"; cubefiles[2] = "rusted_up.jpg";
		cubefiles[3] = "rusted_down.jpg"; cubefiles[4] = "rusted_south.jpg", cubefiles[5] = "rusted_north.jpg";
		Texture* cubemap = new Texture(cubefiles);
		material = new Material(shader, cubemap);
		//cubemap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg", TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}

	void Draw(const OGLRenderer & r) {
		glDepthMask(GL_FALSE);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		glDisable(GL_CULL_FACE);
		mesh->Draw();
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
	}
private:
	GLuint cubemap;
};