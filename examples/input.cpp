#include "lumos.h"

int main() {
    App app = App(800, 600, "Testing Window");
    app.add_update_system([](App& app) {
        if (app.Input->isKeyPressed(GLFW_KEY_ESCAPE)) {
            spdlog::info("Escape key pressed");
            app.close();
        }
        if (app.Input->isKeyPressed(GLFW_KEY_A)) {
            spdlog::info("A key pressed");
        }
        if (app.Input->isKeyPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            spdlog::info("Left mouse button pressed");
        }
        spdlog::info("Scroll callback function called");
        spdlog::info("xoffset: {}, yoffset: {}", app.Input->scrollPos.x, app.Input->scrollPos.y);
    });
    app.run();
}
