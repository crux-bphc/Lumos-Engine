#include "lumos.h"


std::vector<std::thread> App::fixed_update_threads;
int WINDOW_HEIGHT;
int WINDOW_WIDTH;

void App::create_window() {
    if (this->headless) {
        spdlog::debug("Creating headless window");
        this->window = nullptr;
        return;
    }

    spdlog::trace("Creating window");

    // Initialize GLFW
    if (!glfwInit()) {
        spdlog::error("Failed to initialize GLFW");
    }

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef DEBUG

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    #endif

    

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          this->window_title, nullptr, nullptr);
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
        //spdlog::error("GLEW error: {}", glewGetErrorString(err));
        glfwTerminate();
        // Handle GLEW initialization failure
    }


    #ifdef DEBUG

        int context_flag = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &context_flag);
    
        if (context_flag & GL_CONTEXT_FLAG_DEBUG_BIT) {
            spdlog::info("Debug context created");
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, NULL);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        }
        else {
            spdlog::error("Unable to create debug context");
        }
    
    #endif
    this->Input = new input(window);
    

    //spdlog::info("Using OpenGL version {}, and C++ version {} ",
    //             glGetString(GL_VERSION), __cplusplus);
}

App::App(int window_width, int window_height, const char* window_title,
         bool resizable, bool debug) {
    spdlog::info("Starting Lumos Engine 🌕");
    if (debug) {
        spdlog::set_level(spdlog::level::debug);
    }
    WINDOW_WIDTH = window_width;
    WINDOW_HEIGHT = window_height;
    this->window_title = window_title;
    this->resizable = resizable;
    this->reg = entt::basic_registry();

    this->create_window();
    #ifndef LEGACY
    this->rend = new renderer();
    #endif
}

// Opens app in headless mode
App::App(bool debug) {
    spdlog::info("Starting Lumos Engine 🌕");
    if (debug) {
        spdlog::set_level(spdlog::level::debug);
    }
    this->headless = true;

    this->create_window();
}

App::~App() { spdlog::info("Closing Lumos Engine 🌑"); }

void App::SetCameraInput(float arr[2]){
    this->movementArray[0] = arr[0];
    this->movementArray[1] = arr[1];
}

App& App::add_startup_system(std::function<void(App&)> function) {
    this->startup_functions.push_back(function);
    return *this;
}

App& App::add_update_system(std::function<void(App&)> function) {
    this->update_functions.push_back(function);
    return *this;
}

App& App::add_fixed_update_system(std::function<void(App&)> function,
                                  int milliseconds) {
    this->fixed_update_functions.push_back({function, milliseconds});
    return *this;
}

/*
App& App::add_key_callback_system(
    std::function<void(int, int, int, int)> function) {
    this->key_callback_functions.push_back(function);
    return *this;
}

App& App::add_mouse_callback_system(std::function<void(int, int)> function) {
    this->mouse_callback_functions.push_back(function);
    return *this;
}

App& App::add_scroll_callback_system(
    std::function<void(int, int, int)> function) {
    this->scroll_callback_functions.push_back(function);
    return *this;
}
*/


void App::close() {
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
    glfwTerminate();
    running = false;
}

/*
std::pair<double, double> App::get_mouse_position() {
    double xpos, ypos;
    glfwGetCursorPos(this->window, &xpos, &ypos);
    ypos = WINDOW_WIDTH - ypos;
    return {xpos, ypos};
}

bool App::is_mouse_pressed() {
    spdlog::warn("is_mouse_pressed is not working as intended");
    return this->__is_mouse_pressed;
}
*/

void App::run() {
    //this->create_window();

    if (!this->headless) {
        glClear(GL_COLOR_BUFFER_BIT);
    }


    /*
    glfwSetWindowUserPointer(this->window, this);

    
    spdlog::info("Registering key callback functions");
    glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key,
                                        int scancode, int action, int mods) {
        spdlog::trace("Key callback function called");
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        for (std::function<void(int, int, int, int)>& function :
             app->key_callback_functions) {
            function(key, scancode, action, mods);
        }
    });

    spdlog::info("Registering mouse callback functions");
    glfwSetMouseButtonCallback(
        this->window, [](GLFWwindow* window, int button, int action, int mods) {
            spdlog::trace("Mouse callback function called");
            App* app = static_cast<App*>(glfwGetWindowUserPointer(window));

            app->__is_mouse_pressed = (action == GLFW_PRESS);

            for (std::function<void(int, int)>& function :
                 app->mouse_callback_functions) {
                function(button, action);
            }
        });

    spdlog::info("Registering scroll callback functions");
    glfwSetScrollCallback(
        this->window, [](GLFWwindow* window, double xoffset, double yoffset) {
            spdlog::trace("Scroll callback function called");
            App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
            for (std::function<void(int, int, int)>& function :
                 app->scroll_callback_functions) {
                function(xoffset, yoffset, 0);
            }
        });
    */

    

    spdlog::info("Running the startup functions");
    for (const std::function<void(App&)>& function : startup_functions) {
        function(*this);
    }

    spdlog::info("Running the fixed update functions");
    for (const std::pair<std::function<void(App&)>, int>& function_pair :
         fixed_update_functions) {
        const std::function<void(App&)>& function = function_pair.first;
        int milliseconds = function_pair.second;

        // Create and start a thread for the fixed update function
        fixed_update_threads.emplace_back([&]() {
            while (!glfwWindowShouldClose(this->window)) {
                function(*this);
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(milliseconds));
            }
        });
    }

    spdlog::info("Running the main loop");
    if (!this->headless) {
        // Main loop
        running = true;

        while (!glfwWindowShouldClose(this->window) && running) {

            // Poll for and process events
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT);
            // Update Functions (update every frame i.e every loop)
            for (std::function<void(App&)>& function : update_functions) {
                function(*this);
            }
            #ifndef LEGACY
            draw();
            #endif
            // Swap front and back buffers
            glfwSwapBuffers(this->window);
            glFrameTerminatorGREMEDY();
        }
    }

    spdlog::info("Terminating the fixed update functions");
    // Set the termination flag for fixed update threads
    for (auto& thread : fixed_update_threads) {
        if (thread.joinable()) {
            thread.join();  // Wait for each fixed update thread to finish
        }
    }

    spdlog::info("Cleaning up opengl");
    // Clean up GLFW
    this->close();
}

