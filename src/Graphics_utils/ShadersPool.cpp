
#include "ShadersPool.hpp"

ShadersPool& ShadersPool::get_instance()
{
	static ShadersPool pool;
	return pool;
}

void ShadersPool::add(std::string const& name, Shader* Shader_)
{
	mShaders[name] = Shader_;
}
Shader* ShadersPool::get(std::string const& name) 
{
	if(mShaders.find(name) != mShaders.end()) {
		return mShaders.at(name);
	}
	return nullptr;
}

ShadersPool::ShadersPool()
{

}
ShadersPool::~ShadersPool()
{
	for(auto it = mShaders.begin(); it != mShaders.end(); ++it) {
		delete it->second;
	}
}