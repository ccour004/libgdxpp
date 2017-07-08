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

#include <vector>
#include <map>
#include <memory>
#include <sstream>

#include "glutils/IndexData.h"
#include "glutils/VertexData.h"
#include "glutils/ShaderProgram.h"

#include "../math/Matrix3.h"
#include "../math/Matrix4.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/collision/BoundingBox.h"

/** <p>
 * A Mesh holds vertices composed of attributes specified by a {@link VertexAttributes} instance. The vertices are held either in
 * VRAM in form of vertex buffer objects or in RAM in form of vertex arrays. The former variant is more performant and is
 * preferred over vertex arrays if hardware supports it.
 * </p>
 * 
 * <p>
 * Meshes are automatically managed. If the OpenGL context is lost all vertex buffer objects get invalidated and must be reloaded
 * when the context is recreated. This only happens on Android when a user switches to another application or receives an incoming
 * call. A managed Mesh will be reloaded automagically so you don't have to do this manually.
 * </p>
 * 
 * <p>
 * A Mesh consists of vertices and optionally indices which specify which vertices define a triangle. Each vertex is composed of
 * attributes such as position, normal, color or texture coordinate. Note that not all of this attributes must be given, except
 * for position which is non-optional. Each attribute has an alias which is used when rendering a Mesh in OpenGL ES 2.0. The alias
 * is used to bind a specific vertex attribute to a shader attribute. The shader source and the alias of the attribute must match
 * exactly for this to work.
 * </p>
 * 
 * @author mzechner, Dave Clayton <contact@redskyforge.com>, Xoppa */
class Mesh{
protected:
	// TODO: Protected for now, because transforming a portion works but still copies all vertices
	void transformUV (Matrix3& matrix, const int start, const int count);
private:
    VertexData* makeVertexBuffer (bool isGL30,bool isStatic, int maxVertices, const VertexAttributes& vertexAttributes) {
		if (isGL30) {
			return new VertexData(VERTEX_BUFFER_OBJECT_WITH_VAO,isStatic, maxVertices, vertexAttributes);
		} else {
			return new VertexData(VERTEX_BUFFER_OBJECT,isStatic, maxVertices, vertexAttributes);
		}
	}
    
	static void addManagedMesh (std::string app, Mesh mesh);
    
	Vector3 tmpV = Vector3();
public:
    Mesh(){}
    ~Mesh(){SDL_Log("MESH DESTROY!");}
    std::string toString(){
        std::stringstream ss;
		ss<< "VERTICES:"<<vertices.get()<<",INDICES:"<<indices.get();
        return ss.str();
	}
    
	enum VertexDataType {
		VertexArray, VertexBufferObject, VertexBufferObjectSubData, VertexBufferObjectWithVAO
	};

	std::unique_ptr<VertexData> vertices;
	std::unique_ptr<IndexData> indices;
	bool autoBind = true;
	bool isVertexArray;
    
    bool operator==(Mesh& obj){
        return vertices == obj.vertices &&
            indices == obj.indices && autoBind == obj.autoBind && isVertexArray == obj.isVertexArray;
    }
    
    Mesh (int vertType,const std::vector<GLfloat>& vertexValues,
        const std::vector<VertexAttribute>& attributes,int indexType,const std::vector<GLuint>& indexValues, 
        bool isStatic,bool isVertexArray);
    
    /*Mesh (VertexData* vertices,IndexData* indices, bool isVertexArray){
            this->vertices = vertices;
            this->indices = indices;
            this->isVertexArray = isVertexArray;
	}
    
    Mesh (VertexData* vertices,IndexData* indices, 
        const std::vector<GLfloat>& vertexValues,const std::vector<GLuint>& indexValues,bool isVertexArray){
        this->vertices = vertices;
        this->indices = indices;
        this->isVertexArray = isVertexArray;
        setVertices(vertexValues);
        setIndices(indexValues);
	}*/
    
