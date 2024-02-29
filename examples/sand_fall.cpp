#include "lumos.h"

#define WORLD_WIDTH 800
#define WORLD_HEIGHT 800
#define GRID_SIZE_X 400
#define GRID_SIZE_Y 400
#define CELL_SIZE_X 2
#define CELL_SIZE_Y 2

int main() {
    App app = App(WORLD_WIDTH, WORLD_HEIGHT, "Testing Window");

    std::vector<std::vector<Quad*>> grid;

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
            Quad* q = new Quad{
                glm::vec2{(WORLD_WIDTH / GRID_SIZE_X) * CELL_SIZE_X * i,
                          (WORLD_HEIGHT / GRID_SIZE_Y) * CELL_SIZE_Y * j},
                (WORLD_HEIGHT / GRID_SIZE_Y) * CELL_SIZE_Y,
                (WORLD_WIDTH / GRID_SIZE_X) * CELL_SIZE_X,
                //  glm::vec3{float(rand()) / RAND_MAX, 0.0, 0.0},
                glm::vec3{red, green, blue}, PointType::Pixel};
            // if (j >= (GRID_SIZE_Y) / 10)
            q->is_visible = false;
            temp.push_back(q);
        }
        grid.push_back(temp);
    }

    app.add_update_system([&] (App&){
           for (size_t i = 0; i < GRID_SIZE_X; i++) {
               for (size_t j = 0; j < GRID_SIZE_Y; j++) {
                   if (grid[i][j]->is_visible) {
                       grid[i][j]->draw();
                   }
               }
           }
       })
        .add_fixed_update_system(  // Systems for updating particle physics
            [&] (App&){
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
            1)
        .add_fixed_update_system(
            [&] (App&){
                for (size_t i = 0; i < GRID_SIZE_X; i++) {
                    if (rand() % 100 == 0) {
                        grid[i][GRID_SIZE_Y - 1]->is_visible = true;
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
