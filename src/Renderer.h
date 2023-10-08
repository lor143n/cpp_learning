#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//file is a macro that returns the infos (e.g. the name) of the source file executed
 
void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer{

    private:


    public:
        void Clear() const;
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& sh) const;


};