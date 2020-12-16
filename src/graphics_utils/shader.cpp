
#include "shader.hpp"
#include "GLCall.hpp"
#include <utils/fileUtils.hpp>

#include <fstream>
#include <filesystem>

shader::shader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource):
	mLastUsedAtFrame(0)
{
	mRendererID = createShader(vertexShaderSource, fragmentShaderSource);
}
shader::shader(unsigned int id) :
	mRendererID(id),
	mLastUsedAtFrame(0)
{

}
shader::shader() :
	mRendererID(0),
	mLastUsedAtFrame(0)
{
	
}
shader::~shader()
{
	if(valid()) {
		GLCall(glDeleteShader(mRendererID));
	}
}

shader shader::fromFiles(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath)
{
	// Aquire the source code
	std::string vertexShaderSource    = readFromFile(vertexShaderFilePath);
	std::string fragmentShaderSource  = readFromFile(fragmentShaderFilePath);

	// Actually create the shader
	return shader(createShader(vertexShaderSource, fragmentShaderSource));
}
shader shader::fromFiles(std::vector<shaderSource> shaderPaths)
{
	if(shaderPaths.size() < 2 || shaderPaths.size() > 5) { // Those are invalid pipelines
		LOG_WARNING("Invalid number of paths given, empty shader returned");
		return shader();
	}

	// Kind of dirty, but shaderPaths contains path to source files and it latter contains the actual source code
	for(int i = 0; i < shaderPaths.size(); ++i) {
		// Aquire the source code
		shaderPaths[i].source = readFromFile(shaderPaths[i].source);
	}

	// Create the shader
	return shader(createShader(shaderPaths));
}
shader shader::fromFile(std::string const& resourceFilePath)
{
	std::ifstream stream(resourceFilePath);
	if(!stream) // Check if file exists
		return shader();

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
		LOG_WARNING("more than two shaders stages from single file not supported, empty shader returned.");
	}

	// If file is invalid, return an empty shader
	return shader();
}

std::shared_ptr<shader> shader::fromFiles_ptr(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath)
{
	// Aquire the source code, might be a bit redundant, from the non-ptr version, but I am uncertain of how to reuse the code
	// Maybe create the shader normaly and use it's id to create the pointer while ensuring that the destructor doesn't deallocate the handle?
	std::string vertexShaderSource    = readFromFile(vertexShaderFilePath);
	std::string fragmentShaderSource  = readFromFile(fragmentShaderFilePath);

	// Create the shader
	return std::shared_ptr<shader>(new shader(createShader(vertexShaderSource, fragmentShaderSource)));
}
std::shared_ptr<shader> shader::fromFiles_ptr(std::vector<shaderSource> shaderPaths)
{
	// Prety much the same as the non-ptr version...
	if(shaderPaths.size() < 2 || shaderPaths.size() > 5) {
		LOG_WARNING("Invalid number of paths in shader::fromFiles, empty shader returned");
		return std::shared_ptr<shader>(nullptr);
	}

	// Just as wack with the path => source thing
	for(int i = 0; i < shaderPaths.size(); ++i) {
		shaderPaths[i].source = readFromFile(shaderPaths[i].source);
	}

	// Create the shader
	return std::shared_ptr<shader>(new shader(createShader(shaderPaths)));
}
std::shared_ptr<shader> shader::fromFile_ptr(std::string const& resourceFilePath)
{
	std::ifstream stream(resourceFilePath);
	if(!stream) // Ensure that the file exist, in the ptr version, it has the luxury of returning nullptr
		return std::shared_ptr<shader>(nullptr);

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
		LOG_WARNING("More than two shaders stages from single file not supported, empty shader returned.");
	}

	// Return nullptr if file is invalid	
	return std::shared_ptr<shader>(nullptr);
}

void shader::bind() const
{
	GLCall(glUseProgram(mRendererID)); // The abstraction can make the API more consistent (bind everywhere)
}
void shader::unbind() const
{
#ifndef RELEASE_MODE // No unecesary bind in release mode, but useful in debug
	GLCall(glUseProgram(0));
#endif
}

