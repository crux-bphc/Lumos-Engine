#include "lumos.h"

#define WORLD_WIDTH 800
#define WORLD_HEIGHT 800
#define GRID_SIZE_X 20
#define GRID_SIZE_Y 20
#define CELL_SIZE_X 40
#define CELL_SIZE_Y 40
#define SCROLL_SENSITIVITY 1  // Adjust the sensitivity as needed

float brushRadius = CELL_SIZE_X;  // Initial brush radius

int main() {
    App* app = new App(WORLD_WIDTH, WORLD_HEIGHT, "Sand Sim");
    Circle* locator = new Circle(glm::vec2{WORLD_WIDTH / 2, WORLD_HEIGHT / 2},
                                 glm::vec3{1.0, 1.0, 1.0}, brushRadius * 2.0f,
                                 false, PointType::Pixel);

    //std::vector<std::vector<Quad*>> grid;
    renderer rend;

    // Initialize grid
    for (size_t i = 0; i < GRID_SIZE_X; i++) {
        //std::vector<Quad*> temp;
        for (size_t j = 0; j < GRID_SIZE_Y; j++) {
            float red =
                0.545 +
                0.2 *
                    ((float)rand() /
                     RAND_MAX);  // 0.545 is a starting point, adjust as needed
            float green =
                0.27 +
                0.2 * ((float)rand() /
                       RAND_MAX);  // 0.27 is a starting point, adjust as needed
            float blue =
                0.07;  // Constant value for blue to maintain brownish tone
            rect rect_in = {    {glm::vec2{PIXEL(CELL_SIZE_X * i, WORLD_WIDTH), PIXEL(CELL_SIZE_Y * j, WORLD_HEIGHT)}},
                                {glm::vec2(PIXEL(CELL_SIZE_X, WORLD_WIDTH), PIXEL(CELL_SIZE_Y, WORLD_HEIGHT))},
                                0,
                                {glm::vec3(red, green, blue)}};
            sprite2D(*app, rend, rect_in, true);
            //temp.push_back(q);
        }
        //grid.push_back(temp);
    }

    /*
    app->add_update_system([&] (App& app) {
           for (size_t i = 0; i < GRID_SIZE_X; i++) {
               for (size_t j = 0; j < GRID_SIZE_Y; j++) {
                   if (grid[i][j]->is_visible) {
                       grid[i][j]->draw();
                   }
               }
           }
           locator->draw();
       })
        .add_fixed_update_system(  // Systems for updating particle physics
            [grid, locator] (App& app) {
                locator->radius = brushRadius * 2.0f;

                locator->position =
                    glm::vec2{std::get<0>(app.get_mouse_position()),
                              std::get<1>(app.get_mouse_position())};

                for (size_t i = 0; i < GRID_SIZE_X; i++) {
                    for (size_t j = 1; j < GRID_SIZE_Y;
                         j++) {  // Last layer will not get update (for now)
                        if (grid[i][j]->is_visible) {
                            if (!grid[i][j - 1]
                                     ->is_visible) {  // If there is no particle
                                                      // just beneath it
                                grid[i][j]->is_visible = false;
                                grid[i][j - 1]->is_visible = true;
                            } else {
                                int direction = (rand() % 2) * 2 - 1;

                                if (int(i) + direction >= 0 &&
                                    i + direction < GRID_SIZE_X) {
                                    // Check boundaries before moving
                                    if (!grid[i + direction][j - 1]
                                             ->is_visible) {
                                        grid[i][j]->is_visible = false;
                                        grid[i + direction][j - 1]->is_visible =
                                            true;
                                    }
                                }
                            }
                        }
                    }
                }
            },
            10)
        .add_mouse_callback_system([&](int button, int action) {
            if (button == GLFW_MOUSE_BUTTON_LEFT && app->is_mouse_pressed()) {
                spdlog::info("Left mouse button pressed");
                spdlog::info("Mouse position: {}, {}",
                             std::get<0>(app->get_mouse_position()),
                             std::get<1>(app->get_mouse_position()));
                double mouseX = std::get<0>(app->get_mouse_position());
                double mouseY = std::get<1>(app->get_mouse_position());
                // Apply forces to particles in a radius around the mouse
                for (size_t i = 0; i < GRID_SIZE_X; i++) {
                    for (size_t j = 0; j < GRID_SIZE_Y; j++) {
                        float distance = glm::distance(
                            glm::vec2(
                                (WORLD_WIDTH / GRID_SIZE_X) * CELL_SIZE_X * i,
                                (WORLD_HEIGHT / GRID_SIZE_Y) * CELL_SIZE_Y * j),
                            glm::vec2(mouseX, mouseY));
                        if (distance < CELL_SIZE_X * brushRadius) {
                            // Apply some force or update particle state
                            // here For now, just make them visible
                            grid[i][j]->is_visible = true;
                            spdlog::debug(
                                "Putting pixel at {} {}",
                                (WORLD_WIDTH / GRID_SIZE_X) * CELL_SIZE_X * i,
                                (WORLD_HEIGHT / GRID_SIZE_Y) * CELL_SIZE_Y * j);
                        }
                    }
                }
            }
        })
        .add_scroll_callback_system([&](int xoffset, int yoffset, int action) {
            spdlog::info("Scroll callback function called");
            spdlog::info("xoffset: {}, yoffset: {}, action: {}", xoffset,
                         yoffset, action);
            brushRadius += yoffset * SCROLL_SENSITIVITY;
            brushRadius = std::max(float(CELL_SIZE_X),
                                   std::min(brushRadius,
                                            100.0f));  // Adjust the maximum
                                                       // brush radius if needed
            spdlog::info("Brush radius: {}", brushRadius);
        });
    */
    app->run();

    delete locator;
    delete app;
}
