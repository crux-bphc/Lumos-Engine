#pragma once

#include<GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <vector>


void get_file_contents(const char* filename, std::string& file);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ref;
    std::vector<GLuint> uniforms;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();
    void setUniformFloat(GLuint uniformRef, GLfloat data);
    void setUniformTexture(GLuint uniformRef, GLuint texRef);
	glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
};


