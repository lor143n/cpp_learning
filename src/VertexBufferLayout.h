#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

class VertexBufferElement{

    public: 
        unsigned int count;
        unsigned int type;
        unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type){

        switch(type){
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout{

    private:

        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;

    //let's see the video

    public:

        VertexBufferLayout() : m_Stride(0) {};

        template<typename T>
        void Push(unsigned int count, T type){

            bool norm = GL_FALSE;
            if(type == GL_UNSIGNED_INT){
                norm = GL_TRUE;
            } 

            VertexBufferElement vbe({ count, type, norm });
            m_Elements.push_back(vbe);

            m_Stride += VertexBufferElement::GetSizeOfType(type) * count;
        }

        inline unsigned int GetStride() const 
        { 
            return m_Stride; 
        }

        inline const std::vector<VertexBufferElement>& GetElements() const
        { 
            return m_Elements; 
        }

};

