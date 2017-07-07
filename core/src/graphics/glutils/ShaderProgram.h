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
#include "../../math/Vector2.h"
#include "../../math/Vector3.h"
#include "../../math/Matrix3.h"
#include "../../math/Matrix4.h"
#include <limits>
#include <vector>
#include <map>

class Vector3;
class Vector2;
class Matrix3;
class Matrix4;
class ShaderProgram
{
    std::vector<ShaderProgram> managedResources = std::vector<ShaderProgram>();
protected:
    int createProgram ();
public:
    int getProgramID(){
        return program;
    }
	/** default name for position attributes **/
	static const std::string POSITION_ATTRIBUTE;
	/** default name for normal attributes **/
	static const std::string NORMAL_ATTRIBUTE;
	/** default name for color attributes **/
	static const std::string COLOR_ATTRIBUTE;
	/** default name for texcoords attributes, append texture unit number **/
	static const std::string TEXCOORD_ATTRIBUTE;
	/** default name for tangent attribute **/
	static const std::string TANGENT_ATTRIBUTE;
	/** default name for binormal attribute **/
	static const std::string BINORMAL_ATTRIBUTE;
	/** default name for boneweight attribute **/
    static const std::string BONEWEIGHT_ATTRIBUTE;
    /** flag indicating whether attributes & uniforms must be present at all times **/
	static bool pedantic;

	/** code that is always added to the vertex shader code, typically used to inject a #version line. Note that this is added
	 * as-is, you should include a newline (`\n`) if needed. */
	static std::string prependVertexCode;

	/** code that is always added to every fragment shader code, typically used to inject a #version line. Note that this is added
	 * as-is, you should include a newline (`\n`) if needed. */
	static std::string prependFragmentCode;
    
    /** @return whether this ShaderProgram compiled successfully. */
	bool isCompiled () {
		return _compiled;
	}
ShaderProgram(){}
ShaderProgram (const std::string& vertexShader,const std::string& fragmentShader,const std::string& app);
    
int fetchUniformLocation (const std::string& name, bool pedantic);

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param value the value */
	void setUniformi (const std::string& name, int value);

	void setUniformi (int location, int value);

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param value1 the first value
	 * @param value2 the second value */
	void setUniformi (const std::string& name, int value1, int value2);

	void setUniformi (int location, int value1, int value2);

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param value1 the first value
	 * @param value2 the second value
	 * @param value3 the third value */
	void setUniformi (const std::string& name, int value1, int value2, int value3);

	void setUniformi (int location, int value1, int value2, int value3);

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param value1 the first value
	 * @param value2 the second value
	 * @param value3 the third value
	 * @param value4 the fourth value */
	void setUniformi (const std::string& name, int value1, int value2, int value3, int value4);

	void setUniformi (int location, int value1, int value2, int value3, int value4);

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param value the value */
	void setUniformf (const std::string& name, float value);

	void setUniformf (int location, float value);

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param value1 the first value
	 * @param value2 the second value */
	void setUniformf (const std::string& name, float value1, float value2);

	void setUniformf (int location, float value1, float value2);

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param value1 the first value
	 * @param value2 the second value
	 * @param value3 the third value */
	void setUniformf (const std::string& name, float value1, float value2, float value3);

	void setUniformf (int location, float value1, float value2, float value3);

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param value1 the first value
	 * @param value2 the second value
	 * @param value3 the third value
	 * @param value4 the fourth value */
	void setUniformf (const std::string& name, float value1, float value2, float value3, float value4);

	void setUniformf (int location, float value1, float value2, float value3, float value4);

	void setUniform1fv (const std::string& name, float* values,int length);

	void setUniform1fv (int location, float* values, int length);

	void setUniform2fv (const std::string& name, float* values, int length);

	void setUniform2fv (int location, float* values, int length);

	void setUniform3fv (const std::string& name, float* values, int length);

	void setUniform3fv (int location, float* values,int length);

	void setUniform4fv (const std::string& name, float* values, int length);

	void setUniform4fv (int location, float* values,int length);

	/** Sets the uniform matrix with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param matrix the matrix */
	void setUniformMatrix (const std::string& name, const Matrix4& matrix) {
		setUniformMatrix(name, matrix, false);
	}

	/** Sets the uniform matrix with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param matrix the matrix
	 * @param transpose whether the matrix should be transposed */
	void setUniformMatrix (const std::string& name, const Matrix4& matrix, bool transpose) {    
		setUniformMatrix(fetchUniformLocation(name), matrix, transpose);
	}

	void setUniformMatrix (int location, const Matrix4& matrix) {
		setUniformMatrix(location, matrix, false);
	}

	void setUniformMatrix (int location, const Matrix4& matrix, bool transpose);

	/** Sets the uniform matrix with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param matrix the matrix */
	void setUniformMatrix (const std::string& name, Matrix3& matrix) {
		setUniformMatrix(name, matrix, false);
	}

