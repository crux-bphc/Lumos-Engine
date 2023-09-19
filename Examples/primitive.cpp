#include <unistd.h>  // Put this line at the top of the file outside of all functions, of course

#include "../Lumos/lumos.cpp"

int main() {
    char dir[1024];
    getcwd(dir, sizeof(dir));
    std::cout << "CWD = " << dir << std::endl;

    App app = App(800, 600, "Testing Window");

    Shader shader("../Examples/all_red_vertex_shader.glsl", "../Examples/all_red_fragment_shader.glsl");

    app.add_system(SystemType::Startup, [&shader]() {
        shader.init();
    });

    // Create a Box
    Quad* box = new Quad(glm::vec2{0, 0}, 400, 400, glm::vec3{1.0f, 0.0f, 0.0f}, PointType::Pixel);

    // Create a Point
    Point* point = new Point(glm::vec2{0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});

    // Create a circle
    Circle* circle = new Circle(glm::vec2{0.5f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, 10.0f);
    circle->set_shader_program(shader);

    std::vector<glm::vec2> points = {glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}};
    Line2D* line = new Line2D(points, glm::vec3{1.0, 1.0, 1.0}, PointType::Fraction);

    std::vector<glm::vec2> controlPoints;
    controlPoints.push_back(glm::vec2{100.0f, 100.0f});  // First control point
    controlPoints.push_back(glm::vec2{200.0f, 300.0f});  // Second control point
    controlPoints.push_back(glm::vec2{300.0f, 100.0f});  // Third control point

    Triangle* tri = new Triangle(glm::vec2{100.0f, 100.0f}, glm::vec2{200.0f, 300.0f}, glm::vec2{300.0f, 100.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, PointType::Pixel);

    app.add_system(SystemType::Update, [&shader]() {
           shader.use();
       })
        .add_system(SystemType::Update, [&box]() {
            box->draw();
        })
        .add_system(SystemType::Update, [&point]() {
            point->draw();
        })
        .add_system(SystemType::Update, [&circle]() {
            circle->draw();
        })
        .add_system(SystemType::Update, [&line]() {
            line->draw();
        })
        .add_system(SystemType::Update, [&tri]() {
            tri->draw();
        })
        .run();
}