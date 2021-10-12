
#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

/*
	@struct shaderSource is a helper struct to tupple a source string and a shader type
*/
struct shaderSource {
	/*
		@function shaderSource creates the shaderSource object

		@param source_ : 	The data that the object contains
		@param type_ : 		The openGL shader type
	*/
	shaderSource(std::string const& source_, unsigned int type_):
		source(source_),
		type(type_)
	{}
	std::string source; // The source code or path to source code of the shader
	unsigned int type; // The type of shader (vertex, geometry, tesseleration, fragment, etc..)
};

/*
	@class shader describes an openGL shader
	@warning : Make sure that all the function are called whithin the same openGL context
*/
class shader {
private:
	unsigned int mRendererID; // The openGL handle to the shader
	std::unordered_map<std::string, int> mUniformCache; // A cache to remember the uniform locations and hopefully save time when setting uniforms

	int mLastUsedAtFrame;
public:
	/*
		@function shader creates a shader from raw source with only vertex and fragment shader in the pipeline

		@param vertexShaderSource : 	The source of the vertex shader
		@param fragmentShaderSource : 	The source of the fragment shader
	*/
	shader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource);
	/*
		@function shader creates a shader from any (valid) amount of identified sources

		@param shaderSources : The sources of all shader tuppled with their type
	*/
	shader(std::vector<shaderSource> const& shaderSouces);
	/*
		@function shader creates a shader from an already existing openGL shader id

		@param id : The openGL handle to the already created shader
	*/
	shader(unsigned int id);
	/*
		@function shader creates an empty shader 
	*/
	shader();
	/*
		@function ~shader destroy the shader object and deallocate the openGL handle
	*/
	~shader();

	/*
		@function fromFiles creates a shader from raw source with only vertex and fragment shader in the pipeline

		@param vertexShaderSource : 	The source of the vertex shader
		@param fragmentShaderSource : 	The source of the fragment shader

		@return : The created shader
	*/
	static shader fromFiles(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath);
	/*
		@function fromFiles creates a shader from any (valid) amount of identified sources

		@param shaderSources : The sources of all shader tuppled with their type

		@return : The created shader
	*/
	static shader fromFiles(std::vector<shaderSource> shaderPaths);
	/*
		@function fromFile creates a shader from one file containing paths to other files
		it currently only support the vertex-fragment pipeline, because it has never been used

		@param resourceFilePath : The path to the file containing the paths to the sources

		@return : The created shader
	*/
	static shader fromFile(std::string const& resourceFilePath);

	/*
		@function fromFiles_ptr creates a shader from raw source with only vertex and fragment shader in the pipeline

		@param vertexShaderSource : 	The source of the vertex shader
		@param fragmentShaderSource : 	The source of the fragment shader

		@return : A shared pointer to the created shader
	*/
	static std::shared_ptr<shader> fromFiles_ptr(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath);
	/*
		@function fromFiles creates a shader from any (valid) amount of identified sources

		@param shaderSources : The sources of all shader tuppled with their type

		@return : A shared pointer to the created shader
	*/
	static std::shared_ptr<shader> fromFiles_ptr(std::vector<shaderSource> shaderPaths);
	/*
		@function fromFile_ptr creates a shader from one file containing paths to other files
		it currently only support the vertex-fragment pipeline, because it has never been used

		@param resourceFilePath : The path to the file containing the paths to the sources

		@return : A shared pointer to the created shader
	*/
	static std::shared_ptr<shader> fromFile_ptr(std::string const& resourceFilePath);

	/*
		@function bind activates the shader in openGL, so that it can be used
	*/
	void bind() const;
	/*
		@function unbind desactivates the shader in openGL (or does nothing if in release mode)
	*/
	void unbind() const;

	/*
		@function id

		@return : The openGL handle to the shader
	*/
	unsigned int id() const { return mRendererID; };
	/*
		@function valid 

		@return : Whether or not the openGL handle is non-zero
	*/
	bool valid() const { return id() != 0; };
	/*
		@operator bool 

		@return : Whether or not the shader is valid
	*/
	operator bool() const { return valid(); };

	/*
		@function compileShader compiles a segment of the shader program (vertex, fragment, geometry, etc...)

		@param type : The type of shader it is compiling
		@param source : The source code of the shader to compile

		@return : The openGL handle to that part of the shader program, if it succeeds 
	*/
	static unsigned int compileShader(unsigned int type, std::string const& source);
	/*
		@function createShader compiles and link a shader program with a simple vertex-fragment pipeline

		@param vertexShaderSource : 	The source code of the vertex shader
		@param fragmentShaderSource : 	The source code of the fragment shader

		@return : The created program, if it succeeds
	*/
	static unsigned int createShader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource);
	/*
		@function createShader compiles and link a shader program of any (valid) pipeline

		@param shaderSources : The sources of the different shaders with their tupled type

		@return : The created shader, if it succeeds
	*/
	static unsigned int createShader(std::vector<shaderSource> const& shaderSources);

	/*
		@function setUniform1i sets a single int uniform variable at a location

		@param name : 	The name of the uniform
		@param v0 : 	The value to be set
	*/
	void setUniform1i(std::string const& name, int v0);
	/*
		@function setUniform1f sets a single float uniform variable at a location

		@param name : 	The name of the uniform
		@param v0 : 	The value to be set
	*/
	void setUniform1f(std::string const& name, float v0);
	/*
		@function setUniform2f sets a vec2 float uniform variable at a location

		@param name : 	The name of the uniform
		@param v0 : 	The first value of the vec2 to be set
		@param v1 : 	The second value of the vec2 to be set
	*/
	void setUniform2f(std::string const& name, float v0, float v1);
	/*
		@function setUniform2f sets a vec2 float uniform variable at a location

		@param name : 	The name of the uniform
		@param vec2 : 	The vec2 to be set
	*/
	void setUniform2f(std::string const& name, glm::vec2 vec2);
	/*
		@function setUniform3f sets a vec3 float uniform variable at a location

		@param name : 	The name of the uniform
		@param v0 : 	The first value of the vec3 to be set
		@param v1 : 	The second value of the vec3 to be set
		@param v2 : 	The third value of the vec3 to be set
	*/
	void setUniform3f(std::string const& name, float v0, float v1, float v2);
	/*
		@function setUniform3f sets a vec3 float uniform variable at a location

		@param name : 	The name of the uniform
		@param vec3 : 	The vec3 to be set
	*/
	void setUniform3f(std::string const& name, glm::vec3 vec3);
	/*
		@function setUniform4f sets a vec4 float uniform variable at a location

		@param name : 	The name of the uniform
		@param v0 : 	The first value of the vec4 to be set
		@param v1 : 	The second value of the vec4 to be set
		@param v2 : 	The third value of the vec4 to be set
		@param v3 : 	The fourth value of the vec4 to be set
	*/
	void setUniform4f(std::string const& name, float v0, float v1, float v2, float v3);
	/*
		@function setUniform4f sets a vec4 float uniform variable at a location

		@param name : 	The name of the uniform
		@param vec4 : 	The vec4 to be set
	*/
	void setUniform4f(std::string const& name, glm::vec4 vec3);

	/*
		@function setUniformMat4f sets a Mat4 float uniform variable at a location

		@param name : 		The name of the uniform
		@param matrix : 	The matrix to be set (as a glm::mat4)
	*/
	void setUniformMat4f(std::string const& name, glm::mat4 const& matrix);

	int lastUsed() const { return mLastUsedAtFrame; }
	void set_used(int frame) { mLastUsedAtFrame = frame; };
private:
	/*
		@function getUniformLocation retrieves the location of a uniform variable within a shader program

		@param name : The name of the of the uniform 

		@return : The location of the uniform or -1 if it doesn't exist
	*/
	int getUniformLocation(std::string const& name);
};

#endif
