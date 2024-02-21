#include <iostream>

#include "lumos.h"

void hello_world() { spdlog::debug("Hello World"); }

void bye_world() { spdlog::debug("Bye World"); }

int main() {
    App app = App();
    app.add_fixed_update_system(hello_world, 100)
        .add_fixed_update_system(bye_world, 200)
        .run();
}