	/** Creates a new Mesh with the given attributes.
	 * 
	 * @param isStatic whether this mesh is static or not. Allows for internal optimizations.
	 * @param maxVertices the maximum number of vertices this mesh can hold
	 * @param maxIndices the maximum number of indices this mesh can hold
	 * @param attributes the {@link VertexAttribute}s. Each vertex attribute defines one property of a vertex such as position,
	 *           normal or texture coordinate */
	Mesh (bool isGL30,bool isStatic, int maxVertices, int maxIndices, const std::vector<VertexAttribute>& attributes);

	/** Creates a new Mesh with the given attributes.
	 * 
	 * @param isStatic whether this mesh is static or not. Allows for internal optimizations.
	 * @param maxVertices the maximum number of vertices this mesh can hold
	 * @param maxIndices the maximum number of indices this mesh can hold
	 * @param attributes the {@link VertexAttributes}. Each vertex attribute defines one property of a vertex such as position,
	 *           normal or texture coordinate */
	Mesh (bool isGL30,bool isStatic, int maxVertices, int maxIndices, const VertexAttributes& attributes);

	/** Creates a new Mesh with the given attributes. Adds extra optimizations for dynamic (frequently modified) meshes.
	 * 
	 * @param staticVertices whether vertices of this mesh are static or not. Allows for internal optimizations.
	 * @param staticIndices whether indices of this mesh are static or not. Allows for internal optimizations.
	 * @param maxVertices the maximum number of vertices this mesh can hold
	 * @param maxIndices the maximum number of indices this mesh can hold
	 * @param attributes the {@link VertexAttributes}. Each vertex attribute defines one property of a vertex such as position,
	 *           normal or texture coordinate
	 * 
	 * @author Jaroslaw Wisniewski <j.wisniewski@appsisle.com> **/
	Mesh (bool isGL30,bool staticVertices, bool staticIndices, int maxVertices, int maxIndices, const VertexAttributes& attributes);

	/** Creates a new Mesh with the given attributes. This is an expert method with no error checking. Use at your own risk.
	 * 
	 * @param type the {@link VertexDataType} to be used, VBO or VA.
	 * @param isStatic whether this mesh is static or not. Allows for internal optimizations.
	 * @param maxVertices the maximum number of vertices this mesh can hold
	 * @param maxIndices the maximum number of indices this mesh can hold
	 * @param attributes the {@link VertexAttribute}s. Each vertex attribute defines one property of a vertex such as position,
	 *           normal or texture coordinate */
	Mesh (VertexDataType type, bool isStatic, int maxVertices, int maxIndices, const std::vector<VertexAttribute>& attributes);
        
    int getType(const VertexDataType& type,bool isVert){
		switch (type) {
		case VertexBufferObject:return isVert?VERTEX_BUFFER_OBJECT:INDEX_BUFFER_OBJECT;
		case VertexBufferObjectSubData:return isVert?VERTEX_BUFFER_OBJECT_SUB_DATA:INDEX_BUFFER_OBJECT_SUB_DATA;
		case VertexBufferObjectWithVAO:return isVert?VERTEX_BUFFER_OBJECT_WITH_VAO:INDEX_BUFFER_OBJECT_SUB_DATA;
		case VertexArray:default:return isVert?VERTEX_ARRAY:INDEX_ARRAY;
        }
    }
	
	/** Creates a new Mesh with the given attributes. This is an expert method with no error checking. Use at your own risk.
	 * 
	 * @param type the {@link VertexDataType} to be used, VBO or VA.
	 * @param isStatic whether this mesh is static or not. Allows for internal optimizations.
	 * @param maxVertices the maximum number of vertices this mesh can hold
	 * @param maxIndices the maximum number of indices this mesh can hold
	 * @param attributes the {@link VertexAttributes}. */
	Mesh (const VertexDataType& type, bool isStatic, int maxVertices, int maxIndices, const VertexAttributes& attributes);

