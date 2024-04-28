#include "shapes.h"


Shape::Shape(const glm::vec2& position, const glm::vec3& color, bool is_visible)
    : position(position), color(color), is_visible(is_visible) {}

void Shape::hide() {
    this->is_visible = false;
}

Shape::~Shape() {
}

// We are assuming the vector point are given in such a fashion such that
// first two point are lower left points and they rest are present in a clockwise manner
// the position are in pixels
Quad::Quad(const std::vector<float>& coordinates, const std::vector<float>& color, PointType point_type)
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
    this->vertices = {
        {min_x, min_y},
        {max_x, min_y},
        {max_x, max_y},
        {min_x, max_y},
    };
}

Quad::Quad(const glm::vec2& position, float height, float width, const glm::vec3& color, PointType point_type)
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
    this->vertices.push_back({position.x, position.y});
    this->vertices.push_back({position.x + width, position.y});
    this->vertices.push_back({position.x + width, position.y + height});
    this->vertices.push_back({position.x, position.y + height});
    
}

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

// It's in the shape of a square though (idk why)
Point::Point(const glm::vec2& position, const glm::vec3& color, float size, PointType point_type)
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
    this->vertices = {
        {position.x, position.y},
    };
}

// Point(const std::vector<float>& coordinates, const std::vector<float>& color, PointType point_type = PointType::Fraction)
//     : Shape({coordinates[0], coordinates[1]}, {color[0], color[1], color[2]}) {
// }

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

    
Circle::Circle(const glm::vec2& position, const glm::vec3& color, float radius, bool shaded, PointType point_type)
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

Circle::Circle(const std::vector<float>& coordinates, const std::vector<float>& color, float radius, bool shaded, PointType point_type)
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

Line2D::Line2D(std::vector<glm::vec2>& points, const glm::vec3& color, PointType point_type)
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
    this->vertices.push_back({points[0][0], points[0][1]});
    this->vertices.push_back({points[1][0], points[1][1]});

}

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


Triangle::Triangle(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3, const glm::vec3& color, PointType point_type)
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

/*collisionCheck(obj1, obj2) {checks posns and dim for collisions}
types: 
1. sprite2D
2. triangle
3. line2D
4. Circle
5. Point
6. Quad

Sprite2D seems miscellaneous, will need to use vertices to classify into appropriate if conditions */
 
//separating axes theorem implementation for collision detection

bool SATCollision(const std::vector<std::pair<float,float>>& polygon1, const std::vector<std::pair<float,float>>& polygon2) {
    // Loop through all edges of both polygons
    for (size_t i = 0; i < polygon1.size(); ++i) {
        // Get the edge normal (perpendicular to the edge direction)
        std::pair<float,float> edge = polygon1[(i + 1) % polygon1.size()];
        edge.first -= polygon1[i].first;
        edge.second -=polygon1[i].second;
        std::pair<float,float> normal = {edge.second, -edge.first}; // Normalized normal

        // Project all vertices of both polygons onto the axis defined by the normal
        double minProj1 = polygon1[0].first * normal.first + polygon1[0].second * normal.second;
        double maxProj1 = minProj1;
        double minProj2 = polygon2[0].first * normal.first + polygon2[0].second * normal.second;
        double maxProj2 = minProj2;
        for (size_t j = 1; j < polygon1.size(); ++j) {
        double projection = polygon1[j].first * normal.first + polygon1[j].second * normal.second;
        minProj1 = std::min(minProj1, projection);
        maxProj1 = std::max(maxProj1, projection);
        }
        for (size_t j = 1; j < polygon2.size(); ++j) {
        double projection = polygon2[j].first * normal.first + polygon2[j].second * normal.second;
        minProj2 = std::min(minProj2, projection);
        maxProj2 = std::max(maxProj2, projection);
        }

        // Check for overlap on this axis
        if (maxProj1 < minProj2 || maxProj2 < minProj1) {
        return false; // Separating axis found, no collision
        }
    }

    // No separating axis found, polygons are colliding
    return true;
    }

bool onSegment(std::pair<float, float> p, std::pair<float, float> q, std::pair<float, float> r) { 
    if (q.first <= std::max(p.first, r.first) && q.first >= std::min(p.first, r.first) && 
        q.second <= std::max(p.second, r.second) && q.second >= std::min(p.second, r.second)) 
       return true; 
    return false; 
} 

int orientation(std::pair<float, float> p, std::pair<float, float> q, std::pair<float, float> r) { 
    int val = (q.second - p.second) * (r.first - q.first) - 
              (q.first - p.first) * (r.second - q.second); 
    if (val == 0) return 0; 
    return (val > 0)? 1: 2; 
} 

bool doLinesIntersect(std::pair<float, float> p1, std::pair<float, float> q1, std::pair<float, float> p2, std::pair<float, float> q2) { 
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
    if (o1 != o2 && o3 != o4) 
        return true; 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
    return false; 
} 


bool isPointOnLine(const glm::vec2& point, const glm::vec2& line1, const glm::vec2& line2) {
    //check if point lies on line by summing distances, -epsilon<difference<epsilon not needed cuz grid uses int for now
    float dx1 = line2.x - line1.x;
    float dy1 = line2.y - line1.y;
    float dx2 = line2.x - line1.x;
    float dy2 = line2.y - line1.y;
    if (dx1 + dx2 == abs(line1.x - line2.x) && dy1 + dy2 == abs(line1.y - line2.y))
        return true;
    return false;
}

