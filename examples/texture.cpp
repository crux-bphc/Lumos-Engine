#include <cmath>
#include <vector>

#include "lumos.h"
#include "shader.h"
#include "textures.h"

void glide(rect& texRect) { texRect.pos.x += 0.1f; }

int main() {
    App app = App(800, 800, "Testing Window", false, true);
    spdlog::debug("Setting up rects");
    rect texRect;
    texRect.pos = {-0.5f, -0.5f};
    texRect.dim = {1.0f, 1.0f};
    Texture::initShader();
    Texture* tex =
        new Texture("../assets/ass.png", GL_RGB, GL_UNSIGNED_BYTE, texRect, 0);

    app.add_update_system([tex]() { tex->draw(); })
        .add_update_system([&texRect, tex]() {
            texRect.pos.x += 0.01f;
            tex->updateRect(texRect);
        });

    app.run();
}
