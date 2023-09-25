#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
 
static void GLClearError(){


    while(glGetError() != GL_NO_ERROR);

}

static bool GLLogCall(const char* function, const char* file, int line){

    while(GLenum error = glGetError()){
        std::cout <<"[OpenGL Error] (" << error << "): " << function << " " <<file << ": " <<line << std::endl;
        return false;
    }
    return true;

}


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

    glfwSwapInterval(1);

    // Inizializza GLEW per caricare le estensioni OpenGL
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: can't initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    //BUFFER START

    float positions[8] = {
        -0.5f, -0.5f, //0
         0.5f, -0.5f, //1
        0.5f, 0.5f,   //2
         -0.5f, 0.5f, //3  
    };

    unsigned int indices[12] = {
        0, 1, 2,
        2, 3, 0,
    };

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*2*sizeof(unsigned int), indices, GL_STATIC_DRAW));

    //BUFFER END

    Shaders myShaders = OpenAndReadShaders("/home/lor3n/dev/cpp_learning/res/shaders/vertex.shader","/home/lor3n/dev/cpp_learning/res/shaders/fragment.shader");
    
    unsigned int shaders = CreateShader(myShaders.vertexShader, myShaders.fragmentShader);
    GLCall(glUseProgram(shaders));

    int location = glGetUniformLocation(shaders, "u_Color");
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));
    
    float r = 0.0f;
    float increment = 0.01f;
    while (!glfwWindowShouldClose(window)) {
        
        glClear(GL_COLOR_BUFFER_BIT);

        if(r > 1.0f){
            increment = -0.01f;
        } else if (r < 0.0f){
            increment = 0.01f;
        }

        r += increment;

        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        // Swap dei buffer
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shaders);

    // Termina GLFW
    glfwTerminate();

    return 0;
}