//Ray casting algorithm to check if point lies inside a polygon

bool isPointInsidePolygon(const std::pair<float, float>& point, const std::vector<std::pair<float, float>>& polygon) {
  int num_vertices = polygon.size();
  float x = point.first, y = point.second;
  bool inside = false;

  std::pair<float, float> p1 = polygon[0], p2;

  for (int i = 1; i <= num_vertices; i++) {
    p2 = polygon[i % num_vertices];

    if (y > (p1.second > p2.second ? p2.second : p1.second) && 
        y <= (p1.second < p2.second ? p2.second : p1.second)) {
        if (x <= (p1.first > p2.first ? p1.first : p2.first)) {
          float x_intersection = (y - p1.second) * (p2.first - p1.first) / (p2.second - p1.second) + p1.first;

          if (p1.first == p2.first || x <= x_intersection) {
            inside = !inside;
          }
        }
      }
    

    p1 = p2;
  }

  return inside;
}

bool DoesLineCrossLine(const Line2D& obj1, const Line2D& obj2){
    //check if lines intersect by checking if endpoints lie on opposite sides of the other line
    std::pair<float,float> p1 = {obj1.points[0][0], obj1.points[0][1]};
    std::pair<float,float> q1 = {obj1.points[1][0], obj1.points[1][1]};
    std::pair<float,float> p2 = {obj2.points[0][0], obj2.points[0][1]};
    std::pair<float,float> q2 = {obj2.points[1][0], obj2.points[1][1]};

    float o1 = orientation(p1, q1, p2);
    float o2 = orientation(p1, q1, q2);
    float o3 = orientation(p2, q2, p1);
    float o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;
    return false;
}


bool collisionCheck(const Shape& obj1, const Shape& obj2) {

    //this is for simple polygons, for convex shapes we need to split into base components and make axes

    /*template <typename T>
    T Downcast = [](const Shape& obj) {
        
        if (dynamic_cast<const Quad*>(&obj)) {
            const Quad& quad = dynamic_cast<const Quad&>(obj);
            return quad;}};*/

    //circles intersect if distance between centers is less than sum of radii
    if (obj1.getShapeType() == "Circle" && obj2.getShapeType() == "Circle"){
        const Circle& obj1 = dynamic_cast<const Circle&>(obj1);
        const Circle& obj2 = dynamic_cast<const Circle&>(obj2);
        float dist = sqrt(pow(obj1.position.x - obj2.position.x, 2) + pow(obj1.position.y - obj2.position.y, 2));
        if (dist>(obj1.radius + obj2.radius))
            return false;
        return true;
    }

    //two points intersect if same position    
    else if (obj1.getShapeType() == "Point" && obj2.getShapeType() == "Point"){
        if (obj1.position == obj2.position)
            return true;
        return false;
    }

    //line line intersection based on orientation of points
    else if (obj1.getShapeType()=="Line2D" && obj2.getShapeType()=="Line2D"){
        const Line2D& obj1 = dynamic_cast<const Line2D&>(obj1);
        const Line2D& obj2 = dynamic_cast<const Line2D&>(obj2);
        return DoesLineCrossLine(obj1, obj2);
    }

    //point lies on line if it satisfies section theorem with points
    else if (obj1.getShapeType()=="Point" && obj2.getShapeType()=="Line2D" ||
         obj1.getShapeType()=="Line2D" && obj2.getShapeType()=="Point"){

        const Point& point = obj1.getShapeType() == "Point" ? dynamic_cast<const Point&>(obj1) : dynamic_cast<const Point&>(obj2);
        const Line2D& line = obj1.getShapeType() == "Point" ? dynamic_cast<const Line2D&>(obj2) : dynamic_cast<const Line2D&>(obj1);

        if (isPointOnLine(point.position, line.points[0], line.points[1]))
            return true;
        return false;
    }

    //point in polygon
    else if ((obj1.getShapeType() == "Point" && (obj2.getShapeType() == "Quad" || obj2.getShapeType() == "Triangle")) ||
            (obj2.getShapeType() == "Point" && (obj1.getShapeType() == "Quad" || obj1.getShapeType() == "Triangle"))) {

        const Point& point = dynamic_cast<const Point&>(obj1.getShapeType()=="Point" ? obj1 : obj2);

        if (obj2.getShapeType()=="Quad") {
            const Quad& polygon = dynamic_cast<const Quad&>(obj2);
            return isPointInsidePolygon(point.vertices[0], polygon.vertices);
        }
        else {
            const Triangle& polygon = dynamic_cast<const Triangle&>(obj2);
            return isPointInsidePolygon(point.vertices[0], polygon.vertices);
        }
    }

    //line polygon
    else if ((obj1.getShapeType() == "Line2D" && (obj2.getShapeType() == "Quad" || obj2.getShapeType() == "Triangle")) ||
            (obj2.getShapeType() == "Line2D" && (obj1.getShapeType() == "Quad" || obj1.getShapeType() == "Triangle"))) {

        const Line2D& line = dynamic_cast<const Line2D&>(obj1.getShapeType()=="Line2D" ? obj1 : obj2);
        //little bit left here

    }

    //similar to point polygon, needs some modification

    //polygon polygon
    else {
        return SATCollision(obj1.vertices, obj2.vertices);
    }

    
    

    






     
    




    

    return false;
}


