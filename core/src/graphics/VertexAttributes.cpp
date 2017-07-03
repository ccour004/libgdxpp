#include "VertexAttributes.h"
#include "VertexAttribute.h"

	 VertexAttributes::VertexAttributes (const std::vector<VertexAttribute>& attributes) {
		if (attributes.size() == 0) SDL_Log("attributes must be >= 1");

		std::vector<VertexAttribute> list = std::vector<VertexAttribute>(attributes.size());
		for (int i = 0; i < attributes.size(); i++)
			list[i] = attributes[i];

		this->attributes = list;
		vertexSize = calculateOffsets();
	}
    
	 int VertexAttributes::compareTo (VertexAttributes o) {
		if (attributes.size() != o.attributes.size()) return attributes.size() - o.attributes.size();
		long m1 = getMask();
		long m2 = o.getMask();
		if (m1 != m2) return m1 < m2 ? -1 : 1;
		for (int i = attributes.size() - 1; i >= 0; --i) {
			VertexAttribute va0 = attributes[i];
			VertexAttribute va1 = o.attributes[i];
			if (va0.usage != va1.usage) return va0.usage - va1.usage;
			if (va0.unit != va1.unit) return va0.unit - va1.unit;
			if (va0.numComponents != va1.numComponents) return va0.numComponents - va1.numComponents;
			if (va0.normalized != va1.normalized) return va0.normalized ? 1 : -1;
			if (va0.type != va1.type) return va0.type - va1.type;
		}
		return 0;
	}
    
	 long VertexAttributes::getMask () {
		if (mask == -1) {
			long result = 0;
			for (int i = 0; i < attributes.size(); i++) {
				result |= attributes[i].usage;
			}
			mask = result;
		}
		return mask;
	}
    
	 int VertexAttributes::hashCode () {
		long result = 61 * attributes.size();
		for (int i = 0; i < attributes.size(); i++)
			result = result * 61 + attributes[i].hashCode();
		return (int)(result ^ (result >> 32));
	}
    
	 bool VertexAttributes::operator==(VertexAttributes& obj) {
        if(obj.size() != attributes.size()) return false;
		for(int i = 0;i < attributes.size();i++)
            if(obj.attributes[i] != attributes[i])
                return false;
        return true;
	}
    
	 int VertexAttributes::findByUsage (int usage) {
		int len = size();
		for (int i = 0; i < len; i++)
			if (get(i).usage == usage) return i;
		return -1;
	}
    
	 int VertexAttributes::getOffset (int usage, int defaultIfNotFound) {
		int vertexAttribute = findByUsage(usage);
		if (vertexAttribute == -1) return defaultIfNotFound;
		return get(vertexAttribute).offset / 4;
	}


    int VertexAttributes::calculateOffsets () {
		int count = 0;
		for (int i = 0; i < attributes.size(); i++) {
			attributes[i].offset = count;
			count += attributes[i].getSizeInBytes();
		}
		return count;
	}
    
	 std::string VertexAttributes::toString () {
		std::stringstream builder;
		builder<<"[";
		for (int i = 0; i < attributes.size(); i++) {
			builder<<"(";
			builder<<attributes[i].alias;
			builder<<", ";
			builder<<attributes[i].usage;
			builder<<", ";
			builder<<attributes[i].numComponents;
			builder<<", ";
			builder<<attributes[i].offset;
			builder<<")";
			builder<<"\n";
		}
		builder<<"]";
		return builder.str();
	}

