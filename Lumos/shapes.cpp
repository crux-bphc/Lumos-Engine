#pragma once
// #include "freeglut-3.4.0/include/GL/glut.h"
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

#include "data.cpp"

enum PointType {
    Fraction,
    Pixel
};

class Shape {
   protected:
    GLuint shader_program_id;

   public:
    glm::vec2 position;
    glm::vec3 color;
    bool is_visible;

    Shape(const glm::vec2& position, const glm::vec3& color = {1.0, 1.0, 1.0}, bool is_visible = true)
        : position(position), color(color), is_visible(is_visible) {}

    virtual void draw() = 0;

    void hide() {
        this->is_visible = false;
    }

    virtual ~Shape() {}

    void set_shader_program(Shader shader) {
        shader_program_id = shader.get_id();
    }
};

// We are assuming the vector point are given in such a fashion such that
// first two point are lower left points and they rest are present in a clockwise manner
// the position are in pixels
class Quad : public Shape {
   public:
    float height, width;

    Quad(const std::vector<float>& coordinates, const std::vector<float>& color, PointType point_type = PointType::Fraction)
        : Shape({coordinates[0], coordinates[1]}, {color[0], color[1], color[2]}) {
        float min_x = std::min(std::min(coordinates[0], coordinates[2]), std::min(coordinates[4], coordinates[6]));
        float min_y = std::min(std::min(coordinates[1], coordinates[3]), std::min(coordinates[5], coordinates[7]));
        float max_x = std::max(std::max(coordinates[0], coordinates[2]), std::max(coordinates[4], coordinates[6]));
        float max_y = std::max(std::max(coordinates[1], coordinates[3]), std::max(coordinates[5], coordinates[7]));

        switch (point_type) {
            case PointType::Pixel: {
                this->position.x = (this->position.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
                this->position.y = (this->position.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;
                this->width = (max_x - min_x) / static_cast<float>(WINDOW_WIDTH) * 2.0f - 1.0f;
                this->height = (max_y - min_y) / static_cast<float>(WINDOW_HEIGHT) * 2.0f - 1.0f;
                break;
            }
            case PointType::Fraction: {
                this->width = max_x - min_x;
                this->height = max_y - min_y;

                this->position = glm::vec2{min_x, min_y};
                break;
            }
            default:
                break;
        }

        this->color = glm::vec3{color[0], color[1], color[2]};
    }

    Quad(const glm::vec2& position, float height, float width, const glm::vec3& color = {1.0, 1.0, 1.0}, PointType point_type = PointType::Fraction)
        : Shape(position, color) {
        switch (point_type) {
            case PointType::Pixel: {
                this->position.x = (this->position.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
                this->position.y = (this->position.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;
                this->width = static_cast<float>(width) / static_cast<float>(WINDOW_WIDTH) * 2.0f;
                this->height = static_cast<float>(height) / static_cast<float>(WINDOW_HEIGHT) * 2.0f;
                break;
            }
            case PointType::Fraction: {
                this->width = width;
                this->height = height;
                break;
            }

            default:
                break;
        }
    }

    void draw() override {
        if (!this->is_visible) {
            return;
        }

        glBegin(GL_QUADS);
        glColor3f(color.r, color.g, color.b);  // Set the color
        glVertex2f(position.x, position.y);
        glVertex2f(position.x + width, position.y);
        glVertex2f(position.x + width, position.y + height);
        glVertex2f(position.x, position.y + height);
        glEnd();

        glFlush();
    }
};

// It's in the shape of a square though (idk why)
class Point : public Shape {
   public:
    float size;

    Point(const glm::vec2& position, const glm::vec3& color = {1.0, 1.0, 1.0}, float size = 10.0f, PointType point_type = PointType::Fraction)
        : Shape(position, color) {
        switch (point_type) {
            case PointType::Pixel: {
                this->position.x = (this->position.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
                this->position.y = (this->position.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;
                break;
            }
            default:
                break;
        }
        this->size = size;
    }

    // Point(const std::vector<float>& coordinates, const std::vector<float>& color, PointType point_type = PointType::Fraction)
    //     : Shape({coordinates[0], coordinates[1]}, {color[0], color[1], color[2]}) {
    // }

    void draw() override {
        if (!this->is_visible) {
            return;
        }

        glPointSize(this->size);
        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex2f(position.x, position.y);
        glEnd();

        glFlush();
    }
};

class Circle : public Shape {
   public:
    float radius;
    bool shaded;

    Circle(const glm::vec2& position, const glm::vec3& color = {1.0, 1.0, 1.0}, float radius = 1.0f, bool shaded = true, PointType point_type = PointType::Fraction)
        : Shape(position, color), radius(radius), shaded(shaded) {
        switch (point_type) {
            case PointType::Pixel: {
                this->position.x = (this->position.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
                this->position.y = (this->position.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;
                break;
            }
            default:
                break;
        }
    }

    Circle(const std::vector<float>& coordinates, const std::vector<float>& color, float radius = 1.0f, bool shaded = true, PointType point_type = PointType::Fraction)
        : Shape({coordinates[0], coordinates[1]}, {color[0], color[1], color[2]}), radius(radius), shaded(shaded) {
        switch (point_type) {
            case PointType::Pixel: {
                this->position.x = (coordinates[0] / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
                this->position.y = (coordinates[1] / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;
                break;
            }
            default:
                break;
        }
    }

    void draw() override {
        if (!is_visible) {
            return;
        }

        int numSegments = 100;  // Adjust the number of segments for smoother or coarser circles
        if (this->shaded) {
            glBegin(GL_TRIANGLE_FAN);  // Use GL_TRIANGLE_FAN to draw a filled circle

            glColor3f(color.r, color.g, color.b);

            // Center of the circle
            glVertex2f(this->position.x, this->position.y);

            for (int i = 0; i <= numSegments; i++) {
                float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numSegments);
                float x = radius * cos(theta) / static_cast<float>(WINDOW_WIDTH) * 10.0f;
                // float x = (radius * cos(theta) / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
                float y = radius * sin(theta) / static_cast<float>(WINDOW_HEIGHT) * 10.0f;
                // float y = (radius * sin(theta) / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;

                glVertex2f(this->position.x + x, this->position.y + y);
            }

            glEnd();
            glFlush();
        } else {
            glBegin(GL_LINE_LOOP);  // Use GL_LINE_LOOP to draw the circle as an outline

            glColor3f(color.r, color.g, color.b);

            float corrected_radius = (2.0f * this->radius) / static_cast<float>(WINDOW_WIDTH);

            for (int i = 0; i < numSegments; i++) {
                float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numSegments);
                float x = corrected_radius * cos(theta);
                float y = corrected_radius * sin(theta);

                glVertex2f(position.x + x, position.y + y);
            }

            glEnd();
            glFlush();
        }
    }
};

class Line2D : public Shape {
   public:
    std::vector<glm::vec2> points;

    Line2D(std::vector<glm::vec2>& points, const glm::vec3& color = {1.0, 1.0, 1.0}, PointType point_type = PointType::Fraction)
        : Shape(glm::vec2{0.0f, 0.0f}, color), points(points) {
        switch (point_type) {
            case PointType::Pixel: {
                for (glm::vec2& point : points) {
                    point.x = (point.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
                    point.y = (point.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;
                }
                break;
            }
            default:
                break;
        }
    }

    void draw() override {
        if (!is_visible || points.size() < 2) {
            return;
        }

        glBegin(GL_LINES);
        glColor3f(color.r, color.g, color.b);

        for (size_t i = 0; i < points.size() - 1; ++i) {
            glVertex2f(points[i].x, points[i].y);
            glVertex2f(points[i + 1].x, points[i + 1].y);
        }

        glEnd();
        glFlush();
    }
};

class Triangle : public Shape {
   public:
    glm::vec2 p1, p2, p3;

    Triangle(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3, const glm::vec3& color = {1.0, 1.0, 1.0}, PointType point_type = PointType::Fraction)
        : Shape(glm::vec2{0.0f, 0.0f}, color) {
        p1 = point1;
        p2 = point2;
        p3 = point3;

        if (point_type == PointType::Pixel) {
            p1.x = (p1.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
            p1.y = (p1.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;

            p2.x = (p2.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
            p2.y = (p2.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;

            p3.x = (p3.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
            p3.y = (p3.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;
        }
    }

    void draw() override {
        if (!is_visible) {
            return;
        }

        glBegin(GL_TRIANGLES);
        glColor3f(color.r, color.g, color.b);

        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
        glVertex2f(p3.x, p3.y);

        glEnd();
        glFlush();
    }
};
