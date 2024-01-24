#include "lumos.h"

int main() {
    App* app = new App(800, 800, "Batch Renderer", false, true);

    Quad q1 = Quad(glm::vec2{400, 400}, 100, 100, glm::vec3{1.0, 0.0, 0.0}, PointType::Pixel);
    Quad q2 = Quad(glm::vec2{500, 500}, 100, 100, glm::vec3{0.0, 1.0, 0.0}, PointType::Pixel);
    Quad q3 = Quad(glm::vec2{300, 300}, 100, 100, glm::vec3{0.0, 0.0, 1.0}, PointType::Pixel);

    BatchRenderer batch_renderer;
    batch_renderer.add_shape(q1);
    batch_renderer.add_shape(q2);
    batch_renderer.add_shape(q3);

    app->add_update_system([&] {
        batch_renderer.draw();
    });

    app->run();
}