#include "Mesh.h"
#include "glutils/ShaderProgram.h"
#include "glutils/VertexData.h"
#include "glutils/IndexData.h"
#include "VertexAttribute.h"
#include "VertexAttributes.h"

Mesh::Mesh (int vertType,const std::vector<GLfloat>& vertexValues,
        const std::vector<VertexAttribute>& attributes,int indexType,const std::vector<GLuint>& indexValues, 
        bool isStatic,bool isVertexArray){
            vertices = std::make_unique<VertexData>(vertType,isStatic,vertexValues.size(),attributes);
            indices = std::make_unique<IndexData>(indexType,isStatic,indexValues.size());
            setVertices(vertexValues);
            setIndices(indexValues);
            this->isVertexArray = isVertexArray;           
        }

Mesh::Mesh (bool isGL30,bool isStatic, int maxVertices, int maxIndices, const std::vector<VertexAttribute>& attributes) :
        vertices(makeVertexBuffer(isGL30,isStatic, maxVertices, VertexAttributes(attributes))),
        indices(new IndexData(INDEX_BUFFER_OBJECT,isStatic, maxIndices)),isVertexArray(false){
}

Mesh::Mesh (bool isGL30,bool isStatic, int maxVertices, int maxIndices, const VertexAttributes& attributes) :
        vertices(makeVertexBuffer(isGL30,isStatic, maxVertices, attributes)),
        indices(new IndexData(INDEX_BUFFER_OBJECT,isStatic, maxIndices)),isVertexArray(false){
}

Mesh::Mesh (bool isGL30,bool staticVertices, bool staticIndices, int maxVertices, int maxIndices, const VertexAttributes& attributes) :
        vertices(makeVertexBuffer(isGL30,staticVertices, maxVertices, attributes)),
        indices(new IndexData(INDEX_BUFFER_OBJECT,staticIndices, maxIndices)),isVertexArray(false){
}

Mesh::Mesh (VertexDataType type, bool isStatic, int maxVertices, int maxIndices, const std::vector<VertexAttribute>& attributes) :
		Mesh(type, isStatic, maxVertices, maxIndices, VertexAttributes(attributes)){}

Mesh::Mesh (const VertexDataType& type, bool isStatic, int maxVertices, int maxIndices, const VertexAttributes& attributes):
        vertices(new VertexData(getType(type,true),isStatic, maxVertices, attributes)),
        indices(new IndexData(getType(type,false),isStatic, maxIndices)),isVertexArray(false){
}

void Mesh::bind (ShaderProgram& shader) {
		bind(shader, std::vector<int>());
}

void Mesh::bind (ShaderProgram& shader, const std::vector<int>& locations) {
		vertices->bind(shader, locations);
		if (indices->getNumIndices() > 0) indices->bind();
}

void Mesh::render (ShaderProgram& shader, int primitiveType, int offset, int count, bool autoBind) {
		if (count == 0) return;

		if (autoBind) bind(shader);

        int dataOffset = offset;
        if(!isVertexArray) dataOffset = offset * 2;
        
        if(indices->getNumIndices() > 0){
            if(!isVertexArray && count + offset > indices->getNumMaxIndices())
                SDL_Log("Mesh attempting to access memory outside of the index buffer (count: %i, offset: %i, max: %i)",count,dataOffset,indices->getNumMaxIndices());
            glDrawElements(primitiveType, count, /*GL_UNSIGNED_SHORT*/GL_UNSIGNED_INT, /*&indices->getBuffer().data()[dataOffset]*/0);
        }else glDrawArrays(primitiveType, offset, count);
        
		if (autoBind) unbind(shader);
	}

bool Mesh::hasVertexAttribute (int usage){
    VertexAttributes attributes = vertices->getAttributes();
    int len = attributes.size();
    for (int i = 0; i < len; i++)
        if (attributes.get(i).usage == usage) return true;
    return false;
}

VertexAttribute& Mesh::getVertexAttribute (int usage){
    VertexAttributes attributes = vertices->getAttributes();
    int len = attributes.size();
    for (int i = 0; i < len; i++)
        if (attributes.get(i).usage == usage) return attributes.get(i);
}