	/** Sets the uniform matrix with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param matrix the matrix
	 * @param transpose whether the uniform matrix should be transposed */
	void setUniformMatrix (const std::string& name, const Matrix3& matrix, bool transpose) {
		setUniformMatrix(fetchUniformLocation(name), matrix, transpose);
	}

	void setUniformMatrix (int location, const Matrix3& matrix) {
		setUniformMatrix(location, matrix, false);
	}

	void setUniformMatrix (int location, const Matrix3& matrix, bool transpose);

	/** Sets an array of uniform matrices with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param buffer buffer containing the matrix data
	 * @param transpose whether the uniform matrix should be transposed */
	void setUniformMatrix3fv (const std::string& name, const std::vector<float>& buffer, int count, bool transpose);

	/** Sets an array of uniform matrices with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param buffer buffer containing the matrix data
	 * @param transpose whether the uniform matrix should be transposed */
	void setUniformMatrix4fv (const std::string& name, std::vector<float>& buffer, int count, bool transpose);

	void setUniformMatrix4fv (int location, float* values,int length);

	void setUniformMatrix4fv (const std::string& name, float* values,int length) {
		setUniformMatrix4fv(fetchUniformLocation(name), values, length);
	}

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param values x and y as the first and second values respectively */
	void setUniformf (const std::string& name, const Vector2& values) {
		setUniformf(name, values.x, values.y);
	}

	void setUniformf (int location, const Vector2& values) {
		setUniformf(location, values.x, values.y);
	}

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param values x, y and z as the first, second and third values respectively */
	void setUniformf (const std::string& name, const Vector3& values) {
		setUniformf(name, values.x, values.y, values.z);
	}

	void setUniformf (int location, Vector3 values) {
		setUniformf(location, values.x, values.y, values.z);
	}

	/** Sets the uniform with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the name of the uniform
	 * @param values r, g, b and a as the first through fourth values respectively */
	void setUniformf (const std::string& name, Color values) {
		setUniformf(name, values.r, values.g, values.b, values.a);
	}

	void setUniformf (int location, Color values) {
		setUniformf(location, values.r, values.g, values.b, values.a);
	}

	/** Sets the vertex attribute with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the attribute name
	 * @param size the number of components, must be >= 1 and <= 4
	 * @param type the type, must be one of GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
	 *           GL_UNSIGNED_SHORT,GL_FIXED, or GL_FLOAT. GL_FIXED will not work on the desktop
	 * @param normalize whether fixed point data should be normalized. Will not work on the desktop
	 * @param stride the stride in bytes between successive attributes
	 * @param buffer the buffer containing the vertex attributes. */
	void setVertexAttribute (const std::string& name, int size, int type, bool normalize, int stride, int buffer);

	void setVertexAttribute (int location, int size, int type, bool normalize, int stride, int buffer);

	/** Sets the vertex attribute with the given name. The {@link ShaderProgram} must be bound for this to work.
	 * 
	 * @param name the attribute name
	 * @param size the number of components, must be >= 1 and <= 4
	 * @param type the type, must be one of GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
	 *           GL_UNSIGNED_SHORT,GL_FIXED, or GL_FLOAT. GL_FIXED will not work on the desktop
	 * @param normalize whether fixed point data should be normalized. Will not work on the desktop
	 * @param stride the stride in bytes between successive attributes
	 * @param offset byte offset into the vertex buffer object bound to GL_ARRAY_BUFFER. */
	void setVertexAttribute (const std::string& name, int size, int type, bool normalize, int stride);

	void setVertexAttribute (int location, int size, int type, bool normalize, int stride);

	/** Makes OpenGL ES 2.0 use this vertex and fragment shader pair. When you are done with this shader you have to call
	 * {@link ShaderProgram#end()}. */
	void begin ();

	/** Disables this shader. Must be called when one is done with the shader. Don't mix it with dispose, that will release the
	 * shader resources. */
	void end ();

	/** Disposes all resources associated with this shader. Must be called when the shader is no longer used. */
	~ShaderProgram();

	/** Disables the vertex attribute with the given name
	 * 
	 * @param name the vertex attribute name */
	void disableVertexAttribute (const std::string& name);

	void disableVertexAttribute (int location);

	/** Enables the vertex attribute with the given name
	 * 
	 * @param name the vertex attribute name */
	void enableVertexAttribute (const std::string& name);

	void enableVertexAttribute (int location);

	/** Invalidates all shaders so the next time they are used new handles are generated
	 * @param app */
	static void invalidateAllShaderPrograms (const std::string& app) {
		std::vector<ShaderProgram> shaderArray = shaders[app];
		for (int i = 0; i < shaderArray.size(); i++) {
			shaderArray[i].invalidated = true;
			shaderArray[i].checkManaged();
		}
	}

	static void clearAllShaderPrograms (const std::string& app) {
		shaders.erase(app);
	}

	static std::string getManagedStatus () {
		std::stringstream builder;
		builder<<"Managed shaders/app: { ";
        for(auto const& shaderKey:shaders)
            builder<<shaders[shaderKey.first].size()<<" ";
		builder<<"}";
		return builder.str();
	}

