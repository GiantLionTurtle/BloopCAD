
#include "shadersPool.hpp"

shadersPool& shadersPool::get_instance()
{
	static shadersPool pool;
	return pool;
}

void shadersPool::add(std::string const& name, std::shared_ptr<shader> shader_)
{
	mShaders[name] = shader_;
}
std::shared_ptr<shader> shadersPool::get(std::string const& name) 
{
	if(mShaders.find(name) != mShaders.end()) {
		return mShaders.at(name);
	}
	return nullptr;
}

shadersPool::shadersPool()
{

}