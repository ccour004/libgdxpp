#include "ShaderProgram.h"

//Initialize
bool ShaderProgram::pedantic = true;
std::string ShaderProgram::prependVertexCode;
std::string ShaderProgram::prependFragmentCode;

std::map<std::string, std::vector<ShaderProgram>> ShaderProgram::shaders = std::map<std::string, std::vector<ShaderProgram>>();

const std::string ShaderProgram::POSITION_ATTRIBUTE = "a_position";
const std::string ShaderProgram::NORMAL_ATTRIBUTE = "a_normal";
const std::string ShaderProgram::COLOR_ATTRIBUTE = "a_color";
const std::string ShaderProgram::TEXCOORD_ATTRIBUTE = "a_texCoord";
const std::string ShaderProgram::TANGENT_ATTRIBUTE = "a_tangent";
const std::string ShaderProgram::BINORMAL_ATTRIBUTE = "a_binormal";
const std::string ShaderProgram::BONEWEIGHT_ATTRIBUTE = "a_boneWeight";

ShaderProgram::ShaderProgram (const std::string& vertexShader,const std::string& fragmentShader,const std::string& app) {
        std::stringstream vs,fs;
		if (prependVertexCode.length() > 0) vs<< prependVertexCode; 
        vs << vertexShader;
		if (prependFragmentCode.length() > 0) fs<< prependFragmentCode;
        fs << fragmentShader;

		vertexShaderSource = vs.str();
		fragmentShaderSource = fs.str();
		log = "";
        refCount = 0;

		compileShaders(vertexShaderSource, fragmentShaderSource);
		if (isCompiled()) {
			fetchAttributes();
			fetchUniforms();
			addManagedShader(app, *this);
		}
	}
    
	ShaderProgram::~ShaderProgram() {
		glUseProgram(0);
		glDeleteShader(vertexShaderHandle);
		glDeleteShader(fragmentShaderHandle);
		glDeleteProgram(program);
	}
    
    void ShaderProgram::compileShaders (const std::string& vertexShader,const std::string& fragmentShader) {
		vertexShaderHandle = loadShader(GL_VERTEX_SHADER, vertexShader);
		fragmentShaderHandle = loadShader(GL_FRAGMENT_SHADER, fragmentShader);

		if (vertexShaderHandle == -1 || fragmentShaderHandle == -1) {
			_compiled = false;
			return;
		}

		program = linkProgram(createProgram());
		if (program == -1) {
			_compiled = false;
			return;
		}

		_compiled = true;
	}
    
    int ShaderProgram::linkProgram (int program) {
		if (program == -1) return -1;

		glAttachShader(program, vertexShaderHandle);
		glAttachShader(program, fragmentShaderHandle);
		glLinkProgram(program);

		GLint params;
		glGetProgramiv(program, GL_LINK_STATUS,&params);
		if (params == GL_FALSE) {
            GLsizei* length;
            GLchar* cLog;
			glGetProgramInfoLog(program,std::numeric_limits<GLchar>::max(),length,cLog);
            log = cLog;
            delete length;
            delete cLog;
			return -1;
		}

		return program;
	}
    
    int ShaderProgram::loadShader (int type,const std::string& source) {
		GLuint shader = glCreateShader(type);
		if (shader == 0) return -1;
        GLint params;

        const GLchar* ob1 = source.c_str();
        glShaderSource(shader, 1, &ob1, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &params);

		if (params == GL_FALSE) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            GLchar* cLog = new char[logLength + 1];
			glGetShaderInfoLog(shader,logLength,NULL,cLog);
            SDL_Log("%s SHADER FAIL LOG (%i chars): %s <SOURCE: %s",GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT",
                logLength,cLog,source.c_str());
            //delete cLog;
            //delete ob1;
			return -1;
		}
        
        //delete ob1;
		return shader;
	}
    
	void ShaderProgram::fetchAttributes () {
		GLint params;
		glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
		int numAttributes = params;

        const GLsizei bufSize = 256;
        GLint sizes;
        GLenum typeParams;
        GLchar names[bufSize];
		attributeNames = std::vector<std::string>(numAttributes);

		for (int i = 0; i < numAttributes; i++) {
            GLsizei length;
            glGetActiveAttrib(program, i,bufSize,&length, &sizes,&typeParams,names);
            SDL_Log("Attribute #%d Type: %u Name: %s\n", i, typeParams, names);
			int location = glGetAttribLocation(program, names);
            std::string name(names);
            
			attributes[name] = location;
			attributeTypes[name] = typeParams;
			attributeSizes[name] = sizes;
			attributeNames[i] = name;
		}
	}
    
    void ShaderProgram::fetchUniforms () {
        GLint params;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params);
		int numUniforms = params;

        const GLsizei bufSize = 256;
        GLint sizes;
        GLenum typeParams;
        GLchar names[bufSize];
		uniformNames = std::vector<std::string>(numUniforms);

		for (int i = 0; i < numUniforms; i++) {
            GLsizei length;
            glGetActiveUniform(program, i,bufSize,&length, &sizes,&typeParams,names);
            SDL_Log("Uniform #%d Type: %u Name: %s\n", i, typeParams, names);
			int location = glGetUniformLocation(program, names);
            std::string name(names);
            
			uniforms[name] = location;
			uniformTypes[name] = typeParams;
			uniformSizes[name] = sizes;
			uniformNames[i] = name;
		}
	}
    
	void ShaderProgram::setAttributef (const std::string& name, float value1, float value2, float value3, float value4) {
		int location = fetchAttributeLocation(name);
		glVertexAttrib4f(location, value1, value2, value3, value4);
	}
    
	void ShaderProgram::enableVertexAttribute (int location) {
		checkManaged();
		glEnableVertexAttribArray(location);
	}
    
    void ShaderProgram::enableVertexAttribute (const std::string& name) {
		checkManaged();
		int location = fetchAttributeLocation(name);
		if (location == -1) return;
		glEnableVertexAttribArray(location);
	}
    
	void ShaderProgram::disableVertexAttribute (int location) {
		checkManaged();
		glDisableVertexAttribArray(location);
	}
    
	void ShaderProgram::disableVertexAttribute (const std::string& name) {
		checkManaged();
		int location = fetchAttributeLocation(name);
		if (location == -1) return;
		glDisableVertexAttribArray(location);
	}
    
    int ShaderProgram::createProgram () {
		int program = glCreateProgram();
		return program != 0 ? program : -1;
	}
    
	int ShaderProgram::fetchAttributeLocation (const std::string& name) {
        if(attributes.find(name) != attributes.end())
            return attributes.at(name);
        char* cName;
        int location = glGetAttribLocation(program, cName);
        if(location == -1 && pedantic) SDL_Log("no attribute with name '%s' in shader",name.c_str());
        attributes[std::string(cName)] = location;
        delete cName;
		return location;
	}
    
	void ShaderProgram::begin () {
		checkManaged();
		glUseProgram(program);
	}
    
	void ShaderProgram::end () {
		glUseProgram(0);
	}
    
    int ShaderProgram::fetchUniformLocation (const std::string& name, bool pedantic) {
        if(uniforms.find(name) != uniforms.end())
            return uniforms.at(name);
        char* cName;
        int location = glGetUniformLocation(program, cName);
        if(location == -1 && pedantic) SDL_Log("no uniform with name '%s' in shader",name.c_str());
        uniforms[std::string(cName)] = location;
        delete cName;
		return location;
	}
    
	void ShaderProgram::setVertexAttribute (int location, int size, int type, bool normalize, int stride) {
		checkManaged();
		glVertexAttribPointer(location, size, type, normalize, stride,((char *)NULL + (0)));
	}
    
	void ShaderProgram::setVertexAttribute (const std::string& name, int size, int type, bool normalize, int stride) {		
		checkManaged();
		int location = fetchAttributeLocation(name);
		if (location == -1) return;
		glVertexAttribPointer(location, size, type, normalize, stride,((char *)NULL + (0)));
	}
    
	void ShaderProgram::setVertexAttribute (int location, int size, int type, bool normalize, int stride, int buffer) {
		checkManaged();
		glVertexAttribPointer(location, size, type, normalize, stride, ((char *)NULL + (buffer)));
	}
    
	void ShaderProgram::setVertexAttribute (const std::string& name, int size, int type, bool normalize, int stride, int buffer) {
		checkManaged();
		int location = fetchAttributeLocation(name);
		if (location == -1) return;
		glVertexAttribPointer(location, size, type, normalize, stride, ((char *)NULL + (buffer)));
	}
    
	void ShaderProgram::setUniformMatrix4fv (int location, float* values,int length) {
		checkManaged();
		glUniformMatrix4fv(location, length / 16, false, values);
	}
    
	void ShaderProgram::setUniformMatrix4fv (const std::string& name, std::vector<float>& buffer, int count, bool transpose) {
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniformMatrix4fv(location, count, transpose, buffer.data());
	}
    
	void ShaderProgram::setUniformMatrix3fv (const std::string& name, const std::vector<float>& buffer, int count, bool transpose) {
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniformMatrix3fv(location, count, transpose, buffer.data());
	}
    
	void ShaderProgram::setUniform4fv (int location, float* values,int length) {
		checkManaged();
		glUniform4fv(location, length / 4, values);
	}
    
	void ShaderProgram::setUniform4fv (const std::string& name, float* values, int length) {
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform4fv(location, length / 4, values);
	}
    
	void ShaderProgram::setUniform3fv (int location, float* values,int length) {
		checkManaged();
		glUniform3fv(location, length / 3, values);
	}
    
	void ShaderProgram::setUniform3fv (const std::string& name, float* values, int length) {
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform3fv(location, length / 3, values);
	}
    
	void ShaderProgram::setUniform2fv (int location, float* values, int length) {
		checkManaged();
		glUniform2fv(location, length / 2, values);
	}
    
	void ShaderProgram::setUniform2fv (const std::string& name, float* values, int length) {
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform2fv(location, length / 2, values);
	}
    
	void ShaderProgram::setUniform1fv (int location, float* values, int length) {
		checkManaged();
		glUniform1fv(location, length, values);
	}
    
	void ShaderProgram::setUniform1fv (const std::string& name, float* values,int length) {
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform1fv(location, length, values);
	}
    
	void ShaderProgram::setUniformf (int location, float value1, float value2, float value3, float value4) {
		checkManaged();
		glUniform4f(location, value1, value2, value3, value4);
	}
    
	void ShaderProgram::setUniformf (int location, float value1, float value2, float value3) {		
		checkManaged();
		glUniform3f(location, value1, value2, value3);
	}
    
	void ShaderProgram::setUniformf (int location, float value1, float value2) {		
		checkManaged();
		glUniform2f(location, value1, value2);
	}
    
	void ShaderProgram::setUniformf (const std::string& name, float value1, float value2, float value3, float value4) {		
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform4f(location, value1, value2, value3, value4);
	}
    
	void ShaderProgram::setUniformf (const std::string& name, float value1, float value2, float value3) {		
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform3f(location, value1, value2, value3);
	}
    
	void ShaderProgram::setUniformf (const std::string& name, float value1, float value2) {		
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform2f(location, value1, value2);
	}
    
	void ShaderProgram::setUniformf (int location, float value) {		
		checkManaged();
		glUniform1f(location, value);
	}
    
	void ShaderProgram::setUniformf (const std::string& name, float value) {		
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform1f(location, value);
	}
    
	void ShaderProgram::setUniformi (int location, int value1, int value2, int value3, int value4) {
		checkManaged();
		glUniform4i(location, value1, value2, value3, value4);
	}
    
	void ShaderProgram::setUniformi (const std::string& name, int value1, int value2, int value3, int value4) {
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform4i(location, value1, value2, value3, value4);
	}
    
	void ShaderProgram::setUniformi (int location, int value1, int value2, int value3) {
		checkManaged();
		glUniform3i(location, value1, value2, value3);
	}
    
	void ShaderProgram::setUniformi (const std::string& name, int value) {
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform1i(location, value);
	}
    
	void ShaderProgram::setUniformi (int location, int value) {
		checkManaged();
		glUniform1i(location, value);
	}
    
	void ShaderProgram::setUniformi (const std::string& name, int value1, int value2) {
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform2i(location, value1, value2);
	}
    
	void ShaderProgram::setUniformi (int location, int value1, int value2) {
		checkManaged();
		glUniform2i(location, value1, value2);
	}
    
	void ShaderProgram::setUniformi (const std::string& name, int value1, int value2, int value3) {
		checkManaged();
		int location = fetchUniformLocation(name);
		glUniform3i(location, value1, value2, value3);
	}
    
    void ShaderProgram::setUniformMatrix (int location, const Matrix4& matrix, bool transpose) {
		checkManaged();
		glUniformMatrix4fv(location, 1, transpose, matrix.val.data());
	}
    
	void ShaderProgram::setUniformMatrix (int location, const Matrix3& matrix, bool transpose) {
		checkManaged();
		glUniformMatrix3fv(location, 1, transpose, matrix.val.data());
	}