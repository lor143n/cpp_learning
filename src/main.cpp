#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Vertex Shader sorgente
const char* vertexSource = R"(
    #version 330 core
    in vec2 position;
    in vec3 color;
    out vec3 Color;
    void main() {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

// Fragment Shader sorgente
const char* fragmentSource = R"(
    #version 330 core
    in vec3 Color;
    out vec4 outColor;
    void main() {
        outColor = vec4(Color, 1.0);
    }
)";

int main(){

    // Inizializzazione di GLFW
    if (!glfwInit()) {
        std::cerr << "Errore: impossibile inizializzare GLFW" << std::endl;
        return -1;
    }

    // Configura GLFW per utilizzare il profilo OpenGL core (senza compatibilità con versioni precedenti)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // Definisci i vertici del triangolo e i relativi colori
    float vertices[] = {
        // Posizione           // Colore
        0.0f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f, // Vertice superiore
       -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f, // Vertice in basso a sinistra
        0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f  // Vertice in basso a destra
    };

    // Crea un Vertex Array Object (VAO) e un Vertex Buffer Object (VBO)
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Associa il VAO
    glBindVertexArray(VAO);

    // Carica i dati dei vertici nel VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specifica il layout degli attributi dei vertici
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Crea e compila il Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    // Verifica gli errori di compilazione del Vertex Shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Errore nella compilazione del Vertex Shader: " << infoLog << std::endl;
    }

    // Crea e compila il Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // Verifica gli errori di compilazione del Fragment Shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Errore nella compilazione del Fragment Shader: " << infoLog << std::endl;
    }

    // Crea un Program Object e collega il Vertex e il Fragment Shader
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Verifica gli errori nella creazione del Program Object
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Errore nella creazione del Program Object: " << infoLog << std::endl;
    }

    // Cancella gli Shader dopo aver creato il Program Object
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Ciclo principale della finestra
    while (!glfwWindowShouldClose(window)) {
        // Gestione degli eventi
        glfwPollEvents();

        // Sfondo colorato
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Usa il Program Object
        glUseProgram(shaderProgram);

        // Disegna il triangolo
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap dei buffer
        glfwSwapBuffers(window);
    }

    // Pulizia delle risorse
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Termina GLFW
    glfwTerminate();

    return 0;
}