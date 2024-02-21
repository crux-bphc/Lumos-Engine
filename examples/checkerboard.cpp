#include <cmath>
#include <vector>

#include "lumos.h"

int main() {
    App app = App(800, 800, "Testing Window", false, true);
    int pixel_size = 100;

    for (int i = 0; i <= 8; i++) {
        for (int j = 0; j <= 8; j++) {
            // Create a new Point object for each iteration and add it to the
            // vector
            Quad* p = new Quad(glm::vec2{i * pixel_size, j * pixel_size},
                               pixel_size, pixel_size,
                               (i + j) % 2 ? glm::vec3{1.0, 1.0, 1.0}
                                           : glm::vec3{0.5, 0.5, 1.0},
                               PointType::Pixel);
            Point* q =
                new Point(glm::vec2{i * pixel_size, j * pixel_size},
                          glm::vec3{1.0, 0.0, 0.0}, 10.0f, PointType::Pixel);
            app.add_update_system([p]() { p->draw(); })
                .add_update_system([q]() { q->draw(); });
        }
    }

    app.run();
}
