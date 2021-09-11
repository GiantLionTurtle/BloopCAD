
#ifndef TEXTURESPOOL_HPP_
#define TEXTURESPOOL_HPP_

#include "Texture.hpp"

#include <string>
#include <memory>
#include <map>

class TexturesPool {
private:
	std::map<std::string, Texture*> mTextures;
public:
	TexturesPool(TexturesPool const&) = delete; // As it is a singleton, the copy constructor is deleted

	static TexturesPool& get_instance();

	void add(std::string const& name, Texture* texture);
	Texture* get(std::string const& name);
private:
	TexturesPool();
	~TexturesPool();
};

#endif