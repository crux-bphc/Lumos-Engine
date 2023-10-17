#include "lumos.h"

int WINDOW_HEIGHT;
int WINDOW_WIDTH;
std::vector<std::thread> App::fixed_update_threads;


void App::create_window() {
    if (this->headless) {
        spdlog::debug("Creating headless window");
        this->window = nullptr;
        return;
    }

    spdlog::debug("Creating window");

    // Initialize GLFW
    if (!glfwInit()) {
        spdlog::error("Failed to initialize GLFW");
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, this->window_title, nullptr, nullptr);
    this->window = window;
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, this->resizable);

    if (!window) {
        spdlog::error("Failed to create GLFW window");
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW (or other OpenGL loader) after creating the context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "GLEW error: %s\n", glewGetErrorString(err));
        glfwTerminate();
        // Handle GLEW initialization failure
    }
}
App::App(int window_width, int window_height, const char* window_title, bool resizable, bool debug) {
    spdlog::info("Starting Lumos Engine 🌕");
    if (debug) {
        spdlog::set_level(spdlog::level::debug);
    }
    WINDOW_WIDTH = window_width;
    WINDOW_HEIGHT = window_height;
    this->window_title = window_title;
    this->resizable = resizable;
}

// Opens app in headless mode
App::App(bool debug) {
    spdlog::info("Starting Lumos Engine 🌕");
    if (debug) {
        spdlog::set_level(spdlog::level::debug);
    }
    this->headless = true;
}

App::~App() {
    spdlog::info("Closing Lumos Engine 🌑");
}

App& App::add_system(SystemType type, std::function<void()> function) {
    std::string system_type;
    switch (type) {
        case SystemType::Startup:
            this->startup_functions.push_back(function);
            system_type = "Startup";
            break;
        case SystemType::Update:
            this->update_functions.push_back(function);
            system_type = "Update";
            break;
        default:
            break;
    }
    spdlog::debug("Adding system of type {}", system_type);
    return *this;
}

App& App::add_system(SystemType type, std::function<void()> function, int seconds) {
    std::string system_type;

    switch (type) {
        case SystemType::FixedUpdate:
            this->fixed_update_functions.push_back({function, seconds});
            system_type = "FixedUpdate";
            break;
        default:
            break;
    }
    spdlog::debug("Adding system of type {}", system_type);
    return *this;
}

void App::run() {
    this->create_window();

    if (!this->headless) {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    spdlog::debug("Running the startup functions");
    // Functions run in the beginning
    for (const std::function<void()>& function : startup_functions) {
        function();
    }
    
    spdlog::debug("Running the fixed update functions");
    // Fixed update functions
    for (const std::pair<std::function<void()>, int>& function_pair : fixed_update_functions) {
        const std::function<void()>& function = function_pair.first;
        int seconds = function_pair.second;

        // Create and start a thread for the fixed update function
        fixed_update_threads.emplace_back([function, seconds]() {
            while (true) {
                function();
                std::this_thread::sleep_for(std::chrono::seconds(seconds));
            }
        });
    }
    
    spdlog::debug("Running the main loop (contains update function loop)");
    if (!this->headless) {
        // Main loop
        while (!glfwWindowShouldClose(this->window)) {
            glClear(GL_COLOR_BUFFER_BIT);

            // Update Functions (update every frame i.e every loop)
            for (std::function<void()>& function : update_functions) {
                function();
            }

            // Swap front and back buffers
            glfwSwapBuffers(this->window);

            // Poll for and process events
            glfwPollEvents();
        }
    }
    
    
    spdlog::debug("Terminating the fixed update functions");
    // Set the termination flag for fixed update threads
    for (auto& thread : fixed_update_threads) {
        if (thread.joinable()) {
            thread.join();  // Wait for each fixed update thread to finish
        }
    }
    

    // Clean up GLFW
    glfwTerminate();
}
