#include "GInterface.h"
#include "Framebuffer.h"
#include "Texture.h"
GInterface& GInterface::Instance() {
	static GInterface i;
	return i;
}

void GInterface::ClearBuffers(bool depth, bool colour, bool stencil) {
	unsigned int mask = 0;
	if (depth) mask |= GL_DEPTH_BUFFER_BIT;
	if (colour) mask |= GL_COLOR_BUFFER_BIT;
	if (stencil) mask |= GL_STENCIL_BUFFER_BIT;
	glClear(mask);
}

void GInterface::SetDepthTest(bool b) {
	if (b) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
}

void GInterface::SetBlendFunction(GI::BlendFunc s, GI::BlendFunc d) {
	glBlendFunc((GLenum)s, (GLenum)d);
}

void GInterface::SetBlend(bool b) {
	if (b) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

/******************************************
TEXTURES
******************************************/

void GInterface::BindTexture(GI::Texture::Type t, GLuint target, GLuint texture) {
	glActiveTexture(GL_TEXTURE0 + target);
	glBindTexture((GLenum)t, texture);
}

void GInterface::BindTexture(GI::Texture::Type t, GLuint texture) {
	glBindTexture((GLenum)t, texture);
}

void GInterface::BindTexture(Texture& t) {
	glBindTexture((GLenum)t.GetTextureType(), t.GetTextureId());
}

void GInterface::BindTexture(Texture& t, unsigned int target) {
	glActiveTexture(GL_TEXTURE0 + target);
	BindTexture(t);
}

void GInterface::UnbindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GInterface::ActivateTextureUnit(GLuint target) {
	glActiveTexture(GL_TEXTURE0 + target);
}

void GInterface::Set2DTextureWrapR(GI::Texture::Wrap func) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, (GLint)func);
}

void GInterface::Set2DTextureWrapS(GI::Texture::Wrap func) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)func);
}

void GInterface::Set2DTextureWrapT(GI::Texture::Wrap func) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)func);
}

void GInterface::Set2DTextureMagFunc(GI::Texture::Mag func) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)func);
}

void GInterface::Set2DTextureMinFunc(GI::Texture::Min func) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)func);
}

void GInterface::TexImage2D(GI::Texture::IFormat internalFormat, GI::Texture::Format format, GI::Texture::Data datatype, unsigned int width, unsigned int height) {
	glTexImage2D(GL_TEXTURE_2D, 0, (GLint)internalFormat, width, height, 0, (GLenum)format, (GLenum)datatype, NULL);
}

void GInterface::GenTexture(Texture& t) {
	glGenTextures(1, &t.GetTextureId());
}

/******************************************
SHADER
******************************************/

void GInterface::BindShader(Shader* s) {
	glUseProgram(s->GetProgram());
}

/******************************************
UNIFORMS
******************************************/

void GInterface::SendUniform1i(std::string location, int val, Shader* target) {
	glUniform1i(glGetUniformLocation(target->GetProgram(), location.c_str()), val);
}

void GInterface::SendUniform2f(std::string location, Vector2& val, Shader* target) {
	glUniform2f(glGetUniformLocation(target->GetProgram(), location.c_str()), val.x, val.y);
}

void GInterface::SendUniform1f(std::string location, float val, Shader* target) {
	glUniform1f(glGetUniformLocation(target->GetProgram(), location.c_str()), val);
}

void GInterface::SendUniform3fv(std::string location, Vector3& val, Shader* target) {
	glUniform3fv(glGetUniformLocation(target->GetProgram(), location.c_str()), 1, (float*)&val);
}

void GInterface::SendUniform4fv(std::string location, Vector4& val, Shader* target) {
	glUniform4fv(glGetUniformLocation(target->GetProgram(), location.c_str()), 1, (float*)&val);
}

void GInterface::SendUniformMatrix4(std::string location, Matrix4& val, Shader* target) {
	glUniformMatrix4fv(glGetUniformLocation(target->GetProgram(), location.c_str()), 1, false, (float*)&val);
}

/******************************************
FRAMEBUFFER OBJECTS
******************************************/

void GInterface::BindFramebuffer(Framebuffer& f) {
	glBindFramebuffer(GL_FRAMEBUFFER, f.GetFramebufferId());
}
void GInterface::SetFramebufferTexture2D(Framebuffer& f, GI::FBO attachment, Texture& texture) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachment, (GLenum)texture.GetTextureType(), texture.GetTextureId(), 0);
}

void GInterface::GenerateFramebuffer(Framebuffer& f) {
	glGenFramebuffers(1, &f.GetFramebufferId());
}

void GInterface::UnbindFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}