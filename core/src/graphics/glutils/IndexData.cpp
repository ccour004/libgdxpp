#include "IndexData.h"

void IndexData::createBufferObject(){
    glGenBuffers(1,&bufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.capacity(), NULL, usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexData::IndexData(int type,bool isStatic,const std::vector<GLuint>& data){
    isDirty = false;isBound = false;isDirect = false;
    this->type = INDEX_BUFFER_OBJECT;
    usage = isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
    buffer = data;
    isDirect = true;
    glGenBuffers(1, &bufferHandle);
}
IndexData::IndexData (int type,int maxIndices):IndexData(type,true,maxIndices){}
IndexData::IndexData (int type,bool isStatic, int maxIndices){
        isDirty = false;isBound = false;isDirect = false;
        this->type = type;
        usage = isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
        
        switch(type){
            case INDEX_ARRAY: 
                buffer = std::vector<GLuint>(maxIndices); 
            break;
            case INDEX_BUFFER_OBJECT:
                buffer = std::vector<GLuint>(maxIndices * 2);
                isDirect = true;
                glGenBuffers(1, &bufferHandle);
            break;
            case INDEX_BUFFER_OBJECT_SUB_DATA:
                isDirect = true;
                buffer = std::vector<GLuint>();
                createBufferObject();
            break;
        }
}

std::vector<GLuint>& IndexData::getBuffer() {isDirty = true; return buffer;}

void IndexData::setIndices (const std::vector<GLuint>& indices, int offset, int count){
        int counter = 0;
        for(int i = offset;i < offset+count;i++)
            buffer[counter++] = indices[i];
        setIndices(offset,count);
}

void IndexData::setIndices (int offset,int count){
    isDirty = true;
    switch(type){
        case INDEX_ARRAY:
            buffer = std::vector<GLuint>(offset,offset+count);
        break;
        case INDEX_BUFFER_OBJECT:
            if (isBound) {
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(GLuint), buffer.data(), usage);
                isDirty = false;
            }
        break;
        case INDEX_BUFFER_OBJECT_SUB_DATA:
            if (isBound) {
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, buffer.size() * sizeof(GLuint), buffer.data());
                isDirty = false;
            }
        break;
    }
}

void IndexData::updateIndices (int targetOffset, const std::vector<GLuint>& indices, int offset, int count){
    isDirty = true;
    for(int i = offset;i < offset+count;i++)
            buffer[targetOffset++] = indices[i];
    switch(type){
        case INDEX_BUFFER_OBJECT:
            if (isBound) {
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(GLuint), buffer.data(), usage);
                isDirty = false;
            }
        break;
        case INDEX_BUFFER_OBJECT_SUB_DATA:
            if (isBound) {
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, buffer.size() * sizeof(GLuint), buffer.data());
                isDirty = false;
            }
        break;
    }
}

void IndexData::invalidate(){
    isDirty = true;
    switch(type){
        case INDEX_BUFFER_OBJECT:
            glGenBuffers(1,&bufferHandle);
        break;
        case INDEX_BUFFER_OBJECT_SUB_DATA:
            createBufferObject();
        break;
    }
}

void IndexData::bind(){
    switch(type){
        case INDEX_BUFFER_OBJECT:
            if (bufferHandle == 0) SDL_Log("No buffer allocated!");

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferHandle);
            if (isDirty) {
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(GLuint), buffer.data(), usage);
                isDirty = false;
            }
            isBound = true;
        break;
        case INDEX_BUFFER_OBJECT_SUB_DATA:
            if (bufferHandle == 0) SDL_Log("IndexBufferObject cannot be used after it has been disposed.");

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferHandle);
            if (isDirty) {
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, buffer.size() * sizeof(GLuint), buffer.data());
                isDirty = false;
            }
            isBound = true;
        break;
    }
}

void IndexData::unbind(){
    if(type != INDEX_ARRAY){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		isBound = false;
    }
}

IndexData::~IndexData(){
    if(type != INDEX_ARRAY){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1,&bufferHandle);
        bufferHandle = 0;
    }
}

