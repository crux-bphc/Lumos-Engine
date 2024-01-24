#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>

#include "glm/glm.hpp"
#include "vector"
#include "shapes.h"
#include <spdlog/spdlog.h>

struct Vertex {
    glm::vec2 position;
    glm::vec3 color;
};

class BatchRenderer {
   private:
    GLuint shader;
    GLuint vao;
    GLuint vbo;

    std::vector<Vertex> vertices;

   public:
    BatchRenderer();

    // Function to add a shape to the batch
    void add_shape(const Shape& shape);

    // Function to draw the batch
    void draw();
};