/*******************************************************************************
 * Copyright 2011 See AUTHORS file.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#pragma once

#define VERTEX_ARRAY 1
#define VERTEX_BUFFER_OBJECT 2
#define VERTEX_BUFFER_OBJECT_SUB_DATA 3
#define VERTEX_BUFFER_OBJECT_WITH_VAO 4

#include "../../GL.h"
#include <vector>
#include "../VertexAttributes.h"
#include "ShaderProgram.h"

/** A VertexData instance holds vertices for rendering with OpenGL. It is implemented as either a {@link VertexArray} or a
 * {@link VertexBufferObject}. Only the later supports OpenGL ES 2.0.
 * 
 * @author mzechner */
class VertexAttributes;
class VertexData{
private:
	bool isDirty = true,isBound = false,isStatic = true,isDirect,ownsBuffer;
	GLuint bufferHandle,vaoHandle = -1;
	int usage,type;
    VertexAttributes attributes = VertexAttributes();
    std::vector<GLfloat> buffer;
    std::vector<GLuint> tmpHandle;
	std::vector<int> cachedLocations = std::vector<int>();
    
    friend std::ostream& operator<<(std::ostream& os, VertexData &v)  
    {  
        os << "VERTICES("<<v.buffer.size()<<"){";
        for(int i = 0;i < v.buffer.size()-1;i++)
            os<< v.buffer[i] <<",";
        os << v.buffer[v.buffer.size()-1]<<"}";  
        return os;  
    } 
    
	void bufferChanged () {
		if (isBound) {
            if(type == VERTEX_BUFFER_OBJECT_SUB_DATA) 
                glBufferSubData(GL_ARRAY_BUFFER, 0, buffer.capacity(), buffer.data());
			else
                glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(GLfloat), buffer.data(), usage);
			isDirty = false;
		}
	}
    
    void unbindAttributes (ShaderProgram& shaderProgram) {
		if (cachedLocations.size() == 0) {
			return;
		}
		int numAttributes = attributes.size();
		for (int i = 0; i < numAttributes; i++) {
			int location = cachedLocations[i];
			if (location < 0) {
				continue;
			}
			shaderProgram.disableVertexAttribute(location);
		}
	}
    
	void bindAttributes (ShaderProgram& shader,const std::vector<int>&  locations);
    
    void setAllVertexAttributes(ShaderProgram& shader,const std::vector<int>& locations);
    void disableAllVertexAttributes(ShaderProgram& shader,const std::vector<int>& locations);
public:
    VertexData(){}
    ~VertexData();
    bool operator== (VertexData& obj){
        return isDirty == obj.isDirty && isBound == obj.isBound && isStatic == obj.isStatic &&
            isDirect == obj.isDirect && ownsBuffer == obj.ownsBuffer && bufferHandle == obj.bufferHandle &&
            vaoHandle == obj.vaoHandle && usage == obj.usage && type == obj.type && attributes == obj.attributes &&
            buffer == obj.buffer && tmpHandle == obj.tmpHandle && cachedLocations == obj.cachedLocations;
    }
    
	/** Low level method to reset the buffer and attributes to the specified values. Use with care!
	 * @param data
	 * @param ownsBuffer
	 * @param value */
	void setBuffer (std::vector<GLfloat> data, bool ownsBuffer,const VertexAttributes& value) {
		if (isBound) SDL_Log("Cannot change attributes while VBO is bound");
		attributes = value;
		this->ownsBuffer = ownsBuffer;
		buffer = data;
	}
    
	/** @return The GL enum used in the call to {@link GL20#glBufferData(int, int, java.nio.Buffer, int)}, e.g. GL_STATIC_DRAW or
	 *         GL_DYNAMIC_DRAW */
	int getUsage () {
		return usage;
	}

	/** Set the GL enum used in the call to {@link GL20#glBufferData(int, int, java.nio.Buffer, int)}, can only be called when the
	 * VBO is not bound. */
	void setUsage (int value) {
		if (isBound) SDL_Log("Cannot change usage while VBO is bound");
		usage = value;
	}
    
	VertexData (int usage,const std::vector<GLfloat>& data, bool ownsBuffer, const VertexAttributes& attributes) {
        this->type = VERTEX_BUFFER_OBJECT;
		glGenBuffers(1,&bufferHandle);
		setBuffer(data, ownsBuffer, attributes);
		setUsage(usage);
	}
    
	VertexData (int type,int numVertices,const std::vector<VertexAttribute>& attributes);

	VertexData (int type,bool isStatic, int numVertices, const VertexAttributes& attributes);
    
	/** @return the number of vertices this VertexData stores */
	int getNumVertices (){return buffer.size() / attributes.vertexSize;}

	/** @return the number of vertices this VertedData can store */
	int getNumMaxVertices (){return buffer.capacity() / attributes.vertexSize;}

	/** @return the {@link VertexAttributes} as specified during construction. */
	VertexAttributes& getAttributes (){return attributes;}

	/** Sets the vertices of this VertexData, discarding the old vertex data. The count must equal the number of floats per vertex
	 * times the number of vertices to be copied to this VertexData. The order of the vertex attributes must be the same as
	 * specified at construction time via {@link VertexAttributes}.
	 * <p>
	 * This can be called in between calls to bind and unbind. The vertex data will be updated instantly.
	 * @param vertices the vertex data
	 * @param offset the offset to start copying the data from
	 * @param count the number of floats to copy */
	void setVertices (const std::vector<GLfloat>& vertices, int offset, int count);

	/** Update (a portion of) the vertices. Does not resize the backing buffer.
	 * @param vertices the vertex data
	 * @param sourceOffset the offset to start copying the data from
	 * @param count the number of floats to copy */
	void updateVertices (int targetOffset,const std::vector<GLfloat>& vertices, int sourceOffset, int count);

	/** Returns the underlying FloatBuffer and marks it as dirty, causing the buffer contents to be uploaded on the next call to
	 * bind. If you need immediate uploading use {@link #setVertices(float[], int, int)}; Any modifications made to the Buffer
	 * *after* the call to bind will not automatically be uploaded.
	 * @return the underlying FloatBuffer holding the vertex data. */
	std::vector<GLfloat>& getBuffer (){isDirty = true; return buffer;}

	/** Binds this VertexData for rendering via glDrawArrays or glDrawElements. */
	void bind (ShaderProgram& shader) {bind(shader,std::vector<int>());}

	/** Binds this VertexData for rendering via glDrawArrays or glDrawElements.
	 * @param locations array containing the attribute locations. */
	void bind (ShaderProgram& shader,const std::vector<int>& locations);

	/** Unbinds this VertexData. */
	void unbind (ShaderProgram& shader){unbind(shader,std::vector<int>());}

	/** Unbinds this VertexData.
	 * @param locations array containing the attribute locations. */
	void unbind (ShaderProgram& shader, const std::vector<int>& locations);
	
	/** Invalidates the VertexData if applicable. Use this in case of a context loss. */
	void invalidate ();
};