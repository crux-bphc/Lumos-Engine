#pragma once

#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "vector"
#include "glm/glm.hpp"
#include <stdio.h>
#include "shapes.h"

    

class VBO {
  public:
    GLuint ref;
    VBO();
    ~VBO();
    void bind();
    void unbind();
    void addData(GLfloat *vertices, int size);   
    void updateData(GLfloat *vertices, int size); 

};


class EBO {
  public:
    GLuint ref;
    EBO();
    ~EBO();
    void bind();
    void unbind();
    void addData(GLuint *indices, int size);
};

class VAO {
  public:
    GLuint ref;
    VAO();
    ~VAO();
    void bind();
    void unbind();
    void draw(int count);
    void linkEnableAttrib(VBO* vbo,int ref, int numVals, GLenum type, int stride, void* offset); 
    void attachIndex(EBO* ebo);
};

class Rect {
  public:
    
};

