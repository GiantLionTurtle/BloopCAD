
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Shader.hpp"
#include "GLCall.hpp"
#include <Utils/File_util.hpp>

#include <fstream>
#include <filesystem>

Shader::Shader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource)
	: mLastUsedAtFrame(0)
{
	mRendererID = createShader(vertexShaderSource, fragmentShaderSource);
}
Shader::Shader(unsigned int id)
	: mRendererID(id)
	, mLastUsedAtFrame(0)
{

}
Shader::Shader()
	: mRendererID(0)
	, mLastUsedAtFrame(0)
{
	
}
Shader::~Shader()
{
	if(valid()) {
		GLCall(glDeleteShader(mRendererID));
	}
}

Shader Shader::fromFiles(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath)
{
	// Aquire the source code
	std::string vertexShaderSource    = readFromFile(vertexShaderFilePath);
	std::string fragmentShaderSource  = readFromFile(fragmentShaderFilePath);

	// Actually create the Shader
	return Shader(createShader(vertexShaderSource, fragmentShaderSource));
}
Shader Shader::fromFiles(std::vector<ShaderSource> ShaderPaths)
{
	if(ShaderPaths.size() < 2 || ShaderPaths.size() > 5) { // Those are invalid pipelines
		LOG_WARNING("Invalid number of paths given, empty Shader returned");
		return Shader();
	}

	// Kind of dirty, but ShaderPaths contains path to source files and it latter contains the actual source code
	for(int i = 0; i < ShaderPaths.size(); ++i) {
		// Aquire the source code
		ShaderPaths[i].source = readFromFile(ShaderPaths[i].source);
	}

	// Create the Shader
	return Shader(createShader(ShaderPaths));
}
Shader Shader::fromFile(std::string const& resourceFilePath)
{
	std::ifstream stream(resourceFilePath);
	if(!stream) // Check if file exists
		return Shader();

	// Consider every line of the file as a path to source code
	int i = 0;
	std::string filePaths[3];
	std::string line;
	while(stream>>line && i < 3) {
		filePaths[i] = line;
	}

	// Create relative paths given the file's location and the executable's location
	std::string resourceFolderPath = "";
	size_t endOfFolder_loc = resourceFilePath.find_last_of("/\\");
	if (endOfFolder_loc != std::string::npos)
		resourceFolderPath = resourceFilePath.substr(0, endOfFolder_loc);

	if(i == 1) { // It only support vertex-fragment pipeline, in a very dumb way
		return fromFiles(
			std::filesystem::path(resourceFolderPath).append(filePaths[0]).string(), 
			std::filesystem::path(resourceFolderPath).append(filePaths[1]).string());
	} else {
		LOG_WARNING("more than two Shaders stages from single file not supported, empty Shader returned.");
	}

	// If file is invalid, return an empty Shader
	return Shader();
}

Shader* Shader::fromFiles_ptr(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath)
{
	// Aquire the source code, might be a bit redundant, from the non-ptr version, but I am uncertain of how to reuse the code
	// Maybe create the Shader normaly and use it's id to create the pointer while ensuring that the destructor doesn't deallocate the handle?
	std::string vertexShaderSource    = readFromFile(vertexShaderFilePath);
	std::string fragmentShaderSource  = readFromFile(fragmentShaderFilePath);

	// Create the Shader
	return new Shader(createShader(vertexShaderSource, fragmentShaderSource));
}
Shader* Shader::fromFiles_ptr(std::vector<ShaderSource> ShaderPaths)
{
	// Prety much the same as the non-ptr version...
	if(ShaderPaths.size() < 2 || ShaderPaths.size() > 5) {
		LOG_WARNING("Invalid number of paths in Shader::fromFiles, empty Shader returned");
		return nullptr;
	}

	// Just as wack with the path => source thing
	for(int i = 0; i < ShaderPaths.size(); ++i) {
		ShaderPaths[i].source = readFromFile(ShaderPaths[i].source);
	}

	// Create the Shader
	return new Shader(createShader(ShaderPaths));
}
Shader* Shader::fromFile_ptr(std::string const& resourceFilePath)
{
	std::ifstream stream(resourceFilePath);
	if(!stream) // Ensure that the file exist, in the ptr version, it has the luxury of returning nullptr
		return nullptr;

	// Putting each line as a path
	int i = 0;
	std::string filePaths[3];
	std::string line;
	while(stream>>line && i < 3) {
		filePaths[i] = line;
	}

	// Constructing paths relative to the executable
	std::string resourceFolderPath = "";
	size_t endOfFolder_loc = resourceFilePath.find_last_of("/\\");
	if (endOfFolder_loc != std::string::npos)
		resourceFolderPath = resourceFilePath.substr(0, endOfFolder_loc);

	if(i == 1) {
		return fromFiles_ptr(
			std::filesystem::path(resourceFolderPath).append(filePaths[0]).string(), 
			std::filesystem::path(resourceFolderPath).append(filePaths[1]).string());
	} else {
		LOG_WARNING("More than two Shaders stages from single file not supported, empty Shader returned.");
	}

	// Return nullptr if file is invalid	
	return nullptr;
}

