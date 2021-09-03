
#include "TexturesPool.hpp"

#include <Utils/Expunge.hpp>

TexturesPool& TexturesPool::get_instance()
{
	static TexturesPool pool;
	return pool;
}

void TexturesPool::add(std::string const& name, Texture* texture)
{
	mTextures[name] = texture;
}
Texture* TexturesPool::get(std::string const& name) 
{
	if(mTextures.find(name) != mTextures.end()) {
		return mTextures.at(name);
	}
	return nullptr;
}

TexturesPool::TexturesPool()
{

}
TexturesPool::~TexturesPool()
{
	for(auto it = mTextures.begin(); it != mTextures.end(); ++it) {
		expunge(it->second);
	}
}