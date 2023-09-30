#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

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


    //we create a scope to fix the termination issue due by the glGetError function

    {

        //VERTEX INDEX START

        float positions[10] = {
            -1.0f, 1.0f,    //0
            1.0f, 1.0f,     //1
            -1.0f, -1.0f,   //2
            1.0f, -1.0f,    //3  
            0.0f, 0.0f      //4
        };

        unsigned int indices[6] = {
            0, 4, 2,
            1, 4, 3
        };

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexBuffer vb(positions, 5 * 2* sizeof(float));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

        IndexBuffer ib(indices, 6);

        //VERTEX INDEX START

        //SHADERS START

        Shaders myShaders = OpenAndReadShaders("/home/lor3n/dev/cpp_learning/res/shaders/vertex.shader","/home/lor3n/dev/cpp_learning/res/shaders/fragment.shader");
        
        unsigned int shaders = CreateShader(myShaders.vertexShader, myShaders.fragmentShader);
        GLCall(glUseProgram(shaders));

        int location = glGetUniformLocation(shaders, "u_Color");
        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

        
        float r = 0.0f;
        float increment = 0.01f;

        //SHADERS END

        while (!glfwWindowShouldClose(window)) {
            
            glClear(GL_COLOR_BUFFER_BIT);

            //GLCall(glUseProgram(shaders));
            //GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            //GLCall(glBindVertexArray(vao));

            //ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if(r > 1.0f){
                increment = -0.01f;
            } else if (r < 0.0f){
                increment = 0.01f;
            }

            r += increment;

            // Swap dei buffer
            glfwSwapBuffers(window);

            glfwPollEvents();
        }

        glDeleteProgram(shaders);

    }


    glfwTerminate();

    return 0;
}