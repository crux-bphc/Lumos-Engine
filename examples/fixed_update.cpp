#include <iostream>

#include "lumos.h"

void hello_world() {
    spdlog::debug("Hello World");
}

void bye_world() {
    spdlog::debug("Bye World");
}

int main() {
    App app = App();
    app.add_system(SystemType::FixedUpdate, hello_world, 1)
        .add_system(SystemType::FixedUpdate, bye_world, 2)
        .run();
}