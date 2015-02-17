#pragma once
#include "GInterface.h"
#include "Texture.h"
class Framebuffer {
private:
	unsigned int id;
public:
	Framebuffer(bool generate = false);

	unsigned int& GetFramebufferId();

	void GenerateBuffer();

	void Bind();
	void Unbind();

	void SetTexture2D(GI::FBO attachment, Texture& t);
};