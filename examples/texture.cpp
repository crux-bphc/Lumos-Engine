#include <cmath>
#include <vector>

#include "lumos.h"
#include "shader.h"
#include "textures.h"
#include "camera2D.h"

void glide(rect& texRect) { texRect.pos.x += 0.1f; }

int main() {
    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 800;
    App app = App(800, 800, "Testing Window", false, true);
    Camera camera(glm::vec3(0.0f,0.0f,3.0f));
    spdlog::debug("Setting up rects");
    rect texRect;
    texRect.pos = {-0.5f, -0.5f};
    texRect.dim = {1.0f, 1.0f};
    Texture::initShader(camera);
    Texture* tex =
        new Texture("../assets/ass.png", GL_RGB, GL_UNSIGNED_BYTE, texRect, 0);

    
    Camera_Movement direction = STATIC;
    glm::mat4 projection = camera.GetProjectionMatrix(camera,SCR_WIDTH,SCR_HEIGHT);
    GLuint projection_loc = camera.ProjectionLoc;
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    app.add_update_system([tex]() { tex->draw(); })
        .add_update_system([&texRect, tex]() {
            texRect.pos.x += 0.01f;
            tex->updateRect(texRect);
        })
        .add_key_callback_system([&](int key, int scancode, int action, int mods) {
            Camera_Movement direction = STATIC;
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                if (key == GLFW_KEY_UP)
                    direction = UP; 
                if (key == GLFW_KEY_DOWN)
                    direction = DOWN; 
                if (key == GLFW_KEY_LEFT)
                    direction = LEFT; 
                if (key == GLFW_KEY_RIGHT)
                    direction = RIGHT; 
            } else if (action == GLFW_RELEASE) {
                direction = STATIC; 
            }
        })
        .add_fixed_update_system([&]() {
            camera.ProcessKeyboard(direction);
        })
        .add_scroll_callback_system([&] (int xoffset, int yoffset, int action) {
            camera.ProcessMouseScroll(yoffset);
        })
        
        .run();
}
