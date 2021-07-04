
#include "TexturesPool.hpp"

TexturesPool& TexturesPool::get_instance()
{
	static TexturesPool pool;
	return pool;
}

void TexturesPool::add(std::string const& name, std::shared_ptr<Texture> texture)
{
	mTextures[name] = texture;
}
std::shared_ptr<Texture> TexturesPool::get(std::string const& name) 
{
	if(mTextures.find(name) != mTextures.end()) {
		return mTextures.at(name);
	}
	return nullptr;
}

TexturesPool::TexturesPool()
{

}