entt::entity sprite2D(App& app, rect& rect_in, Texture& texture, bool isShown) {

    //renderer& renderer_in = rend;
    entt::registry& reg = app.reg;
    entt::entity s2Dentt = reg.create();
    reg.emplace<position>(s2Dentt, rect_in.pos);
    reg.emplace<dimention>(s2Dentt, rect_in.dim);
    reg.emplace<colour>(s2Dentt, rect_in.col);
    reg.emplace<isDrawn>(s2Dentt, isShown);
    reg.emplace<textureIndex>(s2Dentt, (GLint)texture.texIndex);
    reg.emplace<isUpdated>(s2Dentt, true);
    //spdlog::info("Adding Sprite2D entity");

    return s2Dentt;
}

entt::entity sprite2D(App& app, rect& rect_in, bool isShown) {

    entt::registry& reg = app.reg;
    entt::entity s2Dentt = reg.create();
    reg.emplace<position>(s2Dentt, rect_in.pos);
    reg.emplace<dimention>(s2Dentt, rect_in.dim);
    reg.emplace<colour>(s2Dentt, rect_in.col);
    reg.emplace<isDrawn>(s2Dentt, isShown);
    reg.emplace<textureIndex>(s2Dentt, (GLint)-1);
    reg.emplace<isUpdated>(s2Dentt, true);
    //spdlog::info("Adding Sprite2D entity");

    return s2Dentt;
}

App& App::draw() {
    //spdlog::info("Draw is Called");
    auto view = reg.view<isDrawn, position, dimention, colour, textureIndex>(); 
    for(auto entity: view) {
        auto isDrawn_m = view.get<isDrawn>(entity);
        if (isDrawn_m.draw) {
            auto [dimention_m, colour_m, textureIndex_m] = view.get<position, dimention, colour, textureIndex>(entity);
            auto& position_m = view.get<position>(entity)
            position_m.pos.x += (movementArray[0]); 
            position_m.pos.y += (movementArray[1]);
            glm::vec2 pos = { (position_m.pos.x - 0.5f) * 2.0f, (position_m.pos.y - 0.5f) * 2.0f};
            glm::vec2 dim = { (dimention_m.dim.x * 2.0f), (dimention_m.dim.y * 2.0f)};
            glm::vec3 col = colour_m.colour;
            GLint texIndex = textureIndex_m.texIndex;
            rend->drawQuad(pos, dim, col, texIndex);
            //spdlog::debug("rend pos: {}, {}", pos.x, pos.y);
            //printf("updated vbo into renderer at %d\n", rend->vbo_pos);
        }
    }
    rend->draw();
    return *this;

}

/*
void updateRectS2D(glm::vec2 pos, glm::vec2 dim, float angle, vertTexQuad& vertQuad_in) {
    angle = angle + 1;
    struct vertTexQuad vertQuad;
    vertQuad.vertices[0].position = {pos.x, pos.y, 0.0f};
    vertQuad.vertices[1].position = {pos.x + dim.x, pos.y, 0.0f};
    vertQuad.vertices[2].position = {pos.x + dim.x, pos.y + dim.y,0.0f};
    vertQuad.vertices[3].position = {pos.x, pos.y + dim.y, 0.0f};
    vertQuad.vertices[0].texCoord = {0.0f, 0.0f};
    vertQuad.vertices[1].texCoord = {1.0f, 0.0f};
    vertQuad.vertices[2].texCoord = {1.0f, 1.0f};
    vertQuad.vertices[3].texCoord = {0.0f, 1.0f};

    vertQuad_in = vertQuad;
    
}
*/


void glDebugOutput(GLenum source, GLenum type, 
                    unsigned int id, GLenum severity, 
                    GLsizei length, const char *message, 
                    const void *userParam) {

    switch (type) {
        case GL_DEBUG_TYPE_ERROR: 
            spdlog::error("OpenGL Error Message ID {0}: {1}", id, message);
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            spdlog::debug("OpenGL Debug Message (Deprecated Behaviour) ID {0}:, {1}", id, message);
            break;
        default:
            spdlog::debug("OpenGL Debug Message ID {0}: {1}", id, message);

    }
}
