#include "lumos.h"

#define WORLD_WIDTH 800
#define WORLD_HEIGHT 800
#define GRID_SIZE_X 400
#define GRID_SIZE_Y 400
#define CELL_SIZE_X 2
#define CELL_SIZE_Y 2
#define SCROLL_SENSITIVITY 1  // Adjust the sensitivity as needed

float brushRadius = CELL_SIZE_X;  // Initial brush radius

struct colGrid {
    entt::entity entt;
    bool isShown;
};

int main() {
    App app(WORLD_WIDTH, WORLD_HEIGHT, "Sand Sim");
    Circle* locator = new Circle(glm::vec2{WORLD_WIDTH / 2, WORLD_HEIGHT / 2},
                                 glm::vec3{1.0, 1.0, 1.0}, brushRadius * 2.0f,
                                 false, PointType::Pixel);

    colGrid grid[GRID_SIZE_X][GRID_SIZE_Y];

    // Initialize grid
    for (size_t i = 0; i < GRID_SIZE_X; i++) {
        std::vector<Quad*> temp;
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
                rect rect_in = {{glm::vec2{PIXEL(((float)WORLD_WIDTH / GRID_SIZE_X) * CELL_SIZE_X * i, WORLD_WIDTH),
                          PIXEL(((float)WORLD_HEIGHT / GRID_SIZE_Y) * CELL_SIZE_Y * j, WORLD_HEIGHT)}},
                     {glm::vec2{PIXEL(((float)WORLD_WIDTH / GRID_SIZE_Y) * CELL_SIZE_Y, WORLD_WIDTH),
                          PIXEL(((float)WORLD_WIDTH / GRID_SIZE_X) * CELL_SIZE_X, WORLD_HEIGHT)}},
                      0,
                     {glm::vec3{red, green, blue}}};
            auto ent = sprite2D(app, rect_in, false);
            grid[i][j] = {ent, false};
        }
    }

    app.add_update_system([&grid] (App& app) {
           for (size_t i = 0; i < GRID_SIZE_X; i++) {
               for (size_t j = 0; j < GRID_SIZE_Y; j++) {
                   auto &toDraw = app.reg.get<isDrawn>(grid[i][j].entt);
                   if (grid[i][j].isShown) {
                       toDraw = {true};
                   }
                   else {
                       toDraw = {false};
                   }
               }
           }
           //locator->draw();
       })
        .add_fixed_update_system(  // Systems for updating particle physics
            [&grid, &locator] (App& app) {
                locator->radius = brushRadius * 2.0f;

                locator->position =
                    glm::vec2{app.Input->getMousePosition().x,
                              app.Input->getMousePosition().y};

                for (size_t i = 0; i < GRID_SIZE_X; i++) {
                    for (size_t j = 1; j < GRID_SIZE_Y;
                         j++) {  // Last layer will not get update (for now)
                        if (grid[i][j].isShown) {
                            if (!grid[i][j - 1]
                                     .isShown) {  // If there is no particle
                                                      // just beneath it
                                grid[i][j].isShown = false;
                                grid[i][j - 1].isShown = true;
                            } else {
                                int direction = (rand() % 2) * 2 - 1;

                                if (int(i) + direction >= 0 &&
                                    i + direction < GRID_SIZE_X) {
                                    // Check boundaries before moving
                                    if (!grid[i + direction][j - 1]
                                             .isShown) {
                                        grid[i][j].isShown = false;
                                        grid[i + direction][j - 1].isShown =
                                            true;
                                    }
                                }
                            }
                        }
                    }
                }
            }, 10)
        .add_update_system([&grid](App& app) {
            if (app.Input->isMousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
                //spdlog::info("Left mouse button pressed");
                //spdlog::info("Mouse position: {}, {}",
                //             app.Input->getMousePosition().x,
                //             app.Input->getMousePosition().y);
                double mouseX = app.Input->getMousePosition().x;
                double mouseY = app.Input->getMousePosition().y;
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
                            grid[i][j].isShown = true;
                            //spdlog::debug(
                            //    "Putting pixel at {} {}",
                            //    (WORLD_WIDTH / GRID_SIZE_X) * CELL_SIZE_X * i,
                            //    (WORLD_HEIGHT / GRID_SIZE_Y) * CELL_SIZE_Y * j);
                        }
                    }
                }
            }
            if (app.Input->isKeyPressed(GLFW_KEY_ESCAPE)) {
                spdlog::info("Escape Pressed");
                app.close();
            }
        })
        .add_update_system([](App& app) {
            //spdlog::info("Scroll callback function called");
            //spdlog::info("xoffset: {}, yoffset: {}", 
            //            app.Input->scrollPos.x,
            //            app.Input->scrollPos.y);
            brushRadius = app.Input->scrollPos.y * SCROLL_SENSITIVITY;
            brushRadius = std::max(float(CELL_SIZE_X),
                                   std::min(brushRadius,
                                            100.0f));  // Adjust the maximum
                                                       // brush radius if needed
            //spdlog::info("Brush radius: {}", brushRadius);
        });
    app.run();

    delete locator;
    return 0;
}
