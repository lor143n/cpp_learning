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

    //Vertex che vogliamo memorizzare ed utilizzare

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f, 0.5f,
         0.5f, -0.5f
    };

    /*
    Creazione di un buffer (blocco di memoria riservata), selezione del buffer da usare (binding), allocazione dei dati
    */

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);

    //Every attribute has to be enabled (OpenGL works has a state machine so not matter the order)
    glEnableVertexAttribArray(0);
    //Every vertex has a bunch of attributes, in this case we only the position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    // Ciclo principale della finestra
    while (!glfwWindowShouldClose(window)) {
        
        glClear(GL_COLOR_BUFFER_BIT);

        //questa e' la draw call del buffer selezionato (binded)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap dei buffer
        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    // Termina GLFW
    glfwTerminate();

    return 0;
}