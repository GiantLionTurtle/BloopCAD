
#include "preferences.hpp"
#include <utils/fileUtils.hpp>
#include <utils/xmlParser.hpp>
#include <utils/conversionsUtils.hpp>

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

void preferences::load_from_file(std::string const& filePath)
{
	std::string fileContent = readFromFile(filePath);

	char* c_str = new char[fileContent.size() + 1];
	std::copy(fileContent.begin(), fileContent.end(), &c_str[0]);
	c_str[fileContent.size()] = '\0';

	XML_document doc;
	doc.parse(c_str);

	XML_node* configs = doc.get_child_by_name("configs");
	if(configs) {
		XML_node* config = configs->firstChild();
		while(config) {
			XML_element* elem = dynamic_cast<XML_element*>(config);
			if(elem) {
				XML_attribute* type = elem->get_attribute_by_name("type");
				if(type) {
					if(type->value() == "float") {
						set(elem->name(), string_to_float(elem->content()));
					} else if(type->value() == "long") {
						set(elem->name(), string_to_long(elem->content()));
					} else if(type->value() == "vec3") {
						set(elem->name(), string_to_vec3(elem->content()));
					} else {
						LOG_WARNING("Unknown type: " + type->value());
					}
				}
			} 
			config = config->next();
		}
	} else {
		LOG_WARNING("Could not locate configs node.");
	}
}
void preferences::clear()
{
	mParams_float.clear();
	mParams_long.clear();
	mParams_vec3.clear();
}