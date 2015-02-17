#pragma once
#include "../nclgl/Shader.h"
#include "Texture.h"
#include "GInterface.h"
class Material {
public:
	Material(Shader* s);
	Material(Shader* s, Texture* t);
	/*UNTESTED (SHOULD* allow variable # of textures)
	edit: this is working, but it's bad and i should feel bad... */
	Material(Shader* s, int num, ...);

	void BindTextures();
	void BindShader();

	Shader* GetShader() const;
private:
	unsigned int numOfTextures;
	Texture* textures;
	Shader* shader;
};
