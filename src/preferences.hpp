
#ifndef PREFERENCES_HPP_
#define PREFERENCES_HPP_

#include <utils/errorLogger.hpp>

#include <glm/glm.hpp>

#include <string>
#include <map>

class preferences {
private:
	std::map<std::string, long> mParams_long;
	std::map<std::string, float> mParams_float;
	std::map<std::string, glm::vec3> mParams_vec3;
public:
	preferences(const preferences&) = delete;

	static preferences& get_instance();

	template<typename eT>
	void set(std::string const& param, eT const& val)
	{
		LOG_WARNING("Could not set parameter \"" + param + "\" as this type is not handled.");
	}

	long get_long(std::string const& param);
	float get_float(std::string const& param);
	glm::vec3 get_vec3(std::string const& param);

private:
	preferences();

	template<typename eT>
	eT find_in_map(std::string const& param, std::map<std::string, eT>& map)
	{
		if(map.find(param) != map.end()) {
			return map.at(param);
		}
		LOG_WARNING("Searched for parameter \"" + param + "\", there is no such parameter.");
		return eT(0);
	}
};

template<> void preferences::set(std::string const& param, long const& val);
template<> void preferences::set(std::string const& param, float const& val);
template<> void preferences::set(std::string const& param, glm::vec3 const& val);

#endif