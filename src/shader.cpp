#include "shader.h"

// Reads a text file and outputs a string with everything in the text file
void get_file_contents(const char* filename, std::string& file)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		file = contents;
        return;
	}
    throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    //printf("getting files\n");
    std::string vertCode;
    std::string fragCode;
	get_file_contents(vertexFile, vertCode); 
	get_file_contents(fragmentFile, fragCode);


	const char* vertSource = vertCode.c_str();
	const char* fragSource = fragCode.c_str();

    
    GLint isCompiled = 0;
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSource, NULL);
	glCompileShader(vertShader);

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE) {
        printf("Unable to compile Vertex Shader\n");
    }

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);
    
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE) {
        printf("Unable to compile Fragment Shader\n");
    }


	ref = glCreateProgram();
	glAttachShader(ref, vertShader);
	glAttachShader(ref, fragShader);
	glLinkProgram(ref);


    GLint isLinked = 0;
    glGetProgramiv(ref, GL_LINK_STATUS, (int *)&isLinked);
    if(isLinked == GL_FALSE) {
        printf("Unable to link Shaders\n");
    }

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

}

void Shader::Activate()
{
	glUseProgram(ref);

}

void Shader::Delete()
{
	glDeleteProgram(ref);
}


void Shader::setUniformFloat(GLuint uniformRef, GLfloat data) {
    glUseProgram(ref);
    glUniform1f(uniformRef, data);

}

void Shader::setUniformTexture(GLuint uniformRef, GLuint texRef) {
    glUseProgram(ref);
    glUniform1i(uniformRef, texRef); 
}


