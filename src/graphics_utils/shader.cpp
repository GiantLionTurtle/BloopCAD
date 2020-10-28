
#include "shader.hpp"

#include <fstream>
#include <filesystem>

#include "GLCall.hpp"

shader::shader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource)
{
	mRendererID = createShader(vertexShaderSource, fragmentShaderSource);
}

shader::shader(unsigned int id) :
	mRendererID(id)
{}

shader::shader() :
	mRendererID(0)
{}

shader::~shader()
{
	if(valid()) {
		GLCall(glDeleteShader(mRendererID));
	}
}

shader shader::fromFiles(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath)
{
	std::string vertexShaderSource    = readFromFile(vertexShaderFilePath);
	std::string fragmentShaderSource  = readFromFile(fragmentShaderFilePath);

	return shader(createShader(vertexShaderSource, fragmentShaderSource));
}
shader shader::fromFiles(std::vector<shader_source> shaderPaths)
{
	if(shaderPaths.size() < 2 || shaderPaths.size() > 5) {
		LOG_WARNING("Invalid number of paths given, empty shader returned");
		return shader();
	}
	for(int i = 0; i < shaderPaths.size(); ++i) {
		shaderPaths[i].source = readFromFile(shaderPaths[i].source);
	}

	return shader(createShader(shaderPaths));
}
shader shader::fromFile(std::string const& resourceFilePath)
{
	std::ifstream stream(resourceFilePath);
	if(!stream)
		return shader();

	int i = 0;
	std::string filePaths[3];
	std::string line;
	while(stream>>line && i < 3) {
		filePaths[i] = line;
	}

	std::string resourceFolderPath = "";
	size_t endOfFolder_loc = resourceFilePath.find_last_of("/\\");
	if (endOfFolder_loc != std::string::npos)
		resourceFolderPath = resourceFilePath.substr(0, endOfFolder_loc);

	if(i == 1) {
		return fromFiles(
			std::filesystem::path(resourceFolderPath).append(filePaths[0]).string(), 
			std::filesystem::path(resourceFolderPath).append(filePaths[1]).string());
	} else {
		LOG_WARNING("more than two shaders stages from single file not supported, empty shader returned.");
	}
	return shader();
}

std::shared_ptr<shader> shader::fromFiles_ptr(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath)
{
	std::string vertexShaderSource    = readFromFile(vertexShaderFilePath);
	std::string fragmentShaderSource  = readFromFile(fragmentShaderFilePath);

	return std::shared_ptr<shader>(new shader(createShader(vertexShaderSource, fragmentShaderSource)));
}
std::shared_ptr<shader> shader::fromFiles_ptr(std::vector<shader_source> shaderPaths)
{
	if(shaderPaths.size() < 2 || shaderPaths.size() > 5) {
		LOG_WARNING("Invalid number of paths in shader::fromFiles, empty shader returned");
		return std::shared_ptr<shader>(nullptr);
	}
	for(int i = 0; i < shaderPaths.size(); ++i) {
		shaderPaths[i].source = readFromFile(shaderPaths[i].source);
	}

	return std::shared_ptr<shader>(new shader(createShader(shaderPaths)));
}
std::shared_ptr<shader> shader::fromFile_ptr(std::string const& resourceFilePath)
{
	std::ifstream stream(resourceFilePath);
	if(!stream)
		return std::shared_ptr<shader>(nullptr);

	int i = 0;
	std::string filePaths[3];
	std::string line;
	while(stream>>line && i < 3) {
		filePaths[i] = line;
	}

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
	return std::shared_ptr<shader>(nullptr);
}

unsigned int shader::ID() const
{
	// Probably won't be exposed in the future..
	return mRendererID;
}

bool shader::valid() const
{
	return mRendererID != 0;
}
shader::operator bool() const
{
	return valid();
}

void shader::bind() const
{
	GLCall(glUseProgram(mRendererID));
}
void shader::unbind() const
{
#ifndef RELEASE_MODE
	GLCall(glUseProgram(0));
#endif
}

unsigned int shader::compileShader(unsigned int type, std::string const& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

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

	return id;
}
unsigned int shader::createShader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vertexShader   = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	if(!vertexShader || !fragmentShader) {
		return 0;
	}

	GLCall(glAttachShader(program, vertexShader));
	GLCall(glAttachShader(program, fragmentShader));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

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

	GLCall(glDetachShader(program, vertexShader));
	GLCall(glDetachShader(program, fragmentShader));

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));

	return program;
}
unsigned int shader::createShader(std::vector<shader_source> const& shaderSources)
{
	if(shaderSources.size() == 0 || shaderSources.size() > 5) {
		LOG_WARNING("Invalid number of paths in shader::fromFiles, empty shader returned");
		return 0;
	}
	unsigned int shaders[5];
	GLCall(unsigned int program = glCreateProgram());
	for(int i = 0; i < shaderSources.size(); ++i) {
		shaders[i] = compileShader(shaderSources[i].type, shaderSources[i].source);
		if(!shaders[i]) {
			LOG_WARNING("could not compile shader stage " + std::to_string(i) + ", empty shader returned");
		}
	}
	
	// TODO: check if for loops can be combined
	for(int i = 0; i < shaderSources.size(); ++i) {
		GLCall(glAttachShader(program, shaders[i]));
	}
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	int result;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if(result == GL_FALSE)
	{
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		std::string message;
		message.reserve(length);
		GLCall(glGetProgramInfoLog(program, length, &length, (GLchar *)message.c_str()));
		LOG_ERROR("Failed to link shader program: \"" + message + "\"");

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

	// TODO: check if for loops can be combined
	for(int i = 0; i < shaderSources.size(); ++i) {
		GLCall(glDetachShader(program, shaders[i]));
	}
	for(int i = 0; i < shaderSources.size(); ++i) {
		GLCall(glDeleteShader(shaders[i]));
	}

	return program;
}


std::string shader::readFromFile(std::string const& filePath)
{
	std::ifstream stream(filePath);
	if(!stream) {
		LOG_ERROR("Could not open file \"" + filePath + "\"");
		return "";
	}

	std::string str(std::istreambuf_iterator<char>(stream), {});
	return str;
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
void shader::setUniform3f(std::string const& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}
void shader::setUniform4f(std::string const& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void shader::setUniformMat4f(std::string const& name, glm::mat4 const& matrix)
{
	GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int shader::getUniformLocation(std::string const& name)
{
	if(mUniformCache.find(name) != mUniformCache.end())
		return mUniformCache[name];

	GLCall(int location = glGetUniformLocation(mRendererID, name.c_str()));
	if(location == -1)
		LOG_WARNING("Uniform \"" + name + "\" does not exist.");

	mUniformCache[name] = location;

	return location;
}
