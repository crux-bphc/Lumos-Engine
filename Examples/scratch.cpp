#include <iostream>

#include "../Lumos/lumos.cpp"
#define STB_IMAGE_IMPLEMENTATION

// Vertex shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    void main() {
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoord;
    out vec4 FragColor;

    uniform sampler2D texture1;

    void main() {
        FragColor = texture(texture1, TexCoord);
    }
)";

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Texture Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Load Texture Data
    int width, height, nrChannels;
    // todo Just a reminder, this path is relative to the executable, which make no sense ???
    /*
    Certainly, you can pack the image with the executable, which is often a practical approach for distributing applications. To achieve this, you'll need to embed the image as a binary resource within your executable and load it from memory rather than from a file. This approach ensures that the image is always available, regardless of the executable's location.

Here's how you can embed an image as a binary resource and load it using the STB Image library:

1. **Embed the Image as a Binary Resource:**

   You can convert the image file into a C/C++ source file containing the image data as a byte array. There are tools available, such as `xxd` on Unix-like systems, to generate this source file. Here's an example command to convert an image file (`container.jpg`) into a C/C++ source file (`image_data.cpp`):

   ```bash
   xxd -i container.jpg > image_data.cpp
   ```

   This command will create an `image_data.cpp` file containing a byte array that represents the image data.

2. **Include the Embedded Image Data in Your Code:**

   In your C++ code, include the generated `image_data.cpp` file to access the embedded image data.

   ```cpp
   #include "image_data.cpp"
   ```

3. **Load the Image from Memory:**

   Now, you can use the image data directly from the byte array in memory without needing to load it from a file. For example:

   ```cpp
   int width, height, nrChannels;
   unsigned char* data = stbi_load_from_memory(image_data, sizeof(image_data), &width, &height, &nrChannels, 0);
   ```

   This code uses `stbi_load_from_memory` to load the image data from the embedded byte array `image_data`. Ensure that you've included the correct size of the array (`sizeof(image_data)`) in the function call.

By embedding the image as a binary resource, you eliminate the need to worry about the image's location in the final directory, and it becomes part of your executable. Just make sure to keep the embedded image data up to date if you change or update the image.
    */
   // Need to write another program for this ???
    unsigned char* data = stbi_load("../Examples/container.jpg", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        return -1;
    }

    // Generate Texture Object
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set Texture Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load Texture Data into GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the loaded image data
    stbi_image_free(data);

    // Compile Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n"
                  << infoLog << std::endl;
        return -1;
    }

    // Compile Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed:\n"
                  << infoLog << std::endl;
        return -1;
    }

    // Link Shader Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for shader program linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n"
                  << infoLog << std::endl;
        return -1;
    }

    // Set up vertex data and buffers for a rectangle
    float vertices[] = {
        // Positions          // Texture Coordinates
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,    // Top Right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // Bottom Right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // Bottom Left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f    // Top Left
    };
    GLuint indices[] = {
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input events here

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup and exit
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &textureID);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}
