#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>



int main(){

    // Inizializzazione di GLFW
    if (!glfwInit()) {
        std::cerr << "Errore: impossibile inizializzare GLFW" << std::endl;
        return -1;
    }

    // Crea una finestra GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangolo Colorato", nullptr, nullptr);
    if (!window) {
        std::cerr << "Errore: impossibile creare una finestra GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Fai in modo che il contesto OpenGL creato dalla finestra sia il contesto corrente
    glfwMakeContextCurrent(window);

    // Inizializza GLEW per caricare le estensioni OpenGL
    if (glewInit() != GLEW_OK) {
        std::cerr << "Errore: impossibile inizializzare GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    
    glGenBuffers(1, )


    // Ciclo principale della finestra
    while (!glfwWindowShouldClose(window)) {
        



        // Swap dei buffer
        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    // Termina GLFW
    glfwTerminate();

    return 0;
}