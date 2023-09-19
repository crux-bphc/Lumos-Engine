#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
   private:
    // Shader program ID
    GLuint ID;
    const char* vertex_path;
    const char* fragment_path;

   public:
    // Constructor: Takes vertex and fragment shader file paths
    Shader(const char* vertex_path, const char* fragment_path) {
        this->vertex_path = vertex_path;
        this->fragment_path = fragment_path;
    }

    // Call this in App startup
    void init() {
        // Read shader source code from files
        std::string vertexCode, fragmentCode;
        std::ifstream vShaderFile, fShaderFile;

        // Ensure ifstream objects can throw exceptions
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            spdlog::debug("Check");

            vShaderFile.open(vertex_path);
            fShaderFile.open(fragment_path);
            std::stringstream vShaderStream, fShaderStream;
            spdlog::debug("Check");

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            spdlog::debug("Check");

            vShaderFile.close();
            fShaderFile.close();
            spdlog::debug("Check");

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            spdlog::debug("Check");
        } catch (std::ifstream::failure& e) {
            spdlog::error("Shader file read error: {}", e.what());
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // Compile and link shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];

        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // Delete the shaders as they're linked into our program now and no longer needed
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // Use the shader
    void use() {
        glUseProgram(ID);
    }

    // Utility function for checking shader compilation/linking errors
    void checkCompileErrors(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[512];

        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                std::cerr << "Shader compilation error of type: " << type << "\n"
                          << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 512, NULL, infoLog);
                std::cerr << "Shader program linking error of type: " << type << "\n"
                          << infoLog << std::endl;
            }
        }
    }

    // Set uniforms
    void set_bool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void set_int(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void set_float(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    GLuint get_id() {
        return ID;
    }
};
