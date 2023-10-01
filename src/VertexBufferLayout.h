#pragma once

#include <vector>
#include <GL/glew.h>

class VertexBufferElement{

    private:

        unsigned int count;
        unsigned int type;
        bool normalized;


    public:

        VertexBufferElement(const unsigned int count, const unsigned int type, const bool normalized);

};

class VertexBufferLayout{

    private:

        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;

    //let's see the video

    public:

        VertexBufferLayout();

        template<typename T>
        void Push(int count, T type){

            norm = false
            if(type == GL_UNSIGNED_INT) norm = true;

            VertexBufferElement vbe(count, type, norm);
            m_Elements.push_back(vbe);

            m_Stride += sizeof(T);
        }

        inline unsigned int GetStride() const 
        { 
            return m_Stride; 
        }

        inline const std::vector<VertexBufferElement> GetElements() 
        { 
            return m_Elements; 
        }

};

