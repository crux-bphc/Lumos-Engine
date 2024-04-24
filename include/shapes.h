/**
 * @file shapes.h
  * @brief  Declares various 2D shape classes (Quad, Point, Circle, Line2D, Triangle) with methods for drawing and manipulation in the Lumos engine.
 */

#pragma once

#include <cmath>
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "vector"
#include <spdlog/spdlog.h>

#include "data.h"

/**
 * @enum PointType
 * Enumerates the types of coordinate systems for shapes (Fraction or Pixel).
 */
enum PointType {
    Fraction,   ///< Fractional coordinate system.
    Pixel       ///< Pixel coordinate system.
};

/**
 * @class Shape
 * @brief Base class representing a generic shape.
 *
 * The Shape class defines the common properties and methods for all shapes in the Lumos engine.
 */
class Shape {
public:
    glm::vec2 position;     ///< Position of the shape.
    glm::vec3 color;        ///< Color of the shape.
    bool is_visible;        ///< Flag indicating whether the shape is visible.

    /**
     * @brief Constructor for the Shape class.
     * @param position Initial position of the shape.
     * @param color Initial color of the shape (default is white).
     * @param is_visible Flag indicating whether the shape is initially visible (default is true).
     */
    Shape(const glm::vec2& position, const glm::vec3& color = {1.0, 1.0, 1.0}, bool is_visible = true);

    /**
     * @brief Virtual method to draw the shape.
     */
    virtual void draw() = 0;

    /**
     * @brief Hides the shape, making it not visible.
     */
    void hide();

    /**
     * @brief Destructor for the Shape class.
     */
    virtual ~Shape();
};

/**
 * @class Quad
 * @brief Class representing a 2D rectangle (Quad) in the Lumos engine.
 */
class Quad : public Shape {
public:
    float height;   ///< Height of the quad.
    float width;    ///< Width of the quad.

    /**
     * @brief Constructor for the Quad class using given coordinates.
     * @param coordinates Coordinates of the quad's corners.
     * @param color Color of the quad.
     * @param point_type Type of coordinates (Fraction or Pixel).
     */
    Quad(const std::vector<float>& coordinates, const std::vector<float>& color, PointType point_type = PointType::Fraction);

    /**
     * @brief Constructor for the Quad class using explicit position, height, width, and color.
     * @param position Position of the quad.
     * @param height Height of the quad.
     * @param width Width of the quad.
     * @param color Color of the quad.
     * @param point_type Type of coordinates (Fraction or Pixel).
     */
    Quad(const glm::vec2& position, float height, float width, const glm::vec3& color = {1.0, 1.0, 1.0}, PointType point_type = PointType::Fraction);

    /**
     * @brief Method to draw the quad.
     */
    void draw() override;
};

/**
 * @class Point
 * @brief Class representing a 2D point in the Lumos engine.
 */
class Point : public Shape {
public:
    float size; ///< Size of the point.

    /**
     * @brief Constructor for the Point class.
     * @param position Position of the point.
     * @param color Color of the point.
     * @param size Size of the point (default is 10.0f).
     * @param point_type Type of coordinates (Fraction or Pixel).
     */
    Point(const glm::vec2& position, const glm::vec3& color = {1.0, 1.0, 1.0}, float size = 10.0f, PointType point_type = PointType::Fraction);

    /**
     * @brief Method to draw the point.
     */
    void draw() override;
};

/**
 * @class Circle
 * @brief Class representing a 2D circle in the Lumos engine.
 */
class Circle : public Shape {
public:
    float radius;   ///< Radius of the circle.
    bool shaded;    ///< Flag indicating whether the circle should be shaded.

    /**
     * @brief Constructor for the Circle class using explicit position, color, radius, and shading.
     * @param position Position of the circle.
     * @param color Color of the circle.
     * @param radius Radius of the circle (default is 1.0f).
     * @param shaded Flag indicating whether the circle should be shaded (default is true).
     * @param point_type Type of coordinates (Fraction or Pixel).
     */
    Circle(const glm::vec2& position, const glm::vec3& color = {1.0, 1.0, 1.0}, float radius = 1.0f, bool shaded = true, PointType point_type = PointType::Fraction);

    /**
     * @brief Constructor for the Circle class using coordinates, color, radius, and shading.
     * @param coordinates Coordinates of the circle's center.
     * @param color Color of the circle.
     * @param radius Radius of the circle (default is 1.0f).
     * @param shaded Flag indicating whether the circle should be shaded (default is true).
     * @param point_type Type of coordinates (Fraction or Pixel).
     */
    Circle(const std::vector<float>& coordinates, const std::vector<float>& color, float radius = 1.0f, bool shaded = true, PointType point_type = PointType::Fraction);

    /**
     * @brief Method to draw the circle.
     */
    void draw() override;
};

/**
 * @class Line2D
 * @brief Class representing a 2D line in the Lumos engine.
 */
class Line2D : public Shape {
public:
    std::vector<glm::vec2> points;  ///< List of points forming the line.

    /**
     * @brief Constructor for the Line2D class.
     * @param points List of points forming the line.
     * @param color Color of the line.
     * @param point_type Type of coordinates (Fraction or Pixel).
     */
    Line2D(std::vector<glm::vec2>& points, const glm::vec3& color = {1.0, 1.0, 1.0}, PointType point_type = PointType::Fraction);

    /**
     * @brief Method to draw the line.
     */
    void draw() override;
};

/**
 * @class Triangle
 * @brief Class representing a 2D triangle in the Lumos engine.
 */
class Triangle : public Shape {
public:
    glm::vec2 p1, p2, p3;   ///< Vertices of the triangle.

    /**
     * @brief Constructor for the Triangle class.
     * @param point1 First vertex of the triangle.
     * @param point2 Second vertex of the triangle.
     * @param point3 Third vertex of the triangle.
     * @param color Color of the triangle.
     * @param point_type Type of coordinates (Fraction or Pixel).
     */
    Triangle(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3, const glm::vec3& color = {1.0, 1.0, 1.0}, PointType point_type = PointType::Fraction);

    /**
     * @brief Method to draw the triangle.
     */
    void draw() override;
};