	/** Sets the vertices of this Mesh. The attributes are assumed to be given in float format.
	 * 
	 * @param vertices the vertices->
	 * @return the mesh for invocation chaining. */
	Mesh& setVertices (const std::vector<GLfloat>& vertices) {
		this->vertices->setVertices(vertices, 0, vertices.size());
		return *this;
	}

	/** Sets the vertices of this Mesh. The attributes are assumed to be given in float format.
	 * 
	 * @param vertices the vertices->
	 * @param offset the offset into the vertices array
	 * @param count the number of floats to use
	 * @return the mesh for invocation chaining. */
	Mesh& setVertices (const std::vector<GLfloat>& vertices, int offset, int count) {
		this->vertices->setVertices(vertices, offset, count);
		return *this;
	}

	/** Update (a portion of) the vertices-> Does not resize the backing buffer.
	 * @param targetOffset the offset in number of floats of the mesh part.
	 * @param source the vertex data to update the mesh part with */
	Mesh& updateVertices (int targetOffset, const std::vector<GLfloat>& source) {
		return updateVertices(targetOffset, source, 0, source.size());
	}

	/** Update (a portion of) the vertices-> Does not resize the backing buffer.
	 * @param targetOffset the offset in number of floats of the mesh part.
	 * @param source the vertex data to update the mesh part with
	 * @param sourceOffset the offset in number of floats within the source array
	 * @param count the number of floats to update */
	Mesh& updateVertices (int targetOffset, const std::vector<GLfloat>& source, int sourceOffset, int count) {
		this->vertices->updateVertices(targetOffset, source, sourceOffset, count);
		return *this;
	}

	/** Copies the vertices from the Mesh to the float array. The float array must be large enough to hold all the Mesh's vertices->
	 * @param vertices the array to copy the vertices to */
	const std::vector<GLfloat>& getVertices (std::vector<GLfloat>& vertices) {
		return getVertices(0, -1, vertices);
	}

	/** Copies the the remaining vertices from the Mesh to the float array. The float array must be large enough to hold the
	 * remaining vertices->
	 * @param srcOffset the offset (in number of floats) of the vertices in the mesh to copy
	 * @param vertices the array to copy the vertices to */
	const std::vector<GLfloat>& getVertices (int srcOffset, std::vector<GLfloat>& vertices) {
		return getVertices(srcOffset, -1, vertices);
	}

	/** Copies the specified vertices from the Mesh to the float array. The float array must be large enough to hold count vertices->
	 * @param srcOffset the offset (in number of floats) of the vertices in the mesh to copy
	 * @param count the amount of floats to copy
	 * @param vertices the array to copy the vertices to */
	const std::vector<GLfloat>& getVertices (int srcOffset, int count, std::vector<GLfloat>& vertices) {
		return getVertices(srcOffset, count, vertices, 0);
	}

	/** Copies the specified vertices from the Mesh to the float array. The float array must be large enough to hold
	 * destOffset+count vertices->
	 * @param srcOffset the offset (in number of floats) of the vertices in the mesh to copy
	 * @param count the amount of floats to copy
	 * @param vertices the array to copy the vertices to
	 * @param destOffset the offset (in floats) in the vertices array to start copying */
	const std::vector<GLfloat>& getVertices (int srcOffset, int count, std::vector<GLfloat>& vertices, int destOffset) {
		// TODO: Perhaps this method should be vertexSize aware??
		const int max = getNumVertices() * getVertexSize() / 4;
		if (count == -1) {
			count = max - srcOffset;
			if (count > vertices.size() - destOffset) count = vertices.size() - destOffset;
		}
		if (srcOffset < 0 || count <= 0 || (srcOffset + count) > max || destOffset < 0 || destOffset >= vertices.size())
			SDL_Log("IndexOutOfBoundsException!");
		if ((vertices.size() - destOffset) < count)
			SDL_Log("not enough room in vertices array, has %lui floats, needs %i",vertices.size(),count);
        
        int counter = 0;
        for(int i = destOffset;i < destOffset+count;i++)
            vertices[i] = getVerticesBuffer()[srcOffset+(counter++)];
		return vertices;
	}

