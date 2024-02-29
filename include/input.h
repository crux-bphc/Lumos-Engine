#include <GLFW/glfw3.h>

struct inputPos {
    double x = 0;
    double y = 0;
};

class input {
  public:
    GLFWwindow* window = nullptr;
    static inputPos mousePos;
    static inputPos scrollPos;
    int width;
    int height;
    input(GLFWwindow* window);
    ~input();

    //Keyboard Handling
    bool isKeyPressed(int key);

    //Mouse Handling
    static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    bool isMousePressed(int button);
    inputPos getMousePosition();

    //Scroll Callback
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
   


};
