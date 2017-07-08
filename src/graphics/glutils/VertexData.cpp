#include "VertexData.h"
#include "../VertexAttribute.h"

VertexData::VertexData (int type,int numVertices,const std::vector<VertexAttribute>& attributes):
		VertexData(type,true, numVertices,VertexAttributes(attributes)){
}

VertexData::VertexData (int type,bool isStatic, int numVertices, const VertexAttributes& attributes) {
        this->type = type;
		this->isStatic = isStatic;
		this->attributes = attributes;
        buffer = std::vector<GLfloat>(attributes.vertexSize * numVertices);
        usage = isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
        
        switch(type){
            case VERTEX_BUFFER_OBJECT:
                glGenBuffers(1,&bufferHandle);
                setBuffer(buffer, true, attributes);
            break;
            case VERTEX_BUFFER_OBJECT_WITH_VAO:
                glGenBuffers(1,&bufferHandle);
                glGenVertexArrays(1,&vaoHandle);
            break;
            case VERTEX_BUFFER_OBJECT_SUB_DATA:
                isDirect = true;
            break;
        }
	}

void VertexData::bindAttributes (ShaderProgram& shader,const std::vector<int>&  locations) {
		bool stillValid = this->cachedLocations.size() != 0;
		const int numAttributes = attributes.size();

		if (stillValid) {
			if (locations.size() == 0) {
				for (int i = 0; stillValid && i < numAttributes; i++) {
					VertexAttribute attribute = attributes.get(i);
					int location = shader.getAttributeLocation(attribute.alias);
					stillValid = location == this->cachedLocations[i];
				}
			} else {
				stillValid = locations.size() == this->cachedLocations.size();
				for (int i = 0; stillValid && i < numAttributes; i++) {
					stillValid = locations[i] == this->cachedLocations[i];
				}
			}
		}

		if (!stillValid) {
			glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
			unbindAttributes(shader);
			this->cachedLocations.clear();

			for (int i = 0; i < numAttributes; i++) {
				VertexAttribute attribute = attributes.get(i);
				if (locations.size() == 0) {
					this->cachedLocations.push_back(shader.getAttributeLocation(attribute.alias));
				} else {
					this->cachedLocations.push_back(locations[i]);
				}

				int location = this->cachedLocations[i];
				if (location < 0) continue;

				shader.enableVertexAttribute(location);
				shader.setVertexAttribute(location, attribute.numComponents, attribute.type, attribute.normalized, attributes.vertexSize,
                    attribute.offset);
			}
		}
}

void VertexData::setVertices (const std::vector<GLfloat>& vertices, int offset, int count){
    isDirty = true;
    int counter = 0;
    switch(type){
        case VERTEX_ARRAY:
            buffer = std::vector<GLfloat>(vertices[offset],vertices[offset+count]);
        break;
        case VERTEX_BUFFER_OBJECT:
            for(int i = offset;i < offset+count;i++) buffer[counter++] = vertices[i];
        break;
        case VERTEX_BUFFER_OBJECT_WITH_VAO:case VERTEX_BUFFER_OBJECT_SUB_DATA:
            for(int i = offset;i < offset+count;i++) buffer[counter++] = vertices[i];
            bufferChanged();
        break;
    }
}

void VertexData::updateVertices (int targetOffset,const std::vector<GLfloat>& vertices, int sourceOffset, int count){
    isDirty = true;
    switch(type){
        case VERTEX_ARRAY: case VERTEX_BUFFER_OBJECT:
            for(int i = sourceOffset;i < sourceOffset+count;i++) buffer[targetOffset++] = vertices[i];
        break;
        case VERTEX_BUFFER_OBJECT_WITH_VAO:
            for(int i = sourceOffset;i < sourceOffset+count;i++) buffer[targetOffset++] = vertices[i];
            bufferChanged();
        break;
        case VERTEX_BUFFER_OBJECT_SUB_DATA:
            if (isDirect) {
                for(int i = sourceOffset;i < sourceOffset+count;i++)
                    buffer[targetOffset++] = vertices[i];
            } else SDL_Log("Buffer must be allocated direct."); // Should never happen
            bufferChanged();
        break;
    }
}

void VertexData::setAllVertexAttributes(ShaderProgram& shader,const std::vector<int>& locations){
    const int numAttributes = attributes.size();
    for (int i = 0; i < numAttributes; i++) {
        VertexAttribute attribute = attributes.get(i);
        const int location = locations.size() == 0 ? shader.getAttributeLocation(attribute.alias):locations[i];
        if (location < 0) continue;
        shader.enableVertexAttribute(location);
        shader.setVertexAttribute(location, attribute.numComponents, attribute.type, attribute.normalized, attributes.vertexSize,
            attribute.offset);
    }
}

void VertexData::disableAllVertexAttributes(ShaderProgram& shader,const std::vector<int>& locations){
    const int numAttributes = attributes.size();
    if (locations.size() == 0) {
        for (int i = 0; i < numAttributes; i++) {
            shader.disableVertexAttribute(attributes.get(i).alias);
        }
    } else {
        for (int i = 0; i < numAttributes; i++) {
            const int location = locations[i];
            if (location >= 0) shader.disableVertexAttribute(location);
        }
    }    
}

void VertexData::bind (ShaderProgram& shader,const std::vector<int>& locations){
    switch(type){
        case VERTEX_ARRAY:
            setAllVertexAttributes(shader,locations);
        break;
        case VERTEX_BUFFER_OBJECT:
            glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
            if (isDirty) {
                glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(GLfloat), buffer.data(), usage);
                isDirty = false;
            }
            setAllVertexAttributes(shader,locations);
        break;
        case VERTEX_BUFFER_OBJECT_WITH_VAO:
            glBindVertexArray(vaoHandle);
            bindAttributes(shader, locations);
            if (isDirty) {
                glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
                glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(GLfloat), buffer.data(), usage);
                isDirty = false;
            }
        break;
        case VERTEX_BUFFER_OBJECT_SUB_DATA:
            glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
            if (isDirty) {
                glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(GLfloat), buffer.data(), usage);
                isDirty = false;
            }
            setAllVertexAttributes(shader,locations);
        break;
    }
    isBound = true;
}

void VertexData::unbind(ShaderProgram& shader,const std::vector<int>& locations){
    if(type != VERTEX_BUFFER_OBJECT_WITH_VAO)
        disableAllVertexAttributes(shader,locations);
    if(type == VERTEX_BUFFER_OBJECT || type == VERTEX_BUFFER_OBJECT_SUB_DATA)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    isBound = false;
}

void VertexData::invalidate(){
    isDirty = true;
    if(type != VERTEX_ARRAY)
        glGenBuffers(1,&bufferHandle);
    switch(type){
        case VERTEX_BUFFER_OBJECT_WITH_VAO:
            glGenVertexArrays(1,&vaoHandle);
        break;
        case VERTEX_BUFFER_OBJECT_SUB_DATA:
            glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
            glBufferData(GL_ARRAY_BUFFER, buffer.capacity(), NULL, usage);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        break;
    }
}

VertexData::~VertexData(){SDL_Log("VERTEX DATA DESTROY!");
    if(type == VERTEX_BUFFER_OBJECT || type == VERTEX_BUFFER_OBJECT_SUB_DATA ||
        type == VERTEX_BUFFER_OBJECT_WITH_VAO){
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1,&bufferHandle);
            bufferHandle = 0;
        }
    if(type == VERTEX_BUFFER_OBJECT_WITH_VAO){
        if (vaoHandle != -1) {
			glDeleteVertexArrays(1,tmpHandle.data());
			vaoHandle = -1;
		}
    }
}