	/** Sets the indices of this Mesh
	 * 
	 * @param indices the indices
	 * @return the mesh for invocation chaining. */
	Mesh& setIndices (const std::vector<GLuint>& indices) {
		this->indices->setIndices(indices, 0, indices.size());
		return *this;
	}

	/** Sets the indices of this Mesh.
	 * 
	 * @param indices the indices
	 * @param offset the offset into the indices array
	 * @param count the number of indices to copy
	 * @return the mesh for invocation chaining. */
	Mesh& setIndices (const std::vector<GLuint>& indices, int offset, int count) {
		this->indices->setIndices(indices, offset, count);
		return *this;
	}

	/** Copies the indices from the Mesh to the short array. The short array must be large enough to hold all the Mesh's indices->
	 * @param indices the array to copy the indices to */
	void getIndices (std::vector<GLuint>& indices) {
		getIndices(indices, 0);
	}

	/** Copies the indices from the Mesh to the short array. The short array must be large enough to hold destOffset + all the
	 * Mesh's indices->
	 * @param indices the array to copy the indices to
	 * @param destOffset the offset in the indices array to start copying */
	void getIndices (std::vector<GLuint>& indices, int destOffset) {
		getIndices(0, indices, destOffset);
	}

	/** Copies the remaining indices from the Mesh to the short array. The short array must be large enough to hold destOffset + all
	 * the remaining indices->
	 * @param srcOffset the zero-based offset of the first index to fetch
	 * @param indices the array to copy the indices to
	 * @param destOffset the offset in the indices array to start copying */
	void getIndices (int srcOffset,std::vector<GLuint>& indices, int destOffset) {
		getIndices(srcOffset, -1, indices, destOffset);
	}

	/** Copies the indices from the Mesh to the short array. The short array must be large enough to hold destOffset + count
	 * indices->
	 * @param srcOffset the zero-based offset of the first index to fetch
	 * @param count the total amount of indices to copy
	 * @param indices the array to copy the indices to
	 * @param destOffset the offset in the indices array to start copying */
	void getIndices (int srcOffset, int count, std::vector<GLuint>& indices, int destOffset) {
		int max = getNumIndices();
		if (count < 0) count = max - srcOffset;
		if (srcOffset < 0 || srcOffset >= max || srcOffset + count > max)
			SDL_Log("Invalid range specified, offset: %i, count: %i, max: %i",srcOffset,count,max);
		if ((indices.size() - destOffset) < count)
			SDL_Log("not enough room in indices array, has %lui shorts, needs %i",indices.size(),count);
        
        int counter = 0;
        for(int i = destOffset;i < destOffset+count;i++)
            indices[i] = getIndicesBuffer()[srcOffset+(counter++)];  
	}

	/** @return the number of defined indices */
	int getNumIndices () {
		return indices->getNumIndices();
	}

	/** @return the number of defined vertices */
	int getNumVertices () {
		return vertices->getNumVertices();
	}

	/** @return the maximum number of vertices this mesh can hold */
	int getMaxVertices () {
		return vertices->getNumMaxVertices();
	}

	/** @return the maximum number of indices this mesh can hold */
	int getMaxIndices () {
		return indices->getNumMaxIndices();
	}

	/** @return the size of a single vertex in bytes */
	int getVertexSize () {
		return vertices->getAttributes().vertexSize;
	}

	/** Sets whether to bind the underlying {@link VertexArray} or {@link VertexBufferObject} automatically on a call to one of the
	 * render methods. Usually you want to use autobind. Manual binding is an expert functionality. There is a driver bug on the
	 * MSM720xa chips that will fuck up memory if you manipulate the vertices and indices of a Mesh multiple times while it is
	 * bound. Keep this in mind.
	 * 
	 * @param autoBind whether to autobind meshes. */
	void setAutoBind (bool autoBind) {
		this->autoBind = autoBind;
	}

