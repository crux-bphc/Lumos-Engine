#include "gl_stuff.h"



/**
 * @brief Construct a new VAO::VAO object (vertex array object)
 * 
 */
VAO::VAO() {
    glGenVertexArrays(1, &ref);
}

/**
 * @brief Destroy the VAO::VAO object (vertex array object)
 * 
 */
VAO::~VAO() {
    glDeleteVertexArrays(1, &ref);
}

/**
 * @brief Bind the Vertex Array Object to the current context
 * 
 */
void VAO::bind() {
    glBindVertexArray(ref);
}

/**
 * @brief Unbind the Vertex Array Object from the current context
 * 
 */
void VAO::unbind() {
    glBindVertexArray(0);
} 

/**
 * @brief Draw indices using the GL_TRIANGLES rendering mode
 * 
 * @param count The number of indices to be drawn
 */
void VAO::draw(int count) {
    this->bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void *)0);
    this->unbind();
}

/**
 * @brief Setting up vertex attributes
 * 
 * @param vbo Vertex Buffer Object
 * @param ref Index of the vertex attribute to be modified
 * @param numVals The number of components per vertex attribute
 * @param type Refers to the data type
 * @param stride The byte offset between consecutive vertex attributes
 * @param offset Pointer to the first component of the first vertex attribute in the array
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
 * @brief Binds and unbinds the Element Buffer Object from the current context
 * 
 * @param ebo Refers to the Element Buffer Object
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
 * @brief Bind the Vertex Buffer Object to the current context
 * 
 */
void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ref);
}

/**
 * @brief Unbind the Vertex Buffer Object from the current context
 * 
 */
void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * @brief Adds data to a Vertex Buffer Object
 * 
 * @param vertices Points to the data to be copied into the buffer
 * @param size Specifies the size of the data to be stored in the buffer
 */
void VBO::addData(GLfloat *vertices, int size) {
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, vertices, GL_DYNAMIC_DRAW);
    this->unbind();
}

/**
 * @brief Updates data in a given buffer object
 * 
 * @param vertices Pointer to the new data
 * @param size Size of the data to be replaced
 * @param offset Offset in the buffers data store where the replacement of data will begin
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
 * @brief Bind the Element Buffer Object to the current context
 * 
 */
void EBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ref);
}

/**
 * @brief Unbind the Element Buffer Object from the current context
 * 
 */
void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/**
 * @brief Adds data to an Element Buffer Object
 * 
 * @param indices Pointer to the data to be copied into the buffer
 * @param size Specifies the size of the data to be added
 */
void EBO::addData(GLuint *indices, int size) {
    this->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW);
    this->unbind();
}

/**
 * @brief 
 * 
 * @param indices Pointer to the new data
 * @param size Specifies the size of the data to be replaced
 * @param offset Offset in the buffers data store where the replacement of data will begin
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
 * @brief Update data in the Vertex Buffer Object and the Element Buffer Object
 * 
 * @param index Pointer to the new data
 * @param vData Pointer to vertex data
 * @param iData Object containing index data
 */
void renderer::updateData(int index, vertTexQuad* vData, indexData iData) {
    if (vData != nullptr) {
        vbo.updateData((float*)vData, sizeof(vertTexQuad), index*sizeof(vertTexQuad));
    }
    //uint indexData[] = INDEX((uint)index);
    ebo.updateData(iData.indexData, sizeof(indexData), index*sizeof(indexData));
}

/**
 * @brief Activates the shader and calls the draw function
 * 
 */
void renderer::draw() {
    shader->Activate();
    vao.draw(vbo_pos * 6);
}

/**
 * @brief increments vbo_pos and returns the old value of vbo_pos
 * 
 * @return int 
 */
int renderer::getFree() {
    vbo_pos = vbo_pos + 1;
    return vbo_pos - 1;  
}