	/** @return the number of managed shader programs currently loaded */
	static int getNumManagedShaderPrograms (const std::string& app) {
		return shaders[app].size();
	}

	/** Sets the given attribute
	 * 
	 * @param name the name of the attribute
	 * @param value1 the first value
	 * @param value2 the second value
	 * @param value3 the third value
	 * @param value4 the fourth value */
	void setAttributef (const std::string& name, float value1, float value2, float value3, float value4);

	/** @param name the name of the attribute
	 * @return whether the attribute is available in the shader */
	bool hasAttribute (const std::string& name) {
		return attributes.count(name);
	}

	/** @param name the name of the attribute
	 * @return the type of the attribute, one of {@link GL20#GL_FLOAT}, {@link GL20#GL_FLOAT_VEC2} etc. */
	int getAttributeType (const std::string& name) {
        try{return attributeTypes.at(name);}
        catch(std::out_of_range){}
		return 0;
	}

	/** @param name the name of the attribute
	 * @return the location of the attribute or -1. */
	int getAttributeLocation (const std::string& name) {
        try{return attributes.at(name);}
        catch(std::out_of_range){}
		return -1;
	}

	/** @param name the name of the attribute
	 * @return the size of the attribute or 0. */
	int getAttributeSize (const std::string& name) {
        try{return attributeSizes.at(name);}
        catch(std::out_of_range){}
		return 0;
	}

	/** @param name the name of the uniform
	 * @return whether the uniform is available in the shader */
	bool hasUniform (const std::string& name) {
		return uniforms.count(name);
	}

	/** @param name the name of the uniform
	 * @return the type of the uniform, one of {@link GL20#GL_FLOAT}, {@link GL20#GL_FLOAT_VEC2} etc. */
	int getUniformType (const std::string& name) {
        try{return uniformTypes.at(name);}
        catch(std::out_of_range){}
		return 0;
	}

	/** @param name the name of the uniform
	 * @return the location of the uniform or -1. */
	int getUniformLocation (const std::string& name) {
        try{return uniforms.at(name);}
        catch(std::out_of_range){}
		return -1;
	}

	/** @param name the name of the uniform
	 * @return the size of the uniform or 0. */
	int getUniformSize (const std::string& name) {
        try{return uniformSizes.at(name);}
        catch(std::out_of_range){}
		return 0;
	}

	/** @return the attributes */
	std::vector<std::string> getAttributes () {
		return attributeNames;
	}

	/** @return the uniforms */
	std::vector<std::string> getUniforms () {
		return uniformNames;
	}

	/** @return the source of the vertex shader */
	std::string getVertexShaderSource () {
		return vertexShaderSource;
	}

	/** @return the source of the fragment shader */
	std::string getFragmentShaderSource () {
		return fragmentShaderSource;
    }
private:
	/** the list of currently available shaders **/
	static std::map<std::string, std::vector<ShaderProgram>> shaders;

	/** the log **/
	std::string log;

	/** whether this program compiled successfully **/
	bool _compiled;

	/** uniform lookup **/
	std::map<std::string,int> uniforms;

	/** uniform types **/
	std::map<std::string,int> uniformTypes;

	/** uniform sizes **/
	std::map<std::string,int> uniformSizes;

	/** uniform names **/
	std::vector<std::string> uniformNames;

	/** attribute lookup **/
	std::map<std::string,int> attributes;

	/** attribute types **/
	std::map<std::string,int> attributeTypes;

	/** attribute sizes **/
	std::map<std::string,int> attributeSizes;

	/** attribute names **/
	std::vector<std::string> attributeNames;

	/** program handle **/
	int program;

	/** vertex shader handle **/
	int vertexShaderHandle;

	/** fragment shader handle **/
	int fragmentShaderHandle;

	/** matrix float buffer **/
	//Matrix4* matrix;

	/** vertex shader source **/
	std::string vertexShaderSource;

	/** fragment shader source **/
	std::string fragmentShaderSource;

	/** whether this shader was invalidated **/
	bool invalidated;

	/** reference count **/
    int refCount;
    
    void compileShaders (const std::string& vertexShader,const std::string& fragmentShader);
    
    void fetchUniforms ();

	void fetchAttributes ();
    
    void addManagedShader (const std::string& app, const ShaderProgram& shaderProgram) {
        auto search = shaders.find(app);
        if(search == shaders.end()) managedResources = std::vector<ShaderProgram>();
        else managedResources = shaders[search->first];
		managedResources.push_back(shaderProgram);
		shaders[app] = managedResources;
	}
    
    int loadShader (int type,const std::string& source);
    
    int linkProgram (int program);
    
    /*ShaderProgram (FileHandle vertexShader, FileHandle fragmentShader) {
		this(vertexShader.readstd::string(), fragmentShader.readstd::string());
	}*/

	int fetchAttributeLocation (const std::string& name);

	int fetchUniformLocation (const std::string& name) {
		return fetchUniformLocation(name, pedantic);
	}
    
    void checkManaged () {
		if (invalidated) {
			compileShaders(vertexShaderSource, fragmentShaderSource);
			invalidated = false;
		}
	}
};

