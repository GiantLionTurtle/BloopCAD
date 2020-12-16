
#ifndef SHADERSPOOL_HPP_
#define SHADERSPOOL_HPP_

#include "shader.hpp"

#include <string>
#include <memory>
#include <map>

class shadersPool {
private:
	std::map<std::string, std::shared_ptr<shader>> mShaders;
public:
	shadersPool(shadersPool const&) = delete; // As it is a singleton, the copy constructor is deleted

	static shadersPool& get_instance();

	void add(std::string const& name, std::shared_ptr<shader> shader_);
	std::shared_ptr<shader> get(std::string const& name);
private:
	shadersPool();
};

#endif