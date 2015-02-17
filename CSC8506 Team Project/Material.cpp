#include "Material.h"

Material::Material(Shader* s) {
	numOfTextures = 1;
	shader = s;
	textures = new Texture[1];
	textures[0] = Texture(TEXTUREDIR"Barren Reds.jpg"); //temp default
}

Material::Material(Shader* s, Texture* t) {
	numOfTextures = 1;
	shader = s;
	textures = new Texture[1];
	textures[0] = *t;
}

Material::Material(Shader* s, int num, ...) {
	va_list args;
	va_start(args, num);
	textures = new Texture[num];
	for (int i = 0; i < num; ++i) {
		textures[i] = *va_arg(args, Texture*);
	}
	va_end(args);
	shader = s;
	numOfTextures = num;
}

void Material::BindTextures() {
	for (unsigned int i = 0; i < numOfTextures; ++i) textures[i].Bind(i);
}

void Material::BindShader() {
	GInterface::Instance().BindShader(shader);
}

Shader* Material::GetShader() const {
	return shader;
}