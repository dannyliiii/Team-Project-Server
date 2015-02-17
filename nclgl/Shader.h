/******************************************************************************
Class:Shader
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:VERY simple class to encapsulate GLSL shader loading, linking,
and binding. Useful additions to this class would be overloaded functions to
replace the glUniformxx functions in external code, and possibly a map to store
uniform names and their resulting bindings. 

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once


#include "OGLRenderer.h"

#define SHADER_VERTEX   0
#define SHADER_FRAGMENT 1
#define SHADER_GEOMETRY 2
#define SHADER_TESS_C	3
#define SHADER_TESS_E	4

using namespace std;
class Shader	{
public:
	Shader(string vertex, string fragment , string geometry = "");
	Shader(string shaderFile, unsigned int mask);
	~Shader(void);

	GLuint  GetProgram() { return program;}
	bool	LinkProgram();

	int GetUniformLocation(std::string const& name) const;

protected:
	bool	LoadShaderFile(string from, string &into);
	GLuint	GenerateShader(string from, GLenum type);
	bool	CompileShader(GLint shader, string& data);
	void	SetDefaultAttributes();
	void	PrepareShaderHeading(unsigned int type, string& s);

	GLuint objects[5];
	GLuint program;

	bool loadFailed;

	static const GLuint SHADER_TYPES[5];
	static const string SHADER_NAMES[5];
};

