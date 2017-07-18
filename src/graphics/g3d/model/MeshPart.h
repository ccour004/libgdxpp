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

#include "../../Mesh.h"
#include "../../VertexAttributes.h"
#include "../../glutils/ShaderProgram.h"
#include "../../../math/Vector3.h"
#include "../../../math/collision/BoundingBox.h"

/** A MeshPart is composed of a subset of vertices of a {@link Mesh}, along with the primitive type. The vertices subset is
 * described by an offset and size. When the mesh is indexed (which is when {@link Mesh#getNumIndices()} > 0), then the
 * {@link #offset} represents the offset in the indices array and {@link #size} represents the number of indices. When the mesh
 * isn't indexed, then the {@link #offset} member represents the offset in the vertices array and the {@link #size} member
 * represents the number of vertices.</p>
 * 
 * In other words: Regardless whether the mesh is indexed or not, when {@link #primitiveType} is not a strip, then {@link #size}
 * equals the number of primitives multiplied by the number of vertices per primitive. So if the MeshPart represents 4 triangles (
 * {@link #primitiveType} is GL_TRIANGLES), then the {@link #size} member is 12 (4 triangles * 3 vertices = 12 vertices total).
 * Likewise, if the part represents 12 lines ({@link #primitiveType} is GL_LINES), then the size is 24 (12 lines * 2 vertices = 24
 * vertices total).</p>
 * 
 * Note that some classes might require the mesh (part) to be indexed.</p>
 * 
 * The {@link Mesh} referenced by the {@link #mesh} member must outlive the MeshPart. When the mesh is disposed, the MeshPart is
 * unusable.
 * @author badlogic, Xoppa */
class MeshPart {
	/** Temporary static {@link BoundingBox} instance, used in the {@link #update()} method. **/
	static BoundingBox bounds;
public:
	/** Unique id within model, may be null. Will be ignored by {@link #equals(MeshPart)} **/
	std::string id;
	/** The primitive type, OpenGL constant e.g: {@link GL20#GL_TRIANGLES}, {@link GL20#GL_POINTS}, {@link GL20#GL_LINES},
	 * {@link GL20#GL_LINE_STRIP}, {@link GL20#GL_TRIANGLE_STRIP} **/
	int primitiveType;
	/** The offset in the {@link #mesh} to this part. If the mesh is indexed ({@link Mesh#getNumIndices()} > 0), this is the offset
	 * in the indices array, otherwise it is the offset in the vertices array. **/
	int offset;
	/** The size (in total number of vertices) of this part in the {@link #mesh}. When the mesh is indexed (
	 * {@link Mesh#getNumIndices()} > 0), this is the number of indices, otherwise it is the number of vertices. **/
	int size;
	/** The Mesh the part references, also stored in {@link Model} **/
	std::shared_ptr<Mesh> mesh;
	/** The offset to the center of the bounding box of the shape, only valid after the call to {@link #update()}. **/
	Vector3 center = Vector3();
	/** The location, relative to {@link #center}, of the corner of the axis aligned bounding box of the shape. Or, in other words:
	 * half the dimensions of the bounding box of the shape, where {@link Vector3#x} is half the width, {@link Vector3#y} is half
	 * the height and {@link Vector3#z} is half the depth. Only valid after the call to {@link #update()}. **/
	Vector3 halfExtents = Vector3();
	/** The radius relative to {@link #center} of the bounding sphere of the shape, or negative if not calculated yet. This is the
	 * same as the length of the {@link #halfExtents} member. See {@link #update()}. **/
	float radius = -1.0f;

	/** Construct a new MeshPart, with null values. The MeshPart is unusable until you set all members. **/
	MeshPart () {
	}

	/** Construct a new MeshPart and set all its values.
	 * @param id The id of the new part, may be null.
	 * @param mesh The mesh which holds all vertices and (optional) indices of this part.
	 * @param offset The offset within the mesh to this part.
	 * @param size The size (in total number of vertices) of the part.
	 * @param type The primitive type of the part (e.g. GL_TRIANGLES, GL_LINE_STRIP, etc.). */
	MeshPart (const std::string& id, std::shared_ptr<Mesh> mesh, int offset, int size, int type) {
		set(id, mesh, offset, size, type);
	}

	/** Construct a new MeshPart which is an exact copy of the provided MeshPart.
	 * @param copyFrom The MeshPart to copy. */
	MeshPart (const MeshPart& copyFrom) {
		set(copyFrom);
	}

	/** Set this MeshPart to be a copy of the other MeshPart
	 * @param other The MeshPart from which to copy the values
	 * @return this MeshPart, for chaining */
	MeshPart& set (const MeshPart& other) {
		this->id = other.id;
		this->mesh = other.mesh;
		this->offset = other.offset;
		this->size = other.size;
		this->primitiveType = other.primitiveType;
		this->center.set(other.center);
		this->halfExtents.set(other.halfExtents);
		this->radius = other.radius;
		return *this;
	}

	/** Set this MeshPart to given values, does not {@link #update()} the bounding box values.
	 * @return this MeshPart, for chaining. */
	MeshPart& set (const std::string& id,std::shared_ptr<Mesh> mesh,int offset,int size,int type) {
		this->id = id;
		this->mesh = mesh;
		this->offset = offset;
		this->size = size;
		this->primitiveType = type;
		this->center.set(0, 0, 0);
		this->halfExtents.set(0, 0, 0);
		this->radius = -1f;
		return *this;
	}

	/** Calculates and updates the {@link #center}, {@link #halfExtents} and {@link #radius} values. This is considered a costly
	 * operation and should not be called frequently. All vertices (points) of the shape are traversed to calculate the maximum and
	 * minimum x, y and z coordinate of the shape. Note that MeshPart is not aware of any transformation that might be applied when
	 * rendering. It calculates the untransformed (not moved, not scaled, not rotated) values. */
	void update () {
		mesh.calculateBoundingBox(bounds, offset, size);
		bounds.getCenter(center);
		bounds.getDimensions(halfExtents).scl(0.5f);
		radius = halfExtents.len();
	}

	/** Compares this MeshPart to the specified MeshPart and returns true if they both reference the same {@link Mesh} and the
	 * {@link #offset}, {@link #size} and {@link #primitiveType} members are equal. The {@link #id} member is ignored.
	 * @param other The other MeshPart to compare this MeshPart to.
	 * @return True when this MeshPart equals the other MeshPart (ignoring the {@link #id} member), false otherwise. */
	bool operator==(const MeshPart& other) {
		return other.mesh == mesh && other.primitiveType == primitiveType && other.offset == offset && other.size == size;
	}

	/** Renders the mesh part using the specified shader, must be called in between {@link ShaderProgram#begin()} and
	 * {@link ShaderProgram#end()}.
	 * @param shader the shader to be used
	 * @param autoBind overrides the autoBind member of the Mesh */
	void render (const ShaderProgram& shader, bool autoBind) {
		mesh->render(shader, primitiveType, offset, size, autoBind);
	}

	/** Renders the mesh part using the specified shader, must be called in between {@link ShaderProgram#begin()} and
	 * {@link ShaderProgram#end()}.
	 * @param shader the shader to be used */
	void render (const ShaderProgram& shader) {
		mesh->render(shader, primitiveType, offset, size);
	}
};