#include "lumos.h"

class Box : public Quad {
    public:
    // Use the Quad constructor that takes coordinates and color
    Box(glm::vec2 position, int height, int width, glm::vec3 color = {1.0, 1.0, 1.0}) : Quad(position, height, width, color) {}

    void glide() {
        if (this->position.x > 1.0f) {
            this->position.x = -2.0f;
        }

        this->position = glm::vec2{
            this->position.x + 0.01f,
            this->position.y,
        };
    }
};

int main() {
    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 600;
    App app = App(SCR_WIDTH, SCR_HEIGHT, "Testing Window");
    Box* box = new Box(glm::vec2{-0.5f, -0.5f}, 1.0f, 1.0f, glm::vec3{1.0f, 0.0f, 0.0f});
    Camera camera(glm::vec3(0.0f,0.0f,3.0f));
    Camera_Movement direction = STATIC; 
    //glm::vec3 cameraPosition = camera.GetPosition();
    
    /*glm::mat4 projection = camera.GetProjectionMatrix(camera,SCR_WIDTH,SCR_HEIGHT);
    GLuint projection_loc = camera.projection_loc;
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 view = camera.GetViewMatrix();
    GLuint view_loc = camera.view_loc;
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));*/

    app.add_update_system([&box]() {
        box->draw();
        })
        .add_fixed_update_system([&box]() {
            box->glide();
        }, 1)

        .add_mouse_callback_system([&camera](int xoffset, int yoffset) {
            camera.ProcessMouseMovement(xoffset, yoffset);
        })
        .add_key_callback_system([&](int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                if (key == GLFW_KEY_W)
                    direction = FORWARD;
                if (key == GLFW_KEY_S)
                    direction = BACKWARD;
                if (key == GLFW_KEY_A)
                    direction = LEFT;
                if (key == GLFW_KEY_D)
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