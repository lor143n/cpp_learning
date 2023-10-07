#include "Shader.h"

#include <iostream>
#include <fstream>

#include "Renderer.h"
#include <GL/glew.h>



Shader::Shader(const std::string& filepathVertex, const std::string& filepathFragment) 
        : m_FilePathVertex(filepathVertex), m_FilePathFragment(filepathFragment),m_RendererID(0){

    ShadersText shs = OpenAndReadShaders(filepathVertex, filepathFragment);
    m_RendererID = CreateShader(shs.vertexShader, shs.fragmentShader);
}


Shader::~Shader(){

    GLCall(glDeleteProgram(m_RendererID));

}

void Shader::Bind() const{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3){

    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name){

    
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    ASSERT(location != -1);
    return location;
    
}


unsigned int Shader::CreateShader(const std::string& vertex,const std::string& fragment){

    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

ShadersText Shader::OpenAndReadShaders(const std::string& vertexPath, const std::string& fragmentPath){

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


unsigned int Shader::CompileShader(unsigned int type, const std::string& source){

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
