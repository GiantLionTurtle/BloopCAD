
#include "preferences.hpp"

preferences::preferences()
{

}

preferences& preferences::get_instance()
{
	static preferences prefs;
	return prefs;
}

template<> 
void preferences::set(std::string const& param, long const& val)
{
	mParams_long[param] = val;
}
template<> 
void preferences::set(std::string const& param, float const& val)
{
	mParams_float[param] = val;
}
template<> 
void preferences::set(std::string const& param, glm::vec3 const& val)
{
	mParams_vec3[param] = val;
}

long preferences::get_long(std::string const& param)
{
	return find_in_map(param, mParams_long);
}
float preferences::get_float(std::string const& param)
{
	return find_in_map(param, mParams_float);
}
glm::vec3 preferences::get_vec3(std::string const& param)
{
	return find_in_map(param, mParams_vec3);
}