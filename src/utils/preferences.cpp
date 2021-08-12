
#include "preferences.hpp"
#include <utils/fileUtils.hpp>
#include <utils/xmlParser.hpp>
#include <utils/conversionsUtils.hpp>

preferences::preferences()
{

}

preferences& preferences::get_instance()
{
	static preferences prefs; // Only one instance will be created
	return prefs;
}

void preferences::add_callback(std::string const& pref, std::function<void(long)> func)
{
	add_callback_to_map(pref, mPrefs_long, func); // Select the long map
}
void preferences::add_callback(std::string const& pref, std::function<void(float)> func)
{
	add_callback_to_map(pref, mPrefs_float, func); // Select the float map
}
void preferences::add_callback(std::string const& pref, std::function<void(glm::vec3)> func)
{
	add_callback_to_map(pref, mPrefs_vec3, func); // Select the vec3 map
}

int preferences::n_callback_float(std::string const& pref)
{
	if(mPrefs_float.find(pref) != mPrefs_float.end()) {
		return mPrefs_float.at(pref).second.size();
	}
	return 0;
}
int preferences::n_callback_long(std::string const& pref)
{
	if(mPrefs_long.find(pref) != mPrefs_long.end()) {
		return mPrefs_long.at(pref).second.size();
	}
	return 0;
}
int preferences::n_callback_vec3(std::string const& pref)
{
	if(mPrefs_vec3.find(pref) != mPrefs_vec3.end()) {
		return mPrefs_vec3.at(pref).second.size();
	}
	return 0;
}
int preferences::n_callback(std::string const& pref)
{
	int out = 0;

	out = n_callback_float(pref);
	if(out)
		return out;
	out = n_callback_long(pref);
	if(out)
		return out;
	out = n_callback_vec3(pref);
	if(out)
		return out;
	return 0;
}

void preferences::set(std::string const& pref, long val)
{
	set_in_map(pref, mPrefs_long, val); // Select the long map
}
void preferences::set(std::string const& pref, float val)
{
	set_in_map(pref, mPrefs_float, val); // Select the float map
}
void preferences::set(std::string const& pref, glm::vec3 val)
{
	set_in_map(pref, mPrefs_vec3, val); // Select the vec3 map
}

long preferences::get_long(std::string const& pref)
{
	return find_in_map(pref, mPrefs_long); // Select the long map
}
float preferences::get_float(std::string const& pref)
{
	return find_in_map(pref, mPrefs_float); // Select the float map
}
glm::vec3 preferences::get_vec3(std::string const& pref)
{
	return find_in_map(pref, mPrefs_vec3); // Select the vec3 map
}

void preferences::load_from_file(std::string const& filePath)
{
	std::string fileContent = readFromFile(filePath);

	char* c_str = new char[fileContent.size() + 1];
	std::copy(fileContent.begin(), fileContent.end(), &c_str[0]);
	c_str[fileContent.size()] = '\0';

	XML_document doc;
	doc.parse(c_str);

	XML_node* configs = doc.get_child_by_name("configs"); // Find the configs node, it's children are the preferences
	if(configs) {
		XML_node* config = configs->firstChild();
		while(config) { // Go through all of configs's children
			XML_element* elem = dynamic_cast<XML_element*>(config);
			if(elem) { // Only the elements are valid preference
				XML_attribute* type = elem->get_attribute_by_name("type");
				if(type) {
					// Index their type and set the corresponding registeries
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
	// Clear everything, should there be a callback for destruction?
	mPrefs_float.clear();
	mPrefs_long.clear();
	mPrefs_vec3.clear();
}