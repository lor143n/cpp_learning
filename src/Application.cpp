#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


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

        //VERTEX-INDEX DATA START
        float positions[10] = {
            -0.1f, 0.5f,    //0
            1.0f, 1.0f,     //1
            -1.0f, -1.0f,   //2
            0.5f, -0.5f,    //3  
            0.0f, 0.0f      //4
        };

        unsigned int indices[6] = {
            0, 4, 2,
            1, 4, 3
        };
        //VERTEX-INDEX DATA END

        //VERTEX ARRAY - VERTEX BUFFER - INDEX ARRAY START
        VertexBuffer vb(positions, 5 * 2 * sizeof(float));

        VertexArray va;
        VertexBufferLayout layout;
        layout.Push(2, GL_FLOAT);
        va.AddBuffer(vb, layout);
        va.Bind();

        IndexBuffer ib(indices, 6);
        //VERTEX ARRAY - VERTEX BUFFER - INDEX ARRAY END

        //SHADERS START
        Shader myShader("/home/lor3n/dev/cpp_learning/res/shaders/vertex.shader","/home/lor3n/dev/cpp_learning/res/shaders/fragment.shader");
        myShader.Bind();
        myShader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        float r = 0.0f;
        float increment = 0.01f;
        //SHADERS END

        while (!glfwWindowShouldClose(window)) {
            
            glClear(GL_COLOR_BUFFER_BIT);
            
            //BINDING AND CHANGING SHADERS
            va.Bind();
            ib.Bind();
            myShader.Bind();

            if(r > 1.0f){
                increment = -0.01f;
            } else if (r < 0.0f){
                increment = 0.01f;
            }

            r += increment;
            myShader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            //RENDERING
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            

            // Swap dei buffer
            glfwSwapBuffers(window);

            glfwPollEvents();
        }

    }


    glfwTerminate();

    return 0;
}