/*Mesh& Mesh::copy (bool isGL30,bool isStatic, bool removeDuplicates, const std::vector<int>& usage) {
		// TODO move this to a copy constructor?
		// TODO duplicate the buffers without double copying the data if possible.
		// TODO perhaps move this code to JNI if it turns out being too slow.
		const int vertexSize = getVertexSize() / 4;
		int numVertices = getNumVertices();
		std::vector<GLfloat> vertices = std::vector<GLfloat>(numVertices * vertexSize);
		getVertices(0, vertices->size(), vertices);
		std::vector<GLuint> checks;
		std::vector<VertexAttribute> attrs;
		int newVertexSize = 0;
		if (usage.size() != 0) {
			int size = 0;
			int as = 0;
			for (int i = 0; i < usage.size(); i++)
				if (hasVertexAttribute(usage[i])) {
					size += getVertexAttribute(usage[i]).numComponents;
					as++;
				}
			if (size > 0) {
				attrs = std::vector<VertexAttribute>(as);
				checks = std::vector<GLuint>(size);
				int idx = -1;
				int ai = -1;
				for (int i = 0; i < usage.size(); i++) {
					if (!hasVertexAttribute(usage[i])) continue;
                    VertexAttribute a = getVertexAttribute(usage[i]);
					for (int j = 0; j < a.numComponents; j++)
						checks[++idx] = (short)(a.offset + j);
					attrs[++ai] = a.copy();
					newVertexSize += a.numComponents;
				}
			}
		}
		if (checks.size() == 0) {
			checks = std::vector<GLuint>(vertexSize);
			for (short i = 0; i < vertexSize; i++)
				checks[i] = i;
			newVertexSize = vertexSize;
		}

		int numIndices = getNumIndices();
		std::vector<GLuint> indices;
		if (numIndices > 0) {
			indices = std::vector<GLuint>(numIndices);
			getIndices(indices);
			if (removeDuplicates || newVertexSize != vertexSize) {
				std::vector<GLfloat> tmp = std::vector<GLfloat>(vertices->size());
				int size = 0;
				for (int i = 0; i < numIndices; i++) {
					const int idx1 = indices[i] * vertexSize;
					short newIndex = -1;
					if (removeDuplicates) {
						for (short j = 0; j < size && newIndex < 0; j++) {
							const int idx2 = j * newVertexSize;
							bool found = true;
							for (int k = 0; k < checks.size() && found; k++) {
								if (tmp[idx2 + k] != vertices[idx1 + checks[k]]) found = false;
							}
							if (found) newIndex = j;
						}
					}
					if (newIndex > 0)
						indices[i] = newIndex;
					else {
						const int idx = size * newVertexSize;
						for (int j = 0; j < checks.size(); j++)
							tmp[idx + j] = vertices[idx1 + checks[j]];
						indices[i] = (short)size;
						size++;
					}
				}
				vertices = tmp;
				numVertices = size;
			}
		}

		if (attrs.size() == 0)
			return Mesh(isGL30,isStatic, vertices, indices, getVertexAttributes());
		return Mesh(isGL30,isStatic, vertices, indices, attrs);
		//result.setVertices(vertices, 0, numVertices * newVertexSize);
}*/

void Mesh::transform (Matrix4& matrix, const int start, const int count) {
		const VertexAttribute posAttr = getVertexAttribute(POSITION);
		const int posOffset = posAttr.offset / 4;
		const int stride = getVertexSize() / 4;
		const int numComponents = posAttr.numComponents;
		const int numVertices = getNumVertices();

		std::vector<GLfloat> vertices = std::vector<GLfloat>(count * stride);
		getVertices(start * stride, count * stride, vertices);
		// getVertices(0, vertices->size(), vertices);
		transform(matrix, vertices, stride, posOffset, numComponents, 0, count);
		// setVertices(vertices, 0, vertices->size());
		updateVertices(start * stride, vertices);
}

void Mesh::scale (float scaleX, float scaleY, float scaleZ) {
		const VertexAttribute posAttr = getVertexAttribute(POSITION);
		const int offset = posAttr.offset / 4;
		const int numComponents = posAttr.numComponents;
		const int numVertices = getNumVertices();
		const int vertexSize = getVertexSize() / 4;

		std::vector<GLfloat> vertices = std::vector<GLfloat>(numVertices * vertexSize);
		getVertices(vertices);

		int idx = offset;
		switch (numComponents) {
		case 1:
			for (int i = 0; i < numVertices; i++) {
				vertices[idx] *= scaleX;
				idx += vertexSize;
			}
			break;
		case 2:
			for (int i = 0; i < numVertices; i++) {
				vertices[idx] *= scaleX;
				vertices[idx + 1] *= scaleY;
				idx += vertexSize;
			}
			break;
		case 3:
			for (int i = 0; i < numVertices; i++) {
				vertices[idx] *= scaleX;
				vertices[idx + 1] *= scaleY;
				vertices[idx + 2] *= scaleZ;
				idx += vertexSize;
			}
			break;
		}

		setVertices(vertices);
}

float Mesh::calculateRadiusSquared (const float centerX, const float centerY, const float centerZ, int offset, int count,
		const Matrix4& transform) {
		int numIndices = getNumIndices();
		if (offset < 0 || count < 1 || offset + count > numIndices) SDL_Log("Not enough indices");

		std::vector<GLfloat> verts = vertices->getBuffer();
		std::vector<GLuint> index = indices->getBuffer();
		VertexAttribute posAttrib = getVertexAttribute(POSITION);
		const int posoff = posAttrib.offset / 4;
		const int vertexSize = vertices->getAttributes().vertexSize / 4;
		const int end = offset + count;

		float result = 0;

		switch (posAttrib.numComponents) {
		case 1:
			for (int i = offset; i < end; i++) {
				const int idx = index[i] * vertexSize + posoff;
				tmpV.set(verts[idx], 0, 0);
				tmpV.mul(transform);
				const float r = tmpV.sub(centerX, centerY, centerZ).len2();
				if (r > result) result = r;
			}
			break;
		case 2:
			for (int i = offset; i < end; i++) {
				const int idx = index[i] * vertexSize + posoff;
				tmpV.set(verts[idx], verts[idx+1], 0);
				tmpV.mul(transform);
				const float r = tmpV.sub(centerX, centerY, centerZ).len2();
				if (r > result) result = r;
			}
			break;
		case 3:
			for (int i = offset; i < end; i++) {
				const int idx = index[i] * vertexSize + posoff;
				tmpV.set(verts[idx], verts[idx+1], verts[idx+2]);
				tmpV.mul(transform);
				const float r = tmpV.sub(centerX, centerY, centerZ).len2();
				if (r > result) result = r;
			}
			break;
		}
		return result;
}

