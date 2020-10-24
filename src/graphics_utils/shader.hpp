
#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

struct shader_source {
	shader_source(std::string source_, unsigned int type_):
		source(source_),
		type(type_)
	{}
	std::string source;
	unsigned int type;
};

class shader {
private:
	unsigned int mRendererID;
	std::unordered_map<std::string, int> mUniformCache;

public:

	shader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource);
	shader(std::vector<shader_source> const& shaderSouces);
	shader(unsigned int id);
	shader();

	~shader();

	static shader fromFiles(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath);
	static shader fromFiles(std::vector<shader_source> shaderPaths);
	static shader fromFile(std::string const& resourceFilePath);

	static std::shared_ptr<shader> fromFiles_ptr(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath);
	static std::shared_ptr<shader> fromFiles_ptr(std::vector<shader_source> shaderPaths);
	static std::shared_ptr<shader> fromFile_ptr(std::string const& resourceFilePath);

	unsigned int ID() const;

	bool valid() const;
	operator bool() const;

	void bind() const;
	void unbind() const;

	static unsigned int compileShader(unsigned int type, std::string const& source);
	static unsigned int createShader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource);
	static unsigned int createShader(std::vector<shader_source> const& shaderSources);

	static std::string readFromFile(std::string const& filePath);

	void setUniform1i(std::string const& name, int v0);
	void setUniform1f(std::string const& name, float v0);
	void setUniform2f(std::string const& name, float v0, float v1);
	void setUniform3f(std::string const& name, float v0, float v1, float v2);
	void setUniform4f(std::string const& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(std::string const& name, glm::mat4 const& matrix);

private:

	int getUniformLocation(std::string const& name);
};

#endif