unsigned int shader::compileShader(unsigned int type, std::string const& source)
{
	// Create, assign source and compile shader 
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
		LOG_WARNING("Failed to compile " +
			(type == GL_VERTEX_SHADER ? std::string("vertex") : (type == GL_FRAGMENT_SHADER ? std::string("fragment") : std::string("geometry")))
			+ " shader: " + message);
		return 0;
	}

	// Return the handle to the compiled shader
	return id;
}
unsigned int shader::createShader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource)
{
	// Compile both expected shaders
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vertexShader   = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	if(!vertexShader || !fragmentShader) { // Check for validity
		return 0;
	}

	// Link shader program
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
		LOG_WARNING("Failed to link shader program: \"" + message + "\"");

		GLCall(glDetachShader(program, vertexShader));
		GLCall(glDetachShader(program, fragmentShader));

		GLCall(glDeleteShader(vertexShader));
		GLCall(glDeleteShader(fragmentShader));

		GLCall(glDeleteProgram(program));

		return 0;
	}

	// Delete individual parts of the shader program to not clog up openGL
	GLCall(glDetachShader(program, vertexShader));
	GLCall(glDetachShader(program, fragmentShader));

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));

	// Return the shader program ID
	return program;
}
unsigned int shader::createShader(std::vector<shaderSource> const& shaderSources)
{
	if(shaderSources.size() < 2 || shaderSources.size() > 5) { // Check if the number of shaders in the pipeline is valid, might be a bit redundant..
		LOG_WARNING("Invalid number of paths in shader::fromFiles, empty shader returned");
		return 0;
	}

	// Compile all shaders (max of 5 in a valid pipeling)
	unsigned int shaders[5];
	for(int i = 0; i < shaderSources.size(); ++i) {
		shaders[i] = compileShader(shaderSources[i].type, shaderSources[i].source);
		if(!shaders[i]) {
			LOG_WARNING("could not compile shader stage " + std::to_string(i) + ", empty shader returned");
		}
	}

	// Create shader program and link shaders
	GLCall(unsigned int program = glCreateProgram());	
	// TODO: check if for loops can be combined
	for(int i = 0; i < shaderSources.size(); ++i) {
		GLCall(glAttachShader(program, shaders[i]));
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
		LOG_WARNING("Failed to link shader program: \"" + message + "\"");

		// Deallocate and delete everything if there was an error
		// TODO: check if for loops can be combined
		for(int i = 0; i < shaderSources.size(); ++i) {
			GLCall(glDetachShader(program, shaders[i]));
		}
		for(int i = 0; i < shaderSources.size(); ++i) {
			GLCall(glDeleteShader(shaders[i]));
		}

		GLCall(glDeleteProgram(program));

		return 0;
	}

	// Delete individual parts of the shader program to not clog up openGL
	// TODO: check if for loops can be combined
	for(int i = 0; i < shaderSources.size(); ++i) {
		GLCall(glDetachShader(program, shaders[i]));
	}
	for(int i = 0; i < shaderSources.size(); ++i) {
		GLCall(glDeleteShader(shaders[i]));
	}

	// Return the created shader program
	return program;
}

void shader::setUniform1i(std::string const& name, int v0)
{
	GLCall(glUniform1i(getUniformLocation(name), v0));
}
void shader::setUniform1f(std::string const& name, float v0)
{
	GLCall(glUniform1f(getUniformLocation(name), v0));
}
void shader::setUniform2f(std::string const& name, float v0, float v1)
{
	GLCall(glUniform2f(getUniformLocation(name), v0, v1));
}
void shader::setUniform2f(std::string const& name, glm::vec2 vec2)
{
	GLCall(glUniform2f(getUniformLocation(name), vec2.x, vec2.y));
}
void shader::setUniform3f(std::string const& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}
void shader::setUniform3f(std::string const& name, glm::vec3 vec3)
{
	GLCall(glUniform3f(getUniformLocation(name), vec3.x, vec3.y, vec3.z));
}
void shader::setUniform4f(std::string const& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}
void shader::setUniform4f(std::string const& name, glm::vec4 vec3)
{
	GLCall(glUniform4f(getUniformLocation(name), vec3.x, vec3.y, vec3.z, vec3.w));
}

void shader::setUniformMat4f(std::string const& name, glm::mat4 const& matrix)
{
	GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int shader::getUniformLocation(std::string const& name)
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
