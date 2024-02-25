#include <cmath>
#include <vector>

#include "lumos.h"
#include "shader.h"
#include "textures.h"
#include "camera.h"

void glide(rect& texRect) { texRect.pos.x += 0.1f; }

int main() {
    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 800;
    App app = App(800, 800, "Testing Window", false, true);
    
    spdlog::debug("Setting up rects");
    rect texRect;
    texRect.pos = {-0.5f, -0.5f};
    texRect.dim = {1.0f, 1.0f};
    Camera* camera = new Camera(glm::vec3(texRect.pos.x , texRect.pos.y , 1.0f));
    Texture::initShader(*camera);
    Texture* tex =
        new Texture("../assets/ass.png", GL_RGB, GL_UNSIGNED_BYTE, texRect, 0, camera);


    //GLuint projection_loc = camera->ProjectionLoc;    
    //printf("%i", projection_loc);
    //GLuint view_loc = camera->viewLoc;

    app.add_update_system([tex]() { tex->draw(); })
        .add_update_system([&texRect, tex, &camera]() {
            texRect.pos.x += 0.00001f;
            tex->updateRect(&texRect, *camera);
            printf("%f %f", texRect.pos.x, texRect.pos.y);
        })
        .add_key_callback_system([&](int key, int scancode, int action, int mods) {
            camera->direction = STATIC;
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                if (key == GLFW_KEY_UP)
                    camera->direction = UP; 
                if (key == GLFW_KEY_DOWN)
                    camera->direction = DOWN; 
                if (key == GLFW_KEY_LEFT)
                    camera->direction = LEFT; 
                if (key == GLFW_KEY_RIGHT)
                    camera->direction = RIGHT; 
            } else if (action == GLFW_RELEASE) {
                camera->direction = STATIC; 
            }
        })
        .add_fixed_update_system([&camera]() {camera->ProcessKeyboard(); })
        .add_fixed_update_system([&]() {
            glm::mat4 projection = camera->GetProjectionMatrix(SCR_WIDTH,SCR_HEIGHT);
            //glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
            })

        .add_scroll_callback_system([&] (int xoffset, int yoffset, int action) {
            camera->ProcessMouseScroll(yoffset); 
        })

        .add_fixed_update_system([&]() {
        glm::mat4 view = camera->GetViewMatrix();
        //glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        })
        
        .run();
}
