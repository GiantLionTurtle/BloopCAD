
#ifndef SHADERSPOOL_HPP_
#define SHADERSPOOL_HPP_

#include "Shader.hpp"

#include <string>
#include <memory>
#include <map>

class ShadersPool {
private:
	std::map<std::string, std::shared_ptr<Shader>> mShaders;
public:
	ShadersPool(ShadersPool const&) = delete; // As it is a singleton, the copy constructor is deleted

	static ShadersPool& get_instance();

	void add(std::string const& name, std::shared_ptr<Shader> Shader_);
	std::shared_ptr<Shader> get(std::string const& name);
private:
	ShadersPool();
};

#endif