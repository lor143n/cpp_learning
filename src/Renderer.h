#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//file is a macro that returns the infos (e.g. the name) of the source file executed
 
void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);