void Shader::bind() const
{
	GLCall(glUseProgram(mRendererID)); // The abstraction can make the API more consistent (bind everywhere)
}
void Shader::unbind() const
{
#ifndef RELEASE_MODE // No unecesary bind in release mode, but useful in debug
	GLCall(glUseProgram(0));
#endif
}

unsigned int Shader::compileShader(unsigned int type, std::string const& source)
{
	// Create, assign source and compile Shader 
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	// Check errors and log them
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if(result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		std::string message;
		message.reserve(length);
		GLCall(glGetShaderInfoLog(id, length, &length, (GLchar*)message.c_str()));
		std::string type_str = " UNKNOWN";
		switch(type) {
		case GL_VERTEX_SHADER:
			type_str = "vertex shader";
			break;
		case GL_TESS_CONTROL_SHADER:
			type_str = "tess control shader";
			break;
		case GL_TESS_EVALUATION_SHADER:
			type_str = "tess evaluation shader";
			break;
		case GL_GEOMETRY_SHADER:
			type_str = "geometry shader";
			break;
		case GL_FRAGMENT_SHADER:
			type_str = "fragment shader";
			break;
		}
		LOG_WARNING("Failed to compile " +
			type_str
			+ " Shader: " + message);
		return 0;
	}

	// Return the handle to the compiled Shader
	return id;
}
unsigned int Shader::createShader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource)
{
	// Compile both expected Shaders
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vertexShader   = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	if(!vertexShader || !fragmentShader) { // Check for validity
		return 0;
	}

	// Link Shader program
	GLCall(glAttachShader(program, vertexShader));
	GLCall(glAttachShader(program, fragmentShader));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	// Check & log errors
	int result;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if(result == GL_FALSE)
	{
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		std::string message;
		message.reserve(length);
		GLCall(glGetProgramInfoLog(program, length, &length, (GLchar*)message.c_str()));
		LOG_WARNING("Failed to link Shader program: \"" + message + "\"");

		GLCall(glDetachShader(program, vertexShader));
		GLCall(glDetachShader(program, fragmentShader));

		GLCall(glDeleteShader(vertexShader));
		GLCall(glDeleteShader(fragmentShader));

		GLCall(glDeleteProgram(program));

		return 0;
	}

	// Delete individual parts of the Shader program to not clog up openGL
	GLCall(glDetachShader(program, vertexShader));
	GLCall(glDetachShader(program, fragmentShader));

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));

	// Return the Shader program ID
	return program;
}
unsigned int Shader::createShader(std::vector<ShaderSource> const& ShaderSources)
{
	if(ShaderSources.size() < 2 || ShaderSources.size() > 5) { // Check if the number of Shaders in the pipeline is valid, might be a bit redundant..
		LOG_WARNING("Invalid number of paths in Shader::fromFiles, empty Shader returned");
		return 0;
	}

	// Compile all Shaders (max of 5 in a valid pipeling)
	unsigned int Shaders[5];
	for(int i = 0; i < ShaderSources.size(); ++i) {
		Shaders[i] = compileShader(ShaderSources[i].type, ShaderSources[i].source);
		if(!Shaders[i]) {
			LOG_WARNING("could not compile Shader stage " + std::to_string(i) + ", empty Shader returned");
		}
	}

	// Create Shader program and link Shaders
	GLCall(unsigned int program = glCreateProgram());	
	// TODO: check if for loops can be combined
	for(int i = 0; i < ShaderSources.size(); ++i) {
		GLCall(glAttachShader(program, Shaders[i]));
	}
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	// Check and log errors
	int result;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if(result == GL_FALSE)
	{
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		std::string message;
		message.reserve(length);
		GLCall(glGetProgramInfoLog(program, length, &length, (GLchar *)message.c_str()));
		LOG_WARNING("Failed to link Shader program: \"" + message + "\"");

		// Deallocate and delete everything if there was an error
		// TODO: check if for loops can be combined
		for(int i = 0; i < ShaderSources.size(); ++i) {
			GLCall(glDetachShader(program, Shaders[i]));
		}
		for(int i = 0; i < ShaderSources.size(); ++i) {
			GLCall(glDeleteShader(Shaders[i]));
		}

		GLCall(glDeleteProgram(program));

		return 0;
	}

	// Delete individual parts of the Shader program to not clog up openGL
	// TODO: check if for loops can be combined
	for(int i = 0; i < ShaderSources.size(); ++i) {
		GLCall(glDetachShader(program, Shaders[i]));
	}
	for(int i = 0; i < ShaderSources.size(); ++i) {
		GLCall(glDeleteShader(Shaders[i]));
	}

	// Return the created Shader program
	return program;
}

