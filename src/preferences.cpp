
#include "preferences.hpp"

preferences::preferences()
{

}

preferences& preferences::get_instance()
{
	static preferences prefs;
	return prefs;
}

void preferences::add_callback(std::string const& param, std::function<void(long)> func)
{
	add_callback_to_map(param, mParams_long, func);
}
void preferences::add_callback(std::string const& param, std::function<void(float)> func)
{
	add_callback_to_map(param, mParams_float, func);
}
void preferences::add_callback(std::string const& param, std::function<void(glm::vec3)> func)
{
	add_callback_to_map(param, mParams_vec3, func);
}

void preferences::set(std::string const& param, long val)
{
	set_in_map(param, mParams_long, val);
}
void preferences::set(std::string const& param, float val)
{
	set_in_map(param, mParams_float, val);
}
void preferences::set(std::string const& param, glm::vec3 val)
{
	set_in_map(param, mParams_vec3, val);
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