	/** Binds the underlying {@link VertexBufferObject} and {@link IndexBufferObject} if indices where given. Use this with OpenGL
	 * ES 2.0 and when auto-bind is disabled.
	 * 
	 * @param shader the shader (does not bind the shader) */
	void bind (ShaderProgram& shader);

	/** Binds the underlying {@link VertexBufferObject} and {@link IndexBufferObject} if indices where given. Use this with OpenGL
	 * ES 2.0 and when auto-bind is disabled.
	 * 
	 * @param shader the shader (does not bind the shader)
	 * @param locations array containing the attribute locations. */
	void bind (ShaderProgram& shader, const std::vector<int>& locations);

	/** Unbinds the underlying {@link VertexBufferObject} and {@link IndexBufferObject} is indices were given. Use this with OpenGL
	 * ES 1.x and when auto-bind is disabled.
	 * 
	 * @param shader the shader (does not unbind the shader) */
	void unbind (ShaderProgram&  shader) {
		unbind(shader, std::vector<int>());
	}

	/** Unbinds the underlying {@link VertexBufferObject} and {@link IndexBufferObject} is indices were given. Use this with OpenGL
	 * ES 1.x and when auto-bind is disabled.
	 * 
	 * @param shader the shader (does not unbind the shader)
	 * @param locations array containing the attribute locations. */
	void unbind (ShaderProgram&  shader, const std::vector<int>& locations) {
		vertices->unbind(shader, locations);
		if (indices->getNumIndices() > 0) indices->unbind();
	}

	/** <p>
	 * Renders the mesh using the given primitive type. If indices are set for this mesh then getNumIndices() / #vertices per
	 * primitive primitives are rendered. If no indices are set then getNumVertices() / #vertices per primitive are rendered.
	 * </p>
	 * 
	 * <p>
	 * This method will automatically bind each vertex attribute as specified at construction time via {@link VertexAttributes} to
	 * the respective shader attributes. The binding is based on the alias defined for each VertexAttribute.
	 * </p>
	 * 
	 * <p>
	 * This method must only be called after the {@link ShaderProgram#begin()} method has been called!
	 * </p>
	 * 
	 * <p>
	 * This method is intended for use with OpenGL ES 2.0 and will throw an IllegalStateException when OpenGL ES 1.x is used.
	 * </p>
	 * 
	 * @param primitiveType the primitive type */
	void render (ShaderProgram& shader, int primitiveType) {
		render(shader, primitiveType, 0, indices->getNumMaxIndices() > 0 ? getNumIndices() : getNumVertices(), autoBind);
	}

	/** <p>
	 * Renders the mesh using the given primitive type. offset specifies the offset into either the vertex buffer or the index
	 * buffer depending on whether indices are defined. count specifies the number of vertices or indices to use thus count /
	 * #vertices per primitive primitives are rendered.
	 * </p>
	 * 
	 * <p>
	 * This method will automatically bind each vertex attribute as specified at construction time via {@link VertexAttributes} to
	 * the respective shader attributes. The binding is based on the alias defined for each VertexAttribute.
	 * </p>
	 * 
	 * <p>
	 * This method must only be called after the {@link ShaderProgram#begin()} method has been called!
	 * </p>
	 * 
	 * <p>
	 * This method is intended for use with OpenGL ES 2.0 and will throw an IllegalStateException when OpenGL ES 1.x is used.
	 * </p>
	 * 
	 * @param shader the shader to be used
	 * @param primitiveType the primitive type
	 * @param offset the offset into the vertex or index buffer
	 * @param count number of vertices or indices to use */
	void render (ShaderProgram& shader, int primitiveType, int offset, int count) {
		render(shader, primitiveType, offset, count, autoBind);
	}

