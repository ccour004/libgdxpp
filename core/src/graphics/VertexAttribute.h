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

#include <string>
#include "VertexAttributes.h"
#include "glutils/ShaderProgram.h"

/** A single vertex attribute defined by its {@link Usage}, its number of components and its shader alias. The Usage is used
 * for uniquely identifying the vertex attribute from among its {@linkplain VertexAttributes} siblings. The number of components 
 * defines how many components the attribute has. The alias defines to which shader attribute this attribute should bind. The alias 
 * is used by a {@link Mesh} when drawing with a {@link ShaderProgram}. The alias can be changed at any time.
 * 
 * @author mzechner */
 class VertexAttribute {
    int usageIndex;
public:
	/** The attribute {@link Usage}, used for identification. **/
	 int usage;
	/** the number of components this attribute has **/
	 int numComponents;
	/** For fixed types, whether the values are normalized to either -1f and +1f (signed) or 0f and +1f (unsigned) */
	 bool normalized;
	/** the OpenGL type of each component, e.g. {@link GL20#GL_FLOAT} or {@link GL20#GL_UNSIGNED_BYTE}  */
	 int type;
	/** the offset of this attribute in bytes, don't change this! **/
	int offset;
	/** the alias for the attribute used in a {@link ShaderProgram} **/
	std::string alias;
	/** optional unit/index specifier, used for texture coordinates and bone weights **/
	int unit;
    
    VertexAttribute(){}

	/** Constructs a new VertexAttribute. The GL data type is automatically selected based on the usage.
	 * 
	 * @param usage The attribute {@link Usage}, used to select the {@link #type} and for identification.
	 * @param numComponents the number of components of this attribute, must be between 1 and 4.
	 * @param alias the alias used in a shader for this attribute. Can be changed after construction. */
	VertexAttribute (int usage, int numComponents, const std::string& alias)
    :VertexAttribute(usage, numComponents, alias, 0){}

	/** Constructs a new VertexAttribute. The GL data type is automatically selected based on the usage.
	 * 
	 * @param usage The attribute {@link Usage}, used to select the {@link #type} and for identification.
	 * @param numComponents the number of components of this attribute, must be between 1 and 4.
	 * @param alias the alias used in a shader for this attribute. Can be changed after construction.
	 * @param unit Optional unit/index specifier, used for texture coordinates and bone weights */
	VertexAttribute (int usage, int numComponents, const std::string& alias, int unit):
    VertexAttribute(usage, numComponents, usage == COLOR_PACKED ? GL_UNSIGNED_BYTE : GL_FLOAT, 
				usage == COLOR_PACKED, alias, unit){}

	/** Constructs a new VertexAttribute.
	 * 
	 * @param usage The attribute {@link Usage}, used for identification.
	 * @param numComponents the number of components of this attribute, must be between 1 and 4.
	 * @param type the OpenGL type of each component, e.g. {@link GL20#GL_FLOAT} or {@link GL20#GL_UNSIGNED_BYTE}. Since {@link Mesh}
	 * stores vertex data in 32bit floats, the total size of this attribute (type size times number of components) must be a 
	 * multiple of four.
	 * @param normalized For fixed types, whether the values are normalized to either -1f and +1f (signed) or 0f and +1f (unsigned) 
	 * @param alias The alias used in a shader for this attribute. Can be changed after construction. */
	VertexAttribute (int usage, int numComponents, int type, bool normalized, const std::string& alias):
    VertexAttribute(usage, numComponents, type, normalized, alias, 0){}
	
	/** Constructs a new VertexAttribute.
	 * 
	 * @param usage The attribute {@link Usage}, used for identification.
	 * @param numComponents the number of components of this attribute, must be between 1 and 4.
	 * @param type the OpenGL type of each component, e.g. {@link GL20#GL_FLOAT} or {@link GL20#GL_UNSIGNED_BYTE}. Since {@link Mesh}
	 * stores vertex data in 32bit floats, the total size of this attribute (type size times number of components) must be a 
	 * multiple of four bytes.
	 * @param normalized For fixed types, whether the values are normalized to either -1f and +1f (signed) or 0f and +1f (unsigned) 
	 * @param alias The alias used in a shader for this attribute. Can be changed after construction.
	 * @param unit Optional unit/index specifier, used for texture coordinates and bone weights */
	VertexAttribute (int usage, int numComponents, int type, bool normalized, const std::string& alias, int unit) {
		this->usage = usage;
		this->numComponents = numComponents;
		this->type = type;
		this->normalized = normalized;
		this->alias = alias;
		this->unit = unit;
		this->usageIndex = __builtin_ctz(usage);
	}
	
	/** @return A copy of this VertexAttribute with the same parameters. The {@link #offset} is not copied and must
	 * be recalculated, as is typically done by the {@linkplain VertexAttributes} that owns the VertexAttribute. */
	VertexAttribute copy (){
		return VertexAttribute(usage, numComponents, type, normalized, alias, unit);
	}

	static VertexAttribute position () {
		return VertexAttribute(POSITION, 3, ShaderProgram::POSITION_ATTRIBUTE);
	}

	static VertexAttribute texCoords (int unit) {
		return VertexAttribute(TEXTURE_COORDINATES, 2, ShaderProgram::TEXCOORD_ATTRIBUTE + std::to_string(unit), unit);
	}

	static VertexAttribute normal () {
		return VertexAttribute(NORMAL, 3, ShaderProgram::NORMAL_ATTRIBUTE);
	}
	
	static VertexAttribute colorPacked () {
		return VertexAttribute(COLOR_PACKED, 4, GL_UNSIGNED_BYTE, true, ShaderProgram::COLOR_ATTRIBUTE);
	}

	static VertexAttribute colorUnpacked () {
		return VertexAttribute(COLOR_UNPACKED, 4, GL_FLOAT, false, ShaderProgram::COLOR_ATTRIBUTE);
	}

	static VertexAttribute tangent () {
		return VertexAttribute(TANGENT, 3, ShaderProgram::TANGENT_ATTRIBUTE);
	}

	static VertexAttribute binormal () {
		return VertexAttribute(BINORMAL, 3,ShaderProgram::BINORMAL_ATTRIBUTE);
	}

	static VertexAttribute boneWeight (int unit) {
		return VertexAttribute(BONE_WEIGHT, 2, ShaderProgram::BONEWEIGHT_ATTRIBUTE + std::to_string(unit), unit);
	}

	/** Tests to determine if the passed object was created with the same parameters */
    bool operator ==(const VertexAttribute& obj){
        return obj.usage == usage && obj.numComponents == numComponents
        && obj.normalized == normalized && obj.type == type && obj.offset == offset
        && obj.alias == alias && obj.unit == unit && obj.usageIndex == usageIndex;
	}
    
    bool operator !=(const VertexAttribute& obj){
        return !(*this == obj);
    }

	/** @return A unique number specifying the usage index (3 MSB) and unit (1 LSB). */
	int getKey () {
		return (usageIndex << 8) + (unit & 0xFF);
	}
	
	/** @return How many bytes this attribute uses. */
	int getSizeInBytes () {
		switch (type) {
		case GL_FLOAT:
		case GL_FIXED:
			return 4 * numComponents;
		case GL_UNSIGNED_BYTE:
		case GL_BYTE:
			return numComponents;
		case GL_UNSIGNED_SHORT:
		case GL_SHORT:
			return 2 * numComponents;
		}
		return 0;
	}

	int hashCode () {
		int result = getKey();
		result = 541 * result + numComponents;
		result = 541 * result + std::hash<std::string>{}(alias);
		return result;
	}
};