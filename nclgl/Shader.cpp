#include "Shader.h"
#include <sstream>
Shader::Shader(string vFile, string fFile, string gFile)	{
	program		= glCreateProgram();
	objects[SHADER_VERTEX]		= GenerateShader(vFile	 ,GL_VERTEX_SHADER);
	objects[SHADER_FRAGMENT]	= GenerateShader(fFile,GL_FRAGMENT_SHADER);
	objects[SHADER_GEOMETRY]	= 0;

	if(!gFile.empty()) {
		objects[SHADER_GEOMETRY]	= GenerateShader(gFile,GL_GEOMETRY_SHADER);
		glAttachShader(program,objects[SHADER_GEOMETRY]);
	}

	glAttachShader(program,objects[SHADER_VERTEX]);
	glAttachShader(program,objects[SHADER_FRAGMENT]);

	SetDefaultAttributes();
}

Shader::~Shader(void)	{
	for(int i = 0; i < 3; ++i) {
		glDetachShader(program, objects[i]);
		glDeleteShader(objects[i]);
	}
	glDeleteProgram(program);
}

bool	Shader::LoadShaderFile(string from, string &into)	{
	ifstream	file;
	string		temp;

	cout << "Loading shader: " << from << endl;

	file.open(from.c_str());
	if(!file.is_open()){
		cout << "File does not exist!" << endl;
		return false;
	}

	while(!file.eof()){
		getline(file,temp);
		into += temp + "\n";
	}
	file.close();

	return true;
}

GLuint	Shader::GenerateShader(string from, GLenum type)	{
	string load;
	if(!LoadShaderFile(from,load)) {
		cout << "failed!" << endl;
		loadFailed = true;
		return 0;
	}

	cout << "Compiling shader... ";

	GLuint shader = glCreateShader(type);

	const char *chars = load.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)	{
		cout << "failed!" << endl;
		char error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		cout << error;
		loadFailed = true;
		return 0;
	}
	cout << "success!" << endl;
	loadFailed = false;
	return shader;
}

bool Shader::LinkProgram()	{
	if(loadFailed) {
		return false;
	}
	glLinkProgram(program); 

	GLint code;
	glGetProgramiv(program, GL_LINK_STATUS, &code);

	if (code == GL_FALSE)	{
		cout << "Failed to link shader: " << endl;
		char error[512];
		glGetInfoLogARB(program, sizeof(error), NULL, error);
		cout << error;
		loadFailed = true;
	}

	return code == GL_TRUE ?  true : false;
}

int Shader::GetUniformLocation(std::string const& name) const
{
	return glGetUniformLocation(program, name.c_str());
}

void	Shader::SetDefaultAttributes()	{
	glBindAttribLocation(program, VERTEX_BUFFER,  "position");
	glBindAttribLocation(program, COLOUR_BUFFER,  "colour");
	glBindAttribLocation(program, NORMAL_BUFFER,  "normal");
	glBindAttribLocation(program, TANGENT_BUFFER, "tangent");
	glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");

	glBindAttribLocation(program, MAX_BUFFER+1,  "transformIndex");
}

/*Such new, so shiny, wow...*/

const GLuint Shader::SHADER_TYPES[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER};
const string Shader::SHADER_NAMES[] = { "Vertex Shader", "Fragment Shader", "Geometry Shader", "Tessellation Control Shader", "Tessellation Evaluation Shader" };

Shader::Shader(string shaderFile, unsigned int mask) {
	std::cout << "####################\nBuilding shader program " << shaderFile << std::endl;
	program = glCreateProgram();
	loadFailed = false;
	/*Read the source*/
	ifstream shaderStream(shaderFile.c_str());
	if (!shaderStream) {
		cout << "Failed to find shader file " << shaderFile << endl;
		return;
	}

	string shaderSource;
	shaderSource.assign((istreambuf_iterator<char>(shaderStream)), istreambuf_iterator<char>());

	for (int i = SHADER_VERTEX; i < SHADER_TESS_E; ++i) {
		if (i != (i & mask)) continue;

		objects[i] = glCreateShader(SHADER_TYPES[i]);

		string shaderstring = shaderSource;

		PrepareShaderHeading(SHADER_TYPES[i], shaderstring);

		std::cout << "Compiling " << SHADER_NAMES[i] << std::endl;
		CompileShader(objects[i], shaderstring) ? glAttachShader(program, objects[i]) : loadFailed = true;
	}
	SetDefaultAttributes();
	std::cout << "####################" << std::endl;
}

void Shader::PrepareShaderHeading(GLuint type, string& s) {
	int pos = s.find("\n");
	switch (type) {
	case GL_VERTEX_SHADER:
		s.insert(pos + 1, "#define _VERT_\n");
		return;
	case GL_FRAGMENT_SHADER:
		s.insert(pos + 1, "#define _FRAG_\n");
		return;
	case GL_GEOMETRY_SHADER:
		s.insert(pos + 1, "#define _GEOM_\n");
		return;
	case GL_TESS_CONTROL_SHADER:
		s.insert(pos + 1, "#define _TESSC_\n");
		return;
	case GL_TESS_EVALUATION_SHADER:
		s.insert(pos + 1, "#define _TESSE_\n");
		return;
	default:
		return;
	}
}

bool Shader::CompileShader(GLint shader, string& data) {
	const char* t = data.c_str();
	glShaderSource(shader, 1, &t, NULL);
	glCompileShader(shader);

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		std::cout << "Compilation error:\n" << strInfoLog << std::endl;
		delete strInfoLog;
		std::cout << data << std::endl;
		return false;
	}
	else {
		std::cout << "Compilation successful" << std::endl;
		return true;
	}
}