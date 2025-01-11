#include "textures.h"

/**
 * @brief Keeps track of texture slots loaded using push_back().
 * 
 */
std::vector<int> Texture::texUnits = {};
// printf("Compiling...\n");
// printf("Compiled...\n");

/**
 * @brief Constructs a Texture object.
 *
 * This constructor initializes a Texture object with the provided parameters.
 *
 * @param path The path to the texture file.
 * @param format The format of the texture data.
 * @param pixelType The pixel type of the texture data.
 * @param slot The texture slot to bind the texture to.
 * @param rend The renderer object.
 */
Texture::Texture(const char* path, GLenum format, GLenum pixelType,
                 GLuint slot, renderer& rend) {
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

    this->shader = rend.shader;
    u_textures = glGetUniformLocation(shader->ref, "u_textures");

    load(slot);
}

/**
 * @brief Destructor for Texture objects. Deletes the texture.
 *
 */
Texture::~Texture() { glDeleteTextures(1, &ref); }

/**
 * @brief Sets the texture parameter.
 *
 * This function sets the specified texture parameter for the texture.
 *
 * @param pname Name of parameter.
 * @param param Value of parameter.
 */
void Texture::texParameter(GLenum pname, GLint param) {
    glTextureParameteri(ref, pname, param);
}

/**
 * @brief Loads the texture.
 *
 * This function loads the texture into the texture slot passed and updates the shader uniform u_textures in frag.glsl.
 *
 * @param slot The texture slot to bind the texture to.
 */
void Texture::load(GLuint slot) {
    glBindTextureUnit(slot, ref);
    texUnits.push_back(slot);
    this->texIndex = texUnits.size() - 1;
    shader->Activate();
    glUniform1iv(u_textures, Texture::texUnits.size(),
                 Texture::texUnits.data());
}
