#include "../Lumos/lumos.cpp"

class Box : public Quad {
   public:
    // Use the Quad constructor that takes coordinates and color
    Box(Vec2 position, int height, int width, Color color = {1.0, 1.0, 1.0}) : Quad(position, height, width, color) {}

    void glide() {
        if (this->position.x > 1.0f) {
            this->position.x = -2.0f;
        }

        this->position = Vec2{
            this->position.x + 0.01f,
            this->position.y,
        };
    }
};

int main() {
    App app = App(800, 600, "Testing Window");
    Box* box = new Box(Vec2{-0.5f, -0.5f}, 1.0f, 1.0f, Color{1.0f, 0.0f, 0.0f});

    app.add_system(SystemType::Update, [&box]() {
           box->draw();
       })
        .add_system(SystemType::Update, [&box]() {
            box->glide();
        })
        .run();
}