	/** <p>
	 * Renders the mesh using the given primitive type. offset specifies the offset into either the vertex buffer or the index
	 * buffer depending on whether indices are defined. count specifies the number of vertices or indices to use thus count /
	 * #vertices per primitive primitives are rendered.
	 * </p>
	 * 
	 * <p>
	 * This method will automatically bind each vertex attribute as specified at construction time via {@link VertexAttributes} to
	 * the respective shader attributes. The binding is based on the alias defined for each VertexAttribute.
	 * </p>
	 * 
	 * <p>
	 * This method must only be called after the {@link ShaderProgram#begin()} method has been called!
	 * </p>
	 * 
	 * <p>
	 * This method is intended for use with OpenGL ES 2.0 and will throw an IllegalStateException when OpenGL ES 1.x is used.
	 * </p>
	 * 
	 * @param shader the shader to be used
	 * @param primitiveType the primitive type
	 * @param offset the offset into the vertex or index buffer
	 * @param count number of vertices or indices to use
	 * @param autoBind overrides the autoBind member of this Mesh */
	void render (ShaderProgram& shader, int primitiveType, int offset, int count, bool autoBind);

	/** Returns the first {@link VertexAttribute} having the given {@link Usage}.
	 * 
	 * @param usage the Usage.
	 * @return the VertexAttribute or null if no attribute with that usage was found. */
	VertexAttribute& getVertexAttribute (int usage);

	/** @return the vertex attributes of this Mesh */
	VertexAttributes& getVertexAttributes () {
		return vertices->getAttributes();
	}

	/** @return the backing FloatBuffer holding the vertices-> Does not have to be a direct buffer on Android! */
	std::vector<GLfloat>& getVerticesBuffer () {
		return vertices->getBuffer();
	}

	/** Calculates the {@link BoundingBox} of the vertices contained in this mesh. In case no vertices are defined yet a
	 * {@link GdxRuntimeException} is thrown.
	 * 
	 * @param bbox the bounding box to store the result in. */
	void calculateBoundingBox (BoundingBox& bbox);
    
	/** Calculate the {@link BoundingBox} of the specified part.
	 * @param out the bounding box to store the result in.
	 * @param offset the start index of the part.
	 * @param count the amount of indices the part contains.
	 * @return the value specified by out. */
	BoundingBox calculateBoundingBox (BoundingBox& out, int offset, int count) {
		return extendBoundingBox(out.inf(), offset, count);
	}

	/** Calculate the {@link BoundingBox} of the specified part.
	 * @param out the bounding box to store the result in.
	 * @param offset the start index of the part.
	 * @param count the amount of indices the part contains.
	 * @return the value specified by out. */
	BoundingBox calculateBoundingBox (BoundingBox& out, int offset, int count,const Matrix4& transform) {
		return extendBoundingBox(out.inf(), offset, count, transform);
	}

	/** Extends the specified {@link BoundingBox} with the specified part.
	 * @param out the bounding box to store the result in.
	 * @param offset the start index of the part.
	 * @param count the amount of indices the part contains.
	 * @return the value specified by out. */
	BoundingBox extendBoundingBox (BoundingBox& out, int offset, int count) {
		return extendBoundingBox(out, offset, count, Matrix4());
	}

	/** Extends the specified {@link BoundingBox} with the specified part.
	 * @param out the bounding box to store the result in.
	 * @param offset the start of the part.
	 * @param count the size of the part.
	 * @return the value specified by out. */
	BoundingBox extendBoundingBox (BoundingBox& out, int offset, int count, const Matrix4& transform);

	/** Calculates the squared radius of the bounding sphere around the specified center for the specified part.
	 * @param centerX The X coordinate of the center of the bounding sphere
	 * @param centerY The Y coordinate of the center of the bounding sphere
	 * @param centerZ The Z coordinate of the center of the bounding sphere
	 * @param offset the start index of the part.
	 * @param count the amount of indices the part contains.
	 * @return the squared radius of the bounding sphere. */
	float calculateRadiusSquared (const float centerX, const float centerY, const float centerZ, int offset, int count,
		const Matrix4& transform);

