#include "gl_stuff.h"



/**
 * @brief Construct a new VAO::VAO object
 * 
 */
VAO::VAO() {
    glGenVertexArrays(1, &ref);
}

/**
 * @brief Destroy the VAO::VAO object
 * 
 */
VAO::~VAO() {
    glDeleteVertexArrays(1, &ref);
}

/**
 * @brief 
 * 
 */
void VAO::bind() {
    glBindVertexArray(ref);
}

/**
 * @brief 
 * 
 */
void VAO::unbind() {
    glBindVertexArray(0);
} 

/**
 * @brief 
 * 
 * @param count 
 */
void VAO::draw(int count) {
    this->bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void *)0);
    this->unbind();
}

/**
 * @brief 
 * 
 * @param vbo 
 * @param ref 
 * @param numVals 
 * @param type 
 * @param stride 
 * @param offset 
 */
void VAO::linkEnableAttrib(VBO* vbo, int ref, int numVals, GLenum type, int stride, void* offset) {
    this->bind();
    vbo->bind();
    glVertexAttribPointer(ref, numVals, type, GL_FALSE, stride, offset); 
    glEnableVertexAttribArray(ref);
    vbo->unbind();
    this->unbind();
}

/**
 * @brief 
 * 
 * @param ebo 
 */
void VAO::attachIndex(EBO* ebo) {
    this->bind();
    ebo->bind();
    this->unbind();
    ebo->unbind();
}

/**
 * @brief Construct a new VBO::VBO object
 * 
 */
VBO::VBO() {
    glGenBuffers(1, &ref);
}

/**
 * @brief Destroy the VBO::VBO object
 * 
 */
VBO::~VBO() {
    glDeleteBuffers(1, &ref);
}

/**
 * @brief 
 * 
 */
void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ref);
}

/**
 * @brief 
 * 
 */
void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * @brief 
 * 
 * @param vertices 
 * @param size 
 */
void VBO::addData(GLfloat *vertices, int size) {
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, vertices, GL_DYNAMIC_DRAW);
    this->unbind();
}

/**
 * @brief 
 * 
 * @param vertices 
 * @param size 
 * @param offset 
 */
void VBO::updateData(GLfloat *vertices, int size, int offset) {
    this->bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
    this->unbind();
}

/**
 * @brief Construct a new EBO::EBO object
 * 
 */
EBO::EBO() {
    glGenBuffers(1, &ref);
};

/**
 * @brief Destroy the EBO::EBO object
 * 
 */
EBO::~EBO() {
    glDeleteBuffers(1, &ref);
}

/**
 * @brief 
 * 
 */
void EBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ref);
}

/**
 * @brief 
 * 
 */
void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/**
 * @brief 
 * 
 * @param indices 
 * @param size 
 */
void EBO::addData(GLuint *indices, int size) {
    this->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW);
    this->unbind();
}

/**
 * @brief 
 * 
 * @param indices 
 * @param size 
 * @param offset 
 */
void EBO::updateData(GLuint *indices, int size, int offset) {
    this->bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, indices);
    this->unbind();
}


/**
 * @brief Construct a new renderer::renderer object
 * 
 */
renderer::renderer() { 
    shader = new Shader("./assets/shaders/vert.glsl", "./assets/shaders/frag.glsl");
    vao.attachIndex(&ebo);
    ebo.addData(nullptr, sizeof(indexData) * REND_MAX);
    vbo.addData(nullptr, REND_MAX * sizeof(vertTexQuad));
    vao.attachIndex(&ebo);
    
    vao.linkEnableAttrib(&vbo, 0, 3, GL_FLOAT, sizeof(struct vertTex),
                         (void*)offsetof(vertTex, position));
    vao.linkEnableAttrib(&vbo, 1, 3, GL_FLOAT, sizeof(struct vertTex),
                         (void*)offsetof(vertTex, colour));
    vao.linkEnableAttrib(&vbo, 2, 2, GL_FLOAT, sizeof(struct vertTex),
                         (void*)offsetof(vertTex, texCoord));
    vao.linkEnableAttrib(&vbo, 3, 1, GL_FLOAT, sizeof(struct vertTex),
                         (void*)offsetof(vertTex, texIndex));
    
}

/**
 * @brief 
 * 
 * @param index 
 * @param vData 
 * @param iData 
 */
void renderer::updateData(int index, vertTexQuad* vData, indexData iData) {
    if (vData != nullptr) {
        vbo.updateData((float*)vData, sizeof(vertTexQuad), index*sizeof(vertTexQuad));
    }
    //uint indexData[] = INDEX((uint)index);
    ebo.updateData(iData.indexData, sizeof(indexData), index*sizeof(indexData));
}

/**
 * @brief 
 * 
 */
void renderer::draw() {
    shader->Activate();
    vao.draw(vbo_pos * 6);
}

/**
 * @brief 
 * 
 * @return int 
 */
int renderer::getFree() {
    vbo_pos = vbo_pos + 1;
    return vbo_pos - 1;  
}