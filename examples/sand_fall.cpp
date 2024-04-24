#include "lumos.h"

#define WORLD_WIDTH 800
#define WORLD_HEIGHT 800
#define GRID_SIZE_X 400
#define GRID_SIZE_Y 400
#define CELL_SIZE_X 2
#define CELL_SIZE_Y 2

struct colGrid {
    entt::entity entt;
    bool isShown;
};

int main() {
    App app = App(WORLD_WIDTH, WORLD_HEIGHT, "Testing Window");

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

    app.add_update_system([&] (App&){
           for (size_t i = 0; i < GRID_SIZE_X; i++) {
               for (size_t j = 0; j < GRID_SIZE_Y; j++) {
                   auto &toDraw = app.reg.get<isDrawn>(grid[i][j].entt);
                   if (grid[i][j].isShown) {
                       toDraw = {true};
                       spdlog::debug("pos: {}, {}", i, j);
                   }
                   else {
                       toDraw = {false};
                   }
               }
           }
       })
        .add_fixed_update_system(  // Systems for updating particle physics
            [&] (App&){
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
            },
            1)
        .add_fixed_update_system(
            [&] (App&){
                for (size_t i = 0; i < GRID_SIZE_X; i++) {
                    if (rand() % 100 == 0) {
                        grid[i][GRID_SIZE_Y - 1].isShown = true;
                    }
                }
            },
            10)
        .add_update_system([](App& app) {
            if (app.Input->isKeyPressed(GLFW_KEY_ESCAPE)) {
                spdlog::debug("Escape key pressed, closing the application");
                app.close();
            }
        });
    app.run();
}
