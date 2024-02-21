#include "gl_stuff.h"

VAO::VAO() {
    glGenVertexArrays(1, &ref);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &ref);
}

void VAO::bind() {
    glBindVertexArray(ref);
}

void VAO::unbind() {
    glBindVertexArray(0);
} 

void VAO::draw(int count) {
    this->bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void *)0);
    this->unbind();
}

void VAO::linkEnableAttrib(VBO* vbo, int ref, int numVals, GLenum type, int stride, void* offset) {
    this->bind();
    vbo->bind();
    glVertexAttribPointer(ref, numVals, type, GL_FALSE, stride, offset); 
    glEnableVertexAttribArray(ref);
    vbo->unbind();
    this->unbind();
}

void VAO::attachIndex(EBO* ebo) {
    this->bind();
    ebo->bind();
    this->unbind();
    ebo->unbind();
}



VBO::VBO() {
    glGenBuffers(1, &ref);
}

VBO::~VBO() {
    glDeleteBuffers(1, &ref);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ref);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::addData(GLfloat *vertices, int size) {
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, vertices, GL_STATIC_DRAW);
    this->unbind();
}

void VBO::updateData(GLfloat *vertices, int size) {
    this->bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)size, vertices);
    this->unbind();
}



EBO::EBO() {
    glGenBuffers(1, &ref);
};

EBO::~EBO() {
    glDeleteBuffers(1, &ref);
}

void EBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ref);
}

void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::addData(GLuint *indices, int size) {
    this->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    this->unbind();
}



