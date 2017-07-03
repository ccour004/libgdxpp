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
#include "../../GL.h"
#include <vector>

#define INDEX_ARRAY 1
#define INDEX_BUFFER_OBJECT 2
#define INDEX_BUFFER_OBJECT_SUB_DATA 3

/** An IndexData instance holds index data. Can be either a plain short buffer or an OpenGL buffer object.
 * @author mzechner */
class IndexData{
    GLuint bufferHandle;
    bool isDirty,isBound,isDirect;
    std::vector<GLuint> buffer;
    int type,usage;
    
    void createBufferObject();
public:
    friend std::ostream& operator<<(std::ostream& os, IndexData &v)  
    {  
        os << "INDICES("<<v.buffer.size()<<"){";
        for(int i = 0;i < v.buffer.size()-1;i++)
            os<< v.buffer[i] <<",";
        os << v.buffer[v.buffer.size()-1]<<"}";  
        return os;  
    } 
    
    IndexData(){isDirty = false;isBound = false;isDirect = false;}
    bool operator== (IndexData& obj){
        return bufferHandle == obj.bufferHandle && isDirty == obj.isDirty && isBound == obj.isBound && 
            isDirect == obj.isDirect && buffer == obj.buffer && type == obj.type && usage == obj.usage;
    }
    IndexData(int type,bool isStatic, int maxIndices);
    IndexData(int type,int maxIndices);
    
	/** @return the number of indices currently stored in this buffer */
	int getNumIndices () {return buffer.size();}

	/** @return the maximum number of indices this IndexBufferObject can store. */
	int getNumMaxIndices () {return buffer.capacity();}

	/** <p>
	 * Sets the indices of this IndexBufferObject, discarding the old indices. The count must equal the number of indices to be
	 * copied to this IndexBufferObject.
	 * </p>
	 * 
	 * <p>
	 * This can be called in between calls to {@link #bind()} and {@link #unbind()}. The index data will be updated instantly.
	 * </p>
	 * 
	 * @param indices the index data
	 * @param offset the offset to start copying the data from
	 * @param count the number of shorts to copy */
	void setIndices (const std::vector<GLuint>& indices, int offset, int count);

	/** Copies the specified indices to the indices of this IndexBufferObject, discarding the old indices. Copying start at the
	 * current {@link ShortBuffer#position()} of the specified buffer and copied the {@link ShortBuffer#remaining()} amount of
	 * indices. This can be called in between calls to {@link #bind()} and {@link #unbind()}. The index data will be updated
	 * instantly.
	 * @param indices the index data to copy */
	void setIndices (const std::vector<GLuint>& indices) {buffer = indices; setIndices(0,0);}
    
    void setIndices(int offset,int count);

	/** Update (a portion of) the indices.
	 * @param targetOffset offset in indices buffer
	 * @param indices the index data
	 * @param offset the offset to start copying the data from
	 * @param count the number of shorts to copy */
	void updateIndices (int targetOffset, const std::vector<GLuint>& indices, int offset, int count);

	/** <p>
	 * Returns the underlying ShortBuffer. If you modify the buffer contents they wil be uploaded on the call to {@link #bind()}.
	 * If you need immediate uploading use {@link #setIndices(short[], int, int)}.
	 * </p>
	 * 
	 * @return the underlying short buffer. */
	std::vector<GLuint>& getBuffer();

	/** Binds this IndexBufferObject for rendering with glDrawElements. */
	void bind();

	/** Unbinds this IndexBufferObject. */
	void unbind();

	/** Invalidates the IndexBufferObject so a new OpenGL buffer handle is created. Use this in case of a context loss. */
	void invalidate();

	/** Disposes this IndexDatat and all its associated OpenGL resources. */
	void dispose();
};