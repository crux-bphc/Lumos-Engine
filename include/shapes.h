#pragma once
#include <cmath>
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "vector"

#include "data.h"

enum PointType {
        Fraction,
        Pixel
};

class Shape {
public:
    glm::vec2 position;
    glm::vec3 color;
    bool is_visible;

    Shape(const glm::vec2& position, const glm::vec3& color = {1.0, 1.0, 1.0}, bool is_visible = true);

    virtual void draw() = 0;

    void hide();

    ~Shape();
};

class Quad : public Shape {
public:
    float height, width;
    Quad(const std::vector<float>& coordinates, const std::vector<float>& color, PointType point_type = PointType::Fraction);
    Quad(const glm::vec2& position, float height, float width, const glm::vec3& color = {1.0, 1.0, 1.0}, PointType point_type = PointType::Fraction);
    void draw() override;
};

class Point : public Shape {
public:
    float size;

    Point(const glm::vec2& position, const glm::vec3& color = {1.0, 1.0, 1.0}, float size = 10.0f, PointType point_type = PointType::Fraction);
    void draw() override;
};