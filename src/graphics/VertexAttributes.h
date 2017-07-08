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
#include <sstream>
        	/** The usage of a vertex attribute.
	 * 
	 * @author mzechner */
#define POSITION 1
#define COLOR_UNPACKED 2
#define COLOR_PACKED 4
#define NORMAL 8
#define TEXTURE_COORDINATES 16
#define GENERIC 32
#define BONE_WEIGHT 64
#define TANGENT 128
#define BINORMAL 256

/** Instances of this class specify the vertex attributes of a mesh. VertexAttributes are used by {@link Mesh} instances to define
 * its vertex structure. Vertex attributes have an order. The order is specified by the order they are added to this class.
 * 
 * @author mzechner, Xoppa */
class VertexAttribute;
class VertexAttributes{
    	/** the attributes in the order they were specified **/
	std::vector<VertexAttribute> attributes;
    
    /** cache of the value calculated by {@link #getMask()} **/
	long mask = -1;
    
    int calculateOffsets ();
public:
	/** the size of a single vertex in bytes **/
	int vertexSize;
    
    VertexAttributes(){}

	/** Constructor, sets the vertex attributes in a specific order */
	 VertexAttributes (const std::vector<VertexAttribute>& attributes);

	/** Returns the offset for the first VertexAttribute with the specified usage.
	 * @param usage The usage of the VertexAttribute. */
	 int getOffset (int usage, int defaultIfNotFound);
	
	/** Returns the offset for the first VertexAttribute with the specified usage.
	 * @param usage The usage of the VertexAttribute. */
	 int getOffset (int usage) {
		return getOffset(usage, 0);
	}

	/** Returns the first VertexAttribute for the given usage.
	 * @param usage The usage of the VertexAttribute to find. */
	 int findByUsage (int usage);

	/** @return the number of attributes */
	 int size () {
		return attributes.size();
	}

	/** @param index the index
	 * @return the VertexAttribute at the given index */
	 VertexAttribute& get (int index) {
		return attributes[index];
	}

	 std::string toString ();
	
	 bool operator==(VertexAttributes& obj);

	 int hashCode ();

	/** Calculates a mask based on the contained {@link VertexAttribute} instances. The mask is a bit-wise or of each attributes
	 * {@link VertexAttribute#usage}.
	 * @return the mask */
	 long getMask ();

	 int compareTo (VertexAttributes o);
};