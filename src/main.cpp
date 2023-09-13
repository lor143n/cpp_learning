#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(){

    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }


    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
        std::cout << "Error"<< std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up and terminate GLFW
    glfwTerminate();

    return 0;
}