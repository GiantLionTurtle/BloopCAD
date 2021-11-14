
// BloopCAD
// Copyright (C) 2020  BloopCorp

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

#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

/*
	@struct ShaderSource is a helper struct to tupple a source string and a Shader type
*/
struct ShaderSource {
	/*
		@function ShaderSource creates the ShaderSource object

		@param source_ 	The data that the object contains
		@param type_ 	The openGL Shader type
	*/
	ShaderSource(std::string const& source_, unsigned int type_)
		: source(source_)
		, type(type_)
	{}
	std::string source; // The source code or path to source code of the Shader
	unsigned int type; // The type of Shader (vertex, geometry, tesseleration, fragment, etc..)
};

/*
	@class Shader describes an openGL Shader
	@warning Make sure that all the function are called whithin the same openGL context
*/
class Shader {
private:
	unsigned int mRendererID; // The openGL handle to the Shader
	std::unordered_map<std::string, int> mUniformCache; // A cache to remember the uniform locations and hopefully save time when setting uniforms

	int mLastUsedAtFrame;
public:
	/*
		@function Shader creates a Shader from raw source with only vertex and fragment Shader in the pipeline

		@param vertexShaderSource 	The source of the vertex Shader
		@param fragmentShaderSource 	The source of the fragment Shader
	*/
	Shader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource);
	/*
		@function Shader creates a Shader from any (valid) amount of identified sources

		@param ShaderSources The sources of all Shader tuppled with their type
	*/
	Shader(std::vector<ShaderSource> const& ShaderSouces);
	/*
		@function Shader creates a Shader from an already existing openGL Shader id

		@param id The openGL handle to the already created Shader
	*/
	Shader(unsigned int id);
	/*
		@function Shader creates an empty Shader 
	*/
	Shader();
	/*
		@function ~Shader destroy the Shader object and deallocate the openGL handle
	*/
	~Shader();

	/*
		@function fromFiles creates a Shader from raw source with only vertex and fragment Shader in the pipeline

		@param vertexShaderSource 	The source of the vertex Shader
		@param fragmentShaderSource The source of the fragment Shader

		@return The created Shader
	*/
	static Shader fromFiles(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath);
	/*
		@function fromFiles creates a Shader from any (valid) amount of identified sources

		@param ShaderSources The sources of all Shader tuppled with their type

		@return The created Shader
	*/
	static Shader fromFiles(std::vector<ShaderSource> ShaderPaths);
	/*
		@function fromFile creates a Shader from one file containing paths to other files
		it currently only support the vertex-fragment pipeline, because it has never been used

		@param resourceFilePath The path to the file containing the paths to the sources

		@return The created Shader
	*/
	static Shader fromFile(std::string const& resourceFilePath);

	/*
		@function fromFiles_ptr creates a Shader from raw source with only vertex and fragment Shader in the pipeline

		@param vertexShaderSource 	The source of the vertex Shader
		@param fragmentShaderSource The source of the fragment Shader

		@return A shared pointer to the created Shader
	*/
	static Shader* fromFiles_ptr(std::string const& vertexShaderFilePath, std::string const& fragmentShaderFilePath);
	/*
		@function fromFiles creates a Shader from any (valid) amount of identified sources

		@param ShaderSources The sources of all Shader tuppled with their type

		@return A shared pointer to the created Shader
	*/
	static Shader* fromFiles_ptr(std::vector<ShaderSource> ShaderPaths);
	/*
		@function fromFile_ptr creates a Shader from one file containing paths to other files
		it currently only support the vertex-fragment pipeline, because it has never been used

		@param resourceFilePath The path to the file containing the paths to the sources

		@return A shared pointer to the created Shader
	*/
	static Shader* fromFile_ptr(std::string const& resourceFilePath);

	/*
		@function bind activates the Shader in openGL, so that it can be used
	*/
	void bind() const;
	/*
		@function unbind desactivates the Shader in openGL (or does nothing if in release mode)
	*/
	void unbind() const;

	/*
		@function id

		@return The openGL handle to the Shader
	*/
	unsigned int id() const { return mRendererID; };
	/*
		@function valid 

		@return Whether or not the openGL handle is non-zero
	*/
	bool valid() const { return id() != 0; };
	/*
		@operator bool 

		@return Whether or not the Shader is valid
	*/
	operator bool() const { return valid(); };

	/*
		@function compileShader compiles a segment of the Shader program (vertex, fragment, geometry, etc...)

		@param type 	The type of Shader it is compiling
		@param source 	The source code of the Shader to compile

		@return The openGL handle to that part of the Shader program, if it succeeds 
	*/
	static unsigned int compileShader(unsigned int type, std::string const& source);
	/*
		@function createShader compiles and link a Shader program with a simple vertex-fragment pipeline

		@param vertexShaderSource 	The source code of the vertex Shader
		@param fragmentShaderSource The source code of the fragment Shader

		@return The created program, if it succeeds
	*/
	static unsigned int createShader(std::string const& vertexShaderSource, std::string const& fragmentShaderSource);
	/*
		@function createShader compiles and link a Shader program of any (valid) pipeline

		@param ShaderSources The sources of the different Shaders with their tupled type

		@return The created Shader, if it succeeds
	*/
	static unsigned int createShader(std::vector<ShaderSource> const& ShaderSources);

	/*
		@function setUniform1i sets a single int uniform variable at a location

		@param name 	The name of the uniform
		@param v0 	The value to be set
	*/
	void setUniform1i(std::string const& name, int v0);
	/*
		@function setUniform1f sets a single float uniform variable at a location

		@param name The name of the uniform
		@param v0 	The value to be set
	*/
	void setUniform1f(std::string const& name, float v0);
	/*
		@function setUniform2f sets a vec2 float uniform variable at a location

		@param name The name of the uniform
		@param v0 	The first value of the vec2 to be set
		@param v1 	The second value of the vec2 to be set
	*/
	void setUniform2f(std::string const& name, float v0, float v1);
	/*
		@function setUniform2f sets a vec2 float uniform variable at a location

		@param name 	The name of the uniform
		@param vec2 	The vec2 to be set
	*/
	void setUniform2f(std::string const& name, glm::vec2 vec2);
	/*
		@function setUniform3f sets a vec3 float uniform variable at a location

		@param name The name of the uniform
		@param v0 	The first value of the vec3 to be set
		@param v1 	The second value of the vec3 to be set
		@param v2 	The third value of the vec3 to be set
	*/
	void setUniform3f(std::string const& name, float v0, float v1, float v2);
	/*
		@function setUniform3f sets a vec3 float uniform variable at a location

		@param name 	The name of the uniform
		@param vec3 	The vec3 to be set
	*/
	void setUniform3f(std::string const& name, glm::vec3 vec3);
	/*
		@function setUniform4f sets a vec4 float uniform variable at a location

		@param name The name of the uniform
		@param v0 	The first value of the vec4 to be set
		@param v1 	The second value of the vec4 to be set
		@param v2 	The third value of the vec4 to be set
		@param v3 	The fourth value of the vec4 to be set
	*/
	void setUniform4f(std::string const& name, float v0, float v1, float v2, float v3);
	/*
		@function setUniform4f sets a vec4 float uniform variable at a location

		@param name The name of the uniform
		@param vec4 The vec4 to be set
	*/
	void setUniform4f(std::string const& name, glm::vec4 vec3);

	/*
		@function setUniformMat4f sets a Mat4 float uniform variable at a location

		@param name 	The name of the uniform
		@param matrix 	The matrix to be set (as a glm::mat4)
	*/
	void setUniformMat4f(std::string const& name, glm::mat4 const& matrix);

	int lastUsed() const { return mLastUsedAtFrame; }
	void set_used(int frame) { mLastUsedAtFrame = frame; };
private:
	/*
		@function getUniformLocation retrieves the location of a uniform variable within a Shader program

		@param name The name of the of the uniform 

		@return The location of the uniform or -1 if it doesn't exist
	*/
	int getUniformLocation(std::string const& name);
};

#endif