BoundingBox Mesh::extendBoundingBox (BoundingBox& out, int offset, int count, const Matrix4& transform) {
		const int numIndices = getNumIndices();
		const int numVertices = getNumVertices();
		const int max = numIndices == 0 ? numVertices : numIndices;
		if (offset < 0 || count < 1 || offset + count > max)
			SDL_Log("Invalid part specified ( offset=%i, count=%i, max=%i )",
                offset,count,max);

		const std::vector<GLfloat> verts = vertices->getBuffer();
		const std::vector<GLuint> index = indices->getBuffer();
		const VertexAttribute posAttrib = getVertexAttribute(POSITION);
		const int posoff = posAttrib.offset / 4;
		const int vertexSize = vertices->getAttributes().vertexSize / 4;
		const int end = offset + count;

		switch (posAttrib.numComponents) {
		case 1:
			if (numIndices > 0) {
				for (int i = offset; i < end; i++) {
					const int idx = index[i] * vertexSize + posoff;
					tmpV.set(verts[idx], 0, 0);
					tmpV.mul(transform);
					out.ext(tmpV);
				}
			} else {
				for (int i = offset; i < end; i++) {
					const int idx = i * vertexSize + posoff;
					tmpV.set(verts[idx], 0, 0);
					tmpV.mul(transform);
					out.ext(tmpV);
				}
			}
			break;
		case 2:
			if (numIndices > 0) {
				for (int i = offset; i < end; i++) {
					const int idx = index[i] * vertexSize + posoff;
					tmpV.set(verts[idx], verts[idx+1], 0);
					tmpV.mul(transform);
					out.ext(tmpV);
				}
			} else {
				for (int i = offset; i < end; i++) {
					const int idx = i * vertexSize + posoff;
					tmpV.set(verts[idx], verts[idx+1], 0);
					tmpV.mul(transform);
					out.ext(tmpV);
				}
			}
			break;
		case 3:
			if (numIndices > 0) {
				for (int i = offset; i < end; i++) {
					const int idx = index[idx] * vertexSize + posoff;
					tmpV.set(verts[idx], verts[idx+1], verts[idx+2]);
					tmpV.mul(transform);
					out.ext(tmpV);
				}
			} else {
				for (int i = offset; i < end; i++) {
					const int idx = i * vertexSize + posoff;
					tmpV.set(verts[idx], verts[idx+1], verts[idx+2]);
					tmpV.mul(transform);
					out.ext(tmpV);
				}
			}
			break;
		}
		return out;
}

void Mesh::calculateBoundingBox (BoundingBox& bbox) {
		int numVertices = getNumVertices();
		if (numVertices == 0) SDL_Log("No vertices defined");

		std::vector<GLfloat> verts = vertices->getBuffer();
		bbox.inf();
		VertexAttribute posAttrib = getVertexAttribute(POSITION);
		int offset = posAttrib.offset / 4;
		int vertexSize = vertices->getAttributes().vertexSize / 4;
		int idx = offset;

		switch (posAttrib.numComponents) {
		case 1:
			for (int i = 0; i < numVertices; i++) {
				bbox.ext(verts[idx], 0, 0);
				idx += vertexSize;
			}
			break;
		case 2:
			for (int i = 0; i < numVertices; i++) {
				bbox.ext(verts[idx], verts[idx+1], 0);
				idx += vertexSize;
			}
			break;
		case 3:
			for (int i = 0; i < numVertices; i++) {
				bbox.ext(verts[idx], verts[idx+1], verts[idx+2]);
				idx += vertexSize;
			}
			break;
		}
}

void Mesh::transformUV (Matrix3& matrix, const int start, const int count) {
		VertexAttribute posAttr = getVertexAttribute(TEXTURE_COORDINATES);
		const int offset = posAttr.offset / 4;
		const int vertexSize = getVertexSize() / 4;
		const int numVertices = getNumVertices();

		std::vector<float> vertices = std::vector<float>(numVertices * vertexSize);
		// TODO: getVertices(vertices, start * vertexSize, count * vertexSize);
		getVertices(0, vertices.size(), vertices);
		transformUV(matrix, vertices, vertexSize, offset, start, count);
		setVertices(vertices, 0, vertices.size());
		// TODO: setVertices(start * vertexSize, vertices, 0, vertices->size());
}
