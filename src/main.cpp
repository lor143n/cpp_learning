#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>


struct Shaders{

    std::string vertexShader;
    std::string fragmentShader;
};


Shaders OpenAndReadShaders(const std::string& vertexPath, const std::string& fragmentPath){

    std::string vertexShader;
    std::string fragmentShader;

    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);

    if(!vertexFile.is_open()){

        std::cout << "wrong path!";
    }

    vertexShader.assign(
        (std::istreambuf_iterator<char>(vertexFile)),
        (std::istreambuf_iterator<char>())
    );

    fragmentShader.assign(
        (std::istreambuf_iterator<char>(fragmentFile)),
        (std::istreambuf_iterator<char>())
    );

    return {vertexShader, fragmentShader};
}


static unsigned int CompileShader(unsigned int type, const std::string& source){

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // error handling
    int res;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);
    if(!res){ //you can use GL_FALSE

        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length*sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl; 
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }


    return id;
}


static unsigned int CreateShader(const std::string& vertexShader,const std::string& fragShader){

    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}



int main(){

    // Inizializzazione di GLFW
    int ret = glfwInit();
    if (!ret) {
        std::cerr << "Error: can't initialize GLFW" << std::endl;
        return -1;
    }

    // Crea una finestra GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "Colored Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error: can't create the window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Fai in modo che il contesto OpenGL creato dalla finestra sia il contesto corrente
    glfwMakeContextCurrent(window);

    // Inizializza GLEW per caricare le estensioni OpenGL
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: can't initialize GLEW" << std::endl;
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
    Creazione di un buffer (blocco di memoria riservata), selezione del buffer da usare (binding), allocazione dei vertex
    */

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);


    //Every attribute has to be enabled (OpenGL works has a state machine so not matter the order)
    glEnableVertexAttribArray(0);
    //Every vertex has a bunch of attributes, in this case we only the position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    Shaders myShaders = OpenAndReadShaders("/home/lor3n/dev/cpp_learning/res/shaders/vertex.shader","/home/lor3n/dev/cpp_learning/res/shaders/fragment.shader");
    
    unsigned int shaders = CreateShader(myShaders.vertexShader, myShaders.fragmentShader);
    glUseProgram(shaders);

    // Ciclo principale della finestra
    while (!glfwWindowShouldClose(window)) {
        
        glClear(GL_COLOR_BUFFER_BIT);

        //questa e' la draw call del buffer selezionato (binded)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap dei buffer
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shaders);

    // Termina GLFW
    glfwTerminate();

    return 0;
}