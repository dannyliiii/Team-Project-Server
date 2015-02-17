#include "Framebuffer.h"

Framebuffer::Framebuffer(bool generate) : id(0) {
	if (generate) GenerateBuffer();
}

unsigned int& Framebuffer::GetFramebufferId() {
	return id;
}

void Framebuffer::GenerateBuffer() {
	GInterface::Instance().GenerateFramebuffer(*this);
}

void Framebuffer::Bind() {
	GInterface::Instance().BindFramebuffer(*this);
}

void Framebuffer::Unbind() {
	GInterface::Instance().UnbindFramebuffer();
}

void Framebuffer::SetTexture2D(GI::FBO attachment, Texture& t) {
	Bind();
	GInterface::Instance().SetFramebufferTexture2D(*this, attachment, t);
}