	/** Calculates the radius of the bounding sphere around the specified center for the specified part.
	 * @param centerX The X coordinate of the center of the bounding sphere
	 * @param centerY The Y coordinate of the center of the bounding sphere
	 * @param centerZ The Z coordinate of the center of the bounding sphere
	 * @param offset the start index of the part.
	 * @param count the amount of indices the part contains.
	 * @return the radius of the bounding sphere. */
	float calculateRadius (const float centerX, const float centerY, const float centerZ, int offset, int count,
		const Matrix4& transform) {
		return sqrt(calculateRadiusSquared(centerX, centerY, centerZ, offset, count, transform));
	}

	/** Calculates the squared radius of the bounding sphere around the specified center for the specified part.
	 * @param center The center of the bounding sphere
	 * @param offset the start index of the part.
	 * @param count the amount of indices the part contains.
	 * @return the squared radius of the bounding sphere. */
	float calculateRadius (const Vector3& center, int offset, int count, const Matrix4& transform) {
		return calculateRadius(center.x, center.y, center.z, offset, count, transform);
	}

	/** Calculates the squared radius of the bounding sphere around the specified center for the specified part.
	 * @param centerX The X coordinate of the center of the bounding sphere
	 * @param centerY The Y coordinate of the center of the bounding sphere
	 * @param centerZ The Z coordinate of the center of the bounding sphere
	 * @param offset the start index of the part.
	 * @param count the amount of indices the part contains.
	 * @return the squared radius of the bounding sphere. */
	float calculateRadius (const float centerX, const float centerY, const float centerZ, int offset, int count) {
		return calculateRadius(centerX, centerY, centerZ, offset, count, Matrix4());
	}

	/** Calculates the squared radius of the bounding sphere around the specified center for the specified part.
	 * @param center The center of the bounding sphere
	 * @param offset the start index of the part.
	 * @param count the amount of indices the part contains.
	 * @return the squared radius of the bounding sphere. */
	float calculateRadius (const Vector3& center, int offset, int count) {
		return calculateRadius(center.x, center.y, center.z, offset, count, Matrix4());
	}

	/** Calculates the squared radius of the bounding sphere around the specified center for the specified part.
	 * @param centerX The X coordinate of the center of the bounding sphere
	 * @param centerY The Y coordinate of the center of the bounding sphere
	 * @param centerZ The Z coordinate of the center of the bounding sphere
	 * @return the squared radius of the bounding sphere. */
	float calculateRadius (const float centerX, const float centerY, const float centerZ) {
		return calculateRadius(centerX, centerY, centerZ, 0, getNumIndices(), Matrix4());
	}

	/** Calculates the squared radius of the bounding sphere around the specified center for the specified part.
	 * @param center The center of the bounding sphere
	 * @return the squared radius of the bounding sphere. */
	float calculateRadius (const Vector3& center) {
		return calculateRadius(center.x, center.y, center.z, 0, getNumIndices(), Matrix4());
	}

	/** @return the backing shortbuffer holding the indices-> Does not have to be a direct buffer on Android! */
	std::vector<GLuint>& getIndicesBuffer () {
		return indices->getBuffer();
	}
    
    bool hasVertexAttribute (int usage);

	/** Method to scale the positions in the mesh. Normals will be kept as is. This is a potentially slow operation, use with care.
	 * It will also create a temporary const std::vector<GLfloat>& which will be garbage collected.
	 * 
	 * @param scaleX scale on x
	 * @param scaleY scale on y
	 * @param scaleZ scale on z */
	void scale (float scaleX, float scaleY, float scaleZ);

	/** Method to transform the positions in the mesh. Normals will be kept as is. This is a potentially slow operation, use with
	 * care. It will also create a temporary const std::vector<GLfloat>& which will be garbage collected.
	 * 
	 * @param matrix the transformation matrix */
	void transform (Matrix4& matrix) {
		transform(matrix, 0, getNumVertices());
	}

	// TODO: Protected for now, because transforming a portion works but still copies all vertices
	void transform (Matrix4& matrix, const int start, const int count);

