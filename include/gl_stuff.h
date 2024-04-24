#pragma once

#include <cmath>
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <iostream>
#include "vector"
#include "glm/glm.hpp"
#include <stdio.h>
#include "shader.h"
#include "entt/entt.hpp"
#include <spdlog/spdlog.h>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

#define REND_MAX 1000


//#define X WINDOW_WIDTH
//#define Y WINDOW_HEIGHT

#define PIXEL(x, n) ((float)x/(float)n)

#define INDEX(i) { 0 + i*4, 2 + i*4, 1 + i*4, 0 + i*4, 3 + i*4, 2 + i*4 }
#define INDEX_ZERO { 0, 0, 0, 0, 0, 0}
   
struct position {
    glm::vec2 pos;  
};


struct dimention {
    glm::vec2 dim;  
};

struct colour {
    glm::vec3 colour;
};

struct rect{
    position pos;
    dimention dim;
    float angle;
    colour col;
};

struct isUpdated{
    bool update;
};

class VBO {
  public:
    GLuint ref;
    VBO();
    ~VBO();
    void bind();
    void unbind();
    void addData(GLfloat *vertices, int size);   
    void updateData(GLfloat *vertices, int size, int offset);
};


class EBO {
  public:
    GLuint ref;
    EBO();
    ~EBO();
    void bind();
    void unbind();
    void addData(GLuint *indices, int size);
    void updateData(GLuint *indices, int size, int offset);
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

// Restriction on number of quads rendered by renderer - 1000

struct vertTex {
    glm::vec3 position;
    glm::vec3 colour;
    glm::vec2 texCoord;
    float texIndex;
};

struct vertTexQuad {
    vertTex vertices[4];
};

struct indexData {
    uint indexData[6];
};


struct textureIndex {
    GLint texIndex;
};

struct isDrawn {
    bool draw;
};


class renderer {
  public:
    std::vector<int> freeStack;
    VAO vao;
    vertTexQuad vbo_data[REND_MAX];
    indexData ebo_data[REND_MAX];
    int vbo_pos = 0;
    VBO vbo;
    //unsigned int ebo_data[6 * REND_MAX];
    EBO ebo;
    Shader* shader;
    renderer();
    void updateData(int index, vertTexQuad* vData, indexData iData);
    void draw();
    void drawQuad(glm::vec2 pos, glm::vec2 dim, glm::vec3 col, GLint texIndex);
    int getFree();
};

struct render {
    renderer* m_renderer;
    int slot;
};


