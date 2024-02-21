#pragma once
#include "gl_stuff.h"
#include "shader.h"
#include "stb/stb_image.h"
#include <stdio.h>
#include <spdlog/spdlog.h>
#include "camera.h"

struct vertTex {
    glm::vec3 position;
    glm::vec3 colour;
    glm::vec2 texCoord;
    float texIndex;
    float scale;
};

struct rect {
    glm::vec2 pos;
    glm::vec2 dim;
};


class Texture {
    GLuint texIndex;
    VAO vao;
    VBO vbo;
    EBO ebo;
    static Shader* shader;
    GLuint u_textures;
  public:
    GLuint ref;
    static GLuint uniform;
    int width, height, numColCh;
    Texture(const char* path, GLenum format, GLenum pixelType, rect rect, GLuint slot);
    ~Texture();
    void texParameter(GLenum pname, GLint param);
    void load(GLuint slot);
    void unload();
    static std::vector<int> texUnits;
    static void initShader();
    void draw();
    void updateRect(rect rect);
};