	/** Method to transform the positions in the float array. Normals will be kept as is. This is a potentially slow operation, use
	 * with care.
	 * @param matrix the transformation matrix
	 * @param vertices the float array
	 * @param vertexSize the number of floats in each vertex
	 * @param offset the offset within a vertex to the position
	 * @param dimensions the size of the position
	 * @param start the vertex to start with
	 * @param count the amount of vertices to transform */
	static void transform (Matrix4& matrix, std::vector<GLfloat>& vertices, int vertexSize, int offset, int dimensions,
		int start, int count) {
		if (offset < 0 || dimensions < 1 || (offset + dimensions) > vertexSize) SDL_Log("IndexOutOfBoundsException!");
		if (start < 0 || count < 1 || ((start + count) * vertexSize) > vertices.size())
			SDL_Log("IndexOutOfBoundsException(start = %i, count = %i, vertexSize = %i, length = %lui",start,count,vertexSize,vertices.size());

		Vector3 tmp = Vector3();

		int idx = offset + (start * vertexSize);
		switch (dimensions) {
		case 1:
			for (int i = 0; i < count; i++) {
				tmp.set(vertices[idx], 0, 0).mul(matrix);
				vertices[idx] = tmp.x;
				idx += vertexSize;
			}
			break;
		case 2:
			for (int i = 0; i < count; i++) {
				tmp.set(vertices[idx], vertices[idx + 1], 0).mul(matrix);
				vertices[idx] = tmp.x;
				vertices[idx + 1] = tmp.y;
				idx += vertexSize;
			}
			break;
		case 3:
			for (int i = 0; i < count; i++) {
				tmp.set(vertices[idx], vertices[idx + 1], vertices[idx + 2]).mul(matrix);
				vertices[idx] = tmp.x;
				vertices[idx + 1] = tmp.y;
				vertices[idx + 2] = tmp.z;
				idx += vertexSize;
			}
			break;
		}
	}

	/** Method to transform the texture coordinates in the mesh. This is a potentially slow operation, use with care. It will also
	 * create a temporary const std::vector<GLfloat>& which will be garbage collected.
	 * 
	 * @param matrix the transformation matrix */
	void transformUV (Matrix3& matrix) {
		transformUV(matrix, 0, getNumVertices());
	}

	/** Method to transform the texture coordinates (UV) in the float array. This is a potentially slow operation, use with care.
	 * @param matrix the transformation matrix
	 * @param vertices the float array
	 * @param vertexSize the number of floats in each vertex
	 * @param offset the offset within a vertex to the texture location
	 * @param start the vertex to start with
	 * @param count the amount of vertices to transform */
	static void transformUV (Matrix3& matrix,std::vector<GLfloat>& vertices, int vertexSize, int offset, int start, int count) {
		if (start < 0 || count < 1 || ((start + count) * vertexSize) > vertices.size())
			SDL_Log("start = %i, count = %i, vertexSize = %i, length = %lui",start,count,vertexSize,vertices.size());

		Vector2 tmp = Vector2();

		int idx = offset + (start * vertexSize);
		for (int i = 0; i < count; i++) {
			tmp.set(vertices[idx], vertices[idx + 1]).mul(matrix);
			vertices[idx] = tmp.x;
			vertices[idx + 1] = tmp.y;
			idx += vertexSize;
		}
	}

	/** Copies this mesh optionally removing duplicate vertices and/or reducing the amount of attributes.
	 * @param isStatic whether the new mesh is static or not. Allows for internal optimizations.
	 * @param removeDuplicates whether to remove duplicate vertices if possible. Only the vertices specified by usage are checked.
	 * @param usage which attributes (if available) to copy
	 * @return the copy of this mesh */
	//Mesh& copy (bool isGL30,bool isStatic, bool removeDuplicates, const std::vector<int>& usage);

	/** Copies this mesh.
	 * @param isStatic whether the new mesh is static or not. Allows for internal optimizations.
	 * @return the copy of this mesh */
	/*Mesh& copy (bool isGL30,bool isStatic) {
		return copy(isGL30,isStatic, false, std::vector<int>());
	}*/
};


