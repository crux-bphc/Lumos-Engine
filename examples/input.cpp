#include "lumos.h"

int main() {
    App app = App(800, 600, "Testing Window");
    app.add_key_callback_system([&](int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            spdlog::info("Escape key pressed");
            app.close();
        }
    })
    .add_key_callback_system([&](int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            spdlog::info("A key pressed");
        }
    })
    .add_mouse_callback_system([&](int button, int action) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            spdlog::info("Left mouse button pressed");
        }
    })
    .add_scroll_callback_system([&](int xoffset, int yoffset, int action) {
        spdlog::info("Scroll callback function called");
        spdlog::info("xoffset: {}, yoffset: {}, action: {}", xoffset, yoffset, action);
    });
    app.run();
}