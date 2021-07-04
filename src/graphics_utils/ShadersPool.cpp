
#include "ShadersPool.hpp"

ShadersPool& ShadersPool::get_instance()
{
	static ShadersPool pool;
	return pool;
}

void ShadersPool::add(std::string const& name, std::shared_ptr<Shader> Shader_)
{
	mShaders[name] = Shader_;
}
std::shared_ptr<Shader> ShadersPool::get(std::string const& name) 
{
	if(mShaders.find(name) != mShaders.end()) {
		return mShaders.at(name);
	}
	return nullptr;
}

ShadersPool::ShadersPool()
{

}