#pragma once
#include "../nclgl/common.h"
#include "../Dependencies/SOIL/SOIL.h"
#include <string>
#include <iostream>
#include "GInterface.h"
class Texture {
public:
	Texture();
	Texture(std::string file);
	Texture(std::string file[6]);

	unsigned int& GetTextureId();
	GI::Texture::Type GetTextureType();

	void Unbind();
	void Bind();
	void Bind(unsigned int target);

	void SetTextureWrapS(GI::Texture::Wrap func);
	void SetTextureWrapR(GI::Texture::Wrap func);
	void SetTextureWrapT(GI::Texture::Wrap func);
	void SetTextureMag(GI::Texture::Mag func);
	void SetTextureMin(GI::Texture::Min func);

	void TexImage2D(GI::Texture::IFormat internalFormat, GI::Texture::Format format, GI::Texture::Data datatype, unsigned int width, unsigned int height);
	static Texture GenerateTexture2D(unsigned int width, unsigned int height, GI::Texture::IFormat internalFormat, GI::Texture::Format format, GI::Texture::Data datatype);

private:
	
	unsigned int id;
	GI::Texture::Type type;
	std::string binding;

};