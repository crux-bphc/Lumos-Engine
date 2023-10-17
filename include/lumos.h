#pragma once
#include <spdlog/spdlog.h>

#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include "data.h"
#include "shapes.h"

enum SystemType {
    Startup,
    Update,
    FixedUpdate
};

class App {
private:
    const char* window_title;
    GLFWwindow* window;
    std::vector<std::function<void()>> startup_functions;
    std::vector<std::function<void()>> update_functions;
    std::vector<std::pair<std::function<void()>, int>> fixed_update_functions;
    static std::vector<std::thread> fixed_update_threads;
    bool resizable;
    bool headless = false;
    void create_window();
public:
    App(int window_width, int window_height, const char* window_title, bool resizable = false, bool debug = true);
    App(bool debug = true);
    ~App();
    App& add_system(SystemType type, std::function<void()> function);
    App& add_system(SystemType type, std::function<void()> function, int seconds);
    void run();
};