void Shader::setUniform1i(std::string const& name, int v0)
{
	GLCall_msg(glUniform1i(getUniformLocation(name), v0), "Uniform name: "<<name);
}
void Shader::setUniform1f(std::string const& name, float v0)
{
	GLCall_msg(glUniform1f(getUniformLocation(name), v0), "Uniform name: "<<name);
}
void Shader::setUniform2f(std::string const& name, float v0, float v1)
{
	GLCall_msg(glUniform2f(getUniformLocation(name), v0, v1), "Uniform name: "<<name);
}
void Shader::setUniform2f(std::string const& name, glm::vec2 vec2)
{
	GLCall_msg(glUniform2f(getUniformLocation(name), vec2.x, vec2.y), "Uniform name: "<<name);
}
void Shader::setUniform3f(std::string const& name, float v0, float v1, float v2)
{
	GLCall_msg(glUniform3f(getUniformLocation(name), v0, v1, v2), "Uniform name: "<<name);
}
void Shader::setUniform3f(std::string const& name, glm::vec3 vec3)
{
	GLCall_msg(glUniform3f(getUniformLocation(name), vec3.x, vec3.y, vec3.z), "Uniform name: "<<name);
}
void Shader::setUniform4f(std::string const& name, float v0, float v1, float v2, float v3)
{
	GLCall_msg(glUniform4f(getUniformLocation(name), v0, v1, v2, v3), "Uniform name: "<<name);
}
void Shader::setUniform4f(std::string const& name, glm::vec4 vec3)
{
	GLCall_msg(glUniform4f(getUniformLocation(name), vec3.x, vec3.y, vec3.z, vec3.w), "Uniform name: "<<name);
}

void Shader::setUniformMat4f(std::string const& name, glm::mat4 const& matrix)
{
	GLCall_msg(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]), "Uniform name: "<<name);
}

int Shader::getUniformLocation(std::string const& name)
{
	if(mUniformCache.find(name) != mUniformCache.end()) // Check if the uniform's location is already cached 
		return mUniformCache[name];

	// The location wasn't cached
	GLCall(int location = glGetUniformLocation(mRendererID, name.c_str()));
	if(location == -1) // It does not exist, but -1 is still valid
		LOG_WARNING("Uniform \"" + name + "\" does not exist.");

	// Cache the location
	mUniformCache[name] = location;

	// Return the location
	return location;
}
