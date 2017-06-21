#include "ShaderProgram.h"

//Initialize
bool ShaderProgram::pedantic = true;
std::string ShaderProgram::prependVertexCode = "";
std::string ShaderProgram::prependFragmentCode = "";

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

		compileShaders(vertexShader, fragmentShader);
		if (isCompiled()) {
			fetchAttributes();
			fetchUniforms();
            SDL_Log("Shader fetched all variables!");
			addManagedShader(app, *this);
		}
	}
    
int ShaderProgram::fetchUniformLocation (const std::string& name, bool pedantic) {
        int location;
        try{
            location = uniforms.at(name);
        }catch(...){
            location = glGetUniformLocation(program, name.c_str());
			if (location == -1 && pedantic) SDL_Log("no uniform with name '%s' in shader",name.c_str());
			uniforms[name] = location;
        }
		return location;
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