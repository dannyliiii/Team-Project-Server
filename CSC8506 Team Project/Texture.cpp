#include "Texture.h"

Texture::Texture() : id(0) {

}

Texture::Texture(std::string filepath) {
	id = SOIL_load_OGL_texture((TEXTUREDIR + filepath).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (id == 0) {
		std::cout << "Failed to load " << filepath << std::endl;
		return;
	}
	type = GI::Texture::Type::TEXTURE_2D;

	SetTextureWrapS(GI::Texture::Wrap::REPEAT);
	SetTextureWrapT(GI::Texture::Wrap::REPEAT);
	SetTextureMin(GI::Texture::Min::LINEAR);
	SetTextureMag(GI::Texture::Mag::LINEAR);
	std::cout << "Successfully loaded " << filepath << std::endl;
}

Texture::Texture(std::string filepaths[6]) {
	for (int i = 0; i < 6; ++i) filepaths[i] = TEXTUREDIR + filepaths[i];
	id = SOIL_load_OGL_cubemap(filepaths[0].c_str(), filepaths[1].c_str(), filepaths[2].c_str(), filepaths[3].c_str(), filepaths[4].c_str(), filepaths[5].c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	if (id == 0) {
		std::cout << "Failed to load cubemap " << filepaths[0] << std::endl;
		return;
	}
}

Texture Texture::GenerateTexture2D(unsigned int width, unsigned int height, GI::Texture::IFormat internalFormat, GI::Texture::Format format, GI::Texture::Data datatype) {
	Texture ret = Texture();
	ret.type = GI::Texture::Type::TEXTURE_2D;
	GInterface::Instance().BindTexture(ret);
	GInterface::Instance().GenTexture(ret);
	ret.SetTextureWrapS(GI::Texture::Wrap::CLAMP_TO_EDGE);
	ret.SetTextureWrapT(GI::Texture::Wrap::CLAMP_TO_EDGE);
	ret.SetTextureMag(GI::Texture::Mag::NEAREST);
	ret.SetTextureMin(GI::Texture::Min::NEAREST);
	ret.TexImage2D(internalFormat, format, datatype, width, height);
	ret.Unbind();
	return ret;
}

unsigned int& Texture::GetTextureId() {
	return id;
}

GI::Texture::Type Texture::GetTextureType() {
	return type;
}

void Texture::Bind() {
	GInterface::Instance().BindTexture(*this);
}

void Texture::Bind(unsigned int target) {
	GInterface::Instance().BindTexture(*this, target);
}

void Texture::Unbind() {
	GInterface::Instance().UnbindTexture();
}

void Texture::SetTextureWrapS(GI::Texture::Wrap func) { 
	Bind(); 
	GInterface::Instance().Set2DTextureWrapS(func);
}

void Texture::SetTextureWrapR(GI::Texture::Wrap func) { 
	Bind(); 
	GInterface::Instance().Set2DTextureWrapR(func); 
}

void Texture::SetTextureWrapT(GI::Texture::Wrap func) { 
	Bind(); 
	GInterface::Instance().Set2DTextureWrapT(func); 
}

void Texture::SetTextureMag(GI::Texture::Mag func) { 
	Bind(); 
	GInterface::Instance().Set2DTextureMagFunc(func); 
}

void Texture::SetTextureMin(GI::Texture::Min func) { 
	Bind(); 
	GInterface::Instance().Set2DTextureMinFunc(func); 
}

void Texture::TexImage2D(GI::Texture::IFormat internalFormat, GI::Texture::Format format, GI::Texture::Data datatype, unsigned int width, unsigned int height) { 
	Bind(); 
	GInterface::Instance().TexImage2D(internalFormat, format, datatype, width, height); 
}