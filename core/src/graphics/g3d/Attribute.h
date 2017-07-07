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
#include <string>

/** Extend this class to implement a material attribute. Register the attribute type by statically calling the
 * {@link #register(String)} method, whose return value should be used to instantiate the attribute. A class can implement
 * multiple types
 * @author Xoppa */
class Attribute{
protected:
	/** Call this method to register a custom attribute type, see the wiki for an example. If the alias already exists, then that ID
	 * will be reused. The alias should be unambiguously and will by default be returned by the call to {@link #toString()}.
	 * @param alias The alias of the type to register, must be different for each dirrect type, will be used for debugging
	 * @return the ID of the newly registered type, or the ID of the existing type if the alias was already registered */
	static long registerType (const std::string& alias) {
		long result = getAttributeType(alias);
		if (result > 0) return result;
		types.push_back(alias);
		return (1L << (types.size() - 1));
	}
    
	Attribute (const long type) {
		this->type = type;
		this->typeBit = __builtin_ctz(type);
	}

private:
	/** The registered type aliases */
	static std::vector<std::string> types;
    int typeBit;
public:
	/** @return The ID of the specified attribute type, or zero if not available */
	static long getAttributeType (const std::string& alias) {
		for (int i = 0; i < types.size(); i++)
			if (types[i] == alias) return (1L << i);
		return 0;
	}

	/** @return The alias of the specified attribute type, or null if not available. */
	static std::string getAttributeAlias (const long type) {
		int idx = -1;
		while (type != 0 && ++idx < 63 && (((type >> idx) & 1) == 0))
			;
		return (idx >= 0 && idx < types.size()) ? types[idx] : NULL;
	}

	/** The type of this attribute */
	long type;

	/** @return An exact copy of this attribute */
	Attribute copy ();

	bool operator== (Attribute& obj) {
		return obj.hashCode() == hashCode();
	}

	std::string toString () {
		return getAttributeAlias(type);
	}

	int hashCode () {
		return 7489 * typeBit;
	}
};