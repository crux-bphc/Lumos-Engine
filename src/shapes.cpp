#include "shapes.h"

/**
 * @brief Constructor for the Shape class.
 * 
 * @param position Coordinates of shape in pixels (default is (0, 0))
 * @param color Color of the shape in RGB format (default is white).
 * @param is_visible Visibility of the shape (default is true).
 */
Shape::Shape(const glm::vec2& position, const glm::vec3& color, bool is_visible)
    : position(position), color(color), is_visible(is_visible) {}

/**
 * @brief Hides the shape by changing the is_visible parameter.
 */
void Shape::hide() {
    this->is_visible = false;
}

/**
 * @brief Destructor for the Shape class.
 */
Shape::~Shape() {}

/**
 * @brief Constructor for the Quad class.
 * 
 * @param coordinates Coordinates of the shape in pixels (default is (0, 0)).
 * @param color Color of the shape in RGB format (default is white).
 * @param point_type Type of coordinates (default is PointType::Fraction of the window) Can also use PointType::Pixel.
 */
Quad::Quad(const std::vector<float>& coordinates, const std::vector<float>& color, PointType point_type)
    : Shape({coordinates[0], coordinates[1]}, {color[0], color[1], color[2]}) {
    // Calculate the minimum and maximum coordinates
    float min_x = std::min(std::min(coordinates[0], coordinates[2]), std::min(coordinates[4], coordinates[6]));
    float min_y = std::min(std::min(coordinates[1], coordinates[3]), std::min(coordinates[5], coordinates[7]));
    float max_x = std::max(std::max(coordinates[0], coordinates[2]), std::max(coordinates[4], coordinates[6]));
    float max_y = std::max(std::max(coordinates[1], coordinates[3]), std::max(coordinates[5], coordinates[7]));

    // Adjust the position and size based on the point type
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

/**
 * @brief Constructor for the Quad class.
 * 
 * @param position Position of the shape in 2D space.
 * @param height Height of the shape.
 * @param width Width of the shape.
 * @param color Color of the shape in RGB format.
 * @param point_type Type of coordinates (default is PointType::Fraction of the window).
 */
Quad::Quad(const glm::vec2& position, float height, float width, const glm::vec3& color, PointType point_type)
    : Shape(position, color) {
    // Adjust the position and size based on the point type
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

/**
 * @brief Draws the Quad object.
 * 
 * This function uses OpenGL to draw the Quad object on the screen.
 */
void Quad::draw() {
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

/**
 * @brief Constructor for the Point class.
 * 
 * @param position Position of the point in 2D space.
 * @param color Color of the point in RGB format.
 * @param size Size of the point.
 * @param point_type Type of coordinates (default is PointType::Fraction of the window).
 */
Point::Point(const glm::vec2& position, const glm::vec3& color, float size, PointType point_type) 
    : Shape(position, color) { //It's in the shape of a square though (idk why)
    // Adjust the position based on the point type
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

/**
 * @brief Draws the Point object.
 * 
 * This function uses OpenGL to draw the Point object on the screen.
 */
void Point::draw() {
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

/**
 * @brief Constructor for the Circle class.
 * 
 * @param position Position of the circle in 2D space.
 * @param color Color of the circle in RGB format.
 * @param radius Radius of the circle.
 * @param shaded Whether the circle should be shaded or not.
 * @param point_type Type of coordinates (default is PointType::Fraction of the window).
 */
Circle::Circle(const glm::vec2& position, const glm::vec3& color, float radius, bool shaded, PointType point_type)
    : Shape(position, color), radius(radius), shaded(shaded) {
    // Adjust the position based on the point type
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

/**
 * @brief Constructor for the Circle class.
 * 
 * @param coordinates Coordinates of the circle in pixels.
 * @param color Color of the circle in RGB format.
 * @param radius Radius of the circle.
 * @param shaded Whether the circle should be shaded or not.
 * @param point_type Type of coordinates (default is PointType::Fraction of the window).
 */
Circle::Circle(const std::vector<float>& coordinates, const std::vector<float>& color, float radius, bool shaded, PointType point_type)
    : Shape({coordinates[0], coordinates[1]}, {color[0], color[1], color[2]}), radius(radius), shaded(shaded) {
    // Adjust the position based on the point type
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

/**
 * @brief Draws the Circle object on the screen.
 * 
 * If the circle is visible, it can be drawn as a filled circle or an outline.
 * The number of segments can be adjusted to control the smoothness or coarseness of the circle.
 * Uses GL_TRIANGLE_FAN to draw a filled circle and GL_LINE_LOOP to draw the circle as an outline.
 * 
 * @note This function uses OpenGL for drawing.
 */
void Circle::draw() {
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

/**
 * @brief Constructor for the Line2D class.
 * 
 * @param points Points that define the line in 2D space.
 * @param color Color of the line in RGB format.
 * @param point_type Type of coordinates (default is PointType::Fraction of the window).
 */
Line2D::Line2D(std::vector<glm::vec2>& points, const glm::vec3& color, PointType point_type)
    : Shape(glm::vec2{0.0f, 0.0f}, color), points(points) {
    // Adjust the position based on the point type
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

/**
 * @brief Draws the Line2D object.
 * 
 * This function uses OpenGL to draw the Line2D object on the screen.
 */
void Line2D::draw() {
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

/**
 * @brief Constructor for the Triangle class.
 * 
 * @param point1 First point of the triangle.
 * @param point2 Second point of the triangle.
 * @param point3 Third point of the triangle.
 * @param color Color of the triangle in RGB format.
 * @param point_type Type of coordinates (default is PointType::Fraction of the window).
 */
Triangle::Triangle(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3, const glm::vec3& color, PointType point_type)
    : Shape(glm::vec2{0.0f, 0.0f}, color) {
    p1 = point1;
    p2 = point2;
    p3 = point3;

    // Adjust the position based on the point type
    if (point_type == PointType::Pixel) {
        p1.x = (p1.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
        p1.y = (p1.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;

        p2.x = (p2.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
        p2.y = (p2.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;

        p3.x = (p3.x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
        p3.y = (p3.y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;
    }
}

/**
 * @brief Draws the Triangle object.
 * 
 * This function uses OpenGL to draw the Triangle object on the screen.
 */
void Triangle::draw() {
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
