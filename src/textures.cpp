#include "textures.h"


std::vector<int> Texture::texUnits = {};
// printf("Compiling...\n");
Shader* Texture::shader = nullptr;
// printf("Compiled...\n");

Texture::Texture(const char* path, GLenum format, GLenum pixelType, rect rect,
                 GLuint slot, Camera* camera) {
    stbi_set_flip_vertically_on_load(true);

    unsigned char* bytes = stbi_load(path, &width, &height, &numColCh, 0);

    if (bytes == NULL) {
        spdlog::error("Unable to open texture {}", path);
        exit(EXIT_FAILURE);
    }

    // Texture assumed to be GL_TEXTURE_2D
    glCreateTextures(GL_TEXTURE_2D, 1, &ref);

    // Image Internally stored as GL_RGBA8
    glTextureStorage2D(ref, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(ref, 0, 0, 0, width, height, format, pixelType, bytes);
    stbi_image_free(bytes);

    glGenerateTextureMipmap(ref);

    texParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    texParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    texParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

    load(slot);

    /*struct vertTex vertices[4];

    vertices[0].position = {rect.pos.x, rect.pos.y, 0.0f};
    vertices[1].position = {rect.pos.x + rect.dim.x, rect.pos.y, 0.0f};
    vertices[2].position = {rect.pos.x + rect.dim.x, rect.pos.y + rect.dim.y,
    0.0f}; vertices[3].position = {rect.pos.x, rect.pos.y + rect.dim.y, 0.0f};

    vertices[0].texCoord = {0.0f, 0.0f};
    vertices[1].texCoord = {1.0f, 0.0f};
    vertices[2].texCoord = {1.0f, 1.0f};
    vertices[3].texCoord = {0.0f, 1.0f};

    for(int i = 0; i < 4; i++) {
        vertices[i].colour = {0.0f, 0.0f, 1.0f};
    }

    for(int i = 0; i < 4; i++) {
        vertices[i].texIndex = static_cast<float>(texIndex);
    }

    for(int i = 0; i < 4; i++) {
        vertices[i].scale = 0.5f;
    }*/

    GLuint indices[] = {
        0, 2, 1,  // Upper triangle
        0, 3, 2,  // Lower triangle
    };

    vbo.addData(nullptr, 4 * sizeof(vertTex));
    updateRect(&rect, *camera);

    ebo.addData(indices, sizeof(indices));

    vao.attachIndex(&ebo);

    vao.linkEnableAttrib(&vbo, 0, 3, GL_FLOAT, sizeof(struct vertTex),
                         (void*)offsetof(vertTex, position));
    vao.linkEnableAttrib(&vbo, 1, 3, GL_FLOAT, sizeof(struct vertTex),
                         (void*)offsetof(vertTex, colour));
    vao.linkEnableAttrib(&vbo, 2, 2, GL_FLOAT, sizeof(struct vertTex),
                         (void*)offsetof(vertTex, texCoord));
    vao.linkEnableAttrib(&vbo, 3, 1, GL_FLOAT, sizeof(struct vertTex),
                         (void*)offsetof(vertTex, texIndex));
    vao.linkEnableAttrib(&vbo, 4, 1, GL_FLOAT, sizeof(struct vertTex),
                         (void*)offsetof(vertTex, scale));

    u_textures = glGetUniformLocation(shader->ref, "u_textures");
}

Texture::~Texture() { glDeleteTextures(1, &ref); }

void Texture::texParameter(GLenum pname, GLint param) {
    glTextureParameteri(ref, pname, param);
}

void Texture::load(GLuint slot) {
    glBindTextureUnit(slot, ref);
    texUnits.push_back(slot);
    this->texIndex = texUnits.size() - 1;
    glUniform1iv(u_textures, Texture::texUnits.size(),
                 Texture::texUnits.data());
}

void Texture::draw() {
    //shader->Activate();

    vao.draw(6);
}

void Texture::initShader(Camera& camera) {
    shader = new Shader("../assets/shaders/vert.glsl",
                        "../assets/shaders/frag.glsl");
    //camera.ProjectionLoc = glGetUniformLocation(shader->ref, "projection"); 
    //camera.modelLoc = glGetUniformLocation(shader->ref, "model");
    //camera.viewLoc = glGetUniformLocation(shader->ref, "view");
    //printf("%i %i %i", camera.ProjectionLoc, camera.modelLoc, camera.viewLoc);
}

//Constructor overload: No camera
void Texture::initShader(){
    shader = new Shader("../assets/shaders/vert.glsl",
                        "../assets/shaders/frag.glsl");
    shader->Activate();
    
}

void Texture::updateRect(rect* rect, Camera& camera) {
    struct vertTex vertices[4]; 

    if (camera.direction == UP) {
        rect->pos.y += 0.001f;
        printf("UP");
    }
    if (camera.direction == DOWN) {
        rect->pos.y -= 0.001f;
        printf("down");
    }
    if (camera.direction == LEFT) {
        rect->pos.x -= 0.001f;
        printf("left");
    }
    if (camera.direction == RIGHT) {
        rect->pos.x += 0.001f;
        printf("right");
    }


    vertices[0].position = {rect->pos.x, rect->pos.y, 0.0f};
    vertices[1].position = {rect->pos.x + rect->dim.x, rect->pos.y, 0.0f};
    vertices[2].position = {rect->pos.x + rect->dim.x, rect->pos.y + rect->dim.y,
                            0.0f};
    vertices[3].position = {rect->pos.x, rect->pos.y + rect->dim.y, 0.0f};

    vertices[0].texCoord = {0.0f, 0.0f};
    vertices[1].texCoord = {1.0f, 0.0f};
    vertices[2].texCoord = {1.0f, 1.0f};
    vertices[3].texCoord = {0.0f, 1.0f};



    /*
    
    vertices[0].position = {rect.pos.x, rect.pos.y, 0.0f};
    vertices[1].position = {rect.pos.x + rect.dim.x, rect.pos.y, 0.0f};
    vertices[2].position = {rect.pos.x + rect.dim.x, rect.pos.y + rect.dim.y,
                            0.0f};
    vertices[3].position = {rect.pos.x, rect.pos.y + rect.dim.y, 0.0f};

    glm::vec2 position = {rect.pos.x, rect.pos.y};
    glm::vec2 scale =  {rect.dim.x, rect.dim.y};
    float rot = 0;
    
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    glm::mat4 scal = glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
    glm::mat4 rotat = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 model = scal * rot * trans;
    glUniformMatrix4fv(camera.modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    GLuint a = glGetUniformBlockIndex(shader->ref, "projection");
    //printf("%d\n", a);*/

    for (int i = 0; i < 4; i++) {
        vertices[i].colour = {0.0f, 0.0f, 1.0f};
    }

    for (int i = 0; i < 4; i++) {
        vertices[i].texIndex = static_cast<float>(texIndex);
    }

    for (int i = 0; i < 4; i++) {
        vertices[i].scale = 0.5f;
    }
    vbo.updateData((float*)vertices, sizeof(vertices));
}
