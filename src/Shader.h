#pragma once

#include <string>

struct ShadersText{
    std::string vertexShader;
    std::string fragmentShader;
};

class Shader{


    private:
        std::string m_FilePathVertex;
        std::string m_FilePathFragment;

    public:
        unsigned int m_RendererID;

        Shader(const std::string& filepathFragment, const std::string& filepathVertex);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        //Set uniforms
        void SetUniform4f(const std::string name, float v0, float v1, float v2, float v3);


    private:

        int GetUniformLocation(const std::string& name);
        ShadersText OpenAndReadShaders(const std::string& vertexPath, const std::string& fragmentPath);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateShader(const std::string& vertex,const std::string& fragment);


};