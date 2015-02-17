#pragma once
//#include "../Dependencies/GLEW/include/GL/glew.h"
#include "GInterfaceConstants.h"
#include "../nclgl/Shader.h"
/******************************************************************************
Class: GInterface
Implements:InputDevice
Author: Callan White
Description: Class for updating the OpenGL state machine and tracking
changes made to the state machine in order to reduce 
unnecessary state changes. Also provides a level of abstraction
for people working on parts of the project other than the
rendering system but needing to communicate with the hardware.
*******************************************************************************/
class Texture;
class GInterface {
	friend class Framebuffer;
public:
	static GInterface& Instance();

	void ClearBuffers(bool depth, bool colour, bool stencil = false);

	void SetDepthTest(bool b);

	void SetBlendFunction(GI::BlendFunc s, GI::BlendFunc d);
	void SetBlend(bool b);

	void BindTexture(GI::Texture::Type t, GLuint target, GLuint texture);
	void BindTexture(GI::Texture::Type t, GLuint texture);
	void ActivateTextureUnit(GLuint target);
	void Set2DTextureWrapR(GI::Texture::Wrap func);
	void Set2DTextureWrapS(GI::Texture::Wrap func);
	void Set2DTextureWrapT(GI::Texture::Wrap func);
	void Set2DTextureMinFunc(GI::Texture::Min func);
	void Set2DTextureMagFunc(GI::Texture::Mag func);

	void BindTexture(Texture& t, unsigned int target);
	void BindTexture(Texture& t);
	void UnbindTexture();

	void TexImage2D(GI::Texture::IFormat internalFormat, GI::Texture::Format format, GI::Texture::Data datatype, unsigned int width, unsigned int height);
	void GenTexture(Texture& t);
	
	//uniforms
	void SendUniform1f(std::string location, float val, Shader* target);
	void SendUniform1i(std::string location, int val, Shader* target);
	void SendUniform1f(unsigned int location, float val, Shader* target);
	void SendUniform1i(unsigned int location, int val, Shader* target);
	void SendUniform2f(std::string location, Vector2& vals, Shader* target);
	void SendUniform2f(unsigned int location, Vector2& vals, Shader* target);
	void SendUniform3f(std::string location, Vector3& vals, Shader* target);
	void SendUniform3f(unsigned int location, Vector3& vals, Shader* target);
	void SendUniform3fv(std::string location, Vector3& val, Shader* target);
	void SendUniform4fv(std::string location, Vector4& val, Shader* target);
	void SendUniformMatrix4(std::string location, Matrix4& m, Shader* target);
	void SendUniformMatrix4(unsigned int location, Matrix4& m, Shader* target);

	//shader
	void BindShader(Shader* s);
private:
	GInterface() : depthTestEnabled(false) {};
	GInterface(GInterface const&);
	void operator=(GInterface const&);

	GLuint activeTextureUnit;
	GLuint textureUnitBoundTextures[8];

	bool depthTestEnabled;

	GLuint activeFrameBuffer;

	bool blendEnabled;
	GLenum blendSFactor;
	GLenum blendDFactor;

	Shader* currentProgram;

	GLenum polyFace;
	GLenum polyMode;

	void GenerateFramebuffer(Framebuffer& f);
	void BindFramebuffer(Framebuffer& f);
	void SetFramebufferTexture2D(Framebuffer& f, GI::FBO a, Texture& t);
	void UnbindFramebuffer();

};