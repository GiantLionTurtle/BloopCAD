
#ifndef PREFERENCES_HPP_
#define PREFERENCES_HPP_

#include <utils/errorLogger.hpp>

#include <glm/glm.hpp>

#include <string>
#include <map>
#include <vector>
#include <functional>

template<typename eT>
using parameter_list = std::map<std::string, std::pair<eT, std::vector<std::function<void(eT)>>>>;

class preferences {
private:
	parameter_list<long> mParams_long;
	parameter_list<float> mParams_float;
	parameter_list<glm::vec3> mParams_vec3;
public:
	preferences(const preferences&) = delete;

	static preferences& get_instance();

	void set(std::string const& param, long val);
	void set(std::string const& param, float val);
	void set(std::string const& param, glm::vec3 val);

	void add_callback(std::string const& param, std::function<void(long)> func);
	void add_callback(std::string const& param, std::function<void(float)> func);
	void add_callback(std::string const& param, std::function<void(glm::vec3)> func);

	long get_long(std::string const& param);
	float get_float(std::string const& param);
	glm::vec3 get_vec3(std::string const& param);

private:
	preferences();

	template<typename eT>
	void add_callback_to_map(std::string const& param, parameter_list<eT>& map, std::function<void(eT)> func)
	{
		if(map.find(param) != map.end()) {
			std::get<1>(map.at(param)).push_back(func);
		} else {
			map[param] = std::make_pair<eT, std::vector<std::function<void(eT)>>>(eT(0), {func});
		}
	}

	template<typename eT>
	void set_in_map(std::string const& param, parameter_list<eT>& map, eT val)
	{
		if(map.find(param) != map.end()) {
			std::vector<std::function<void(eT)>> funcs = std::get<1>(map.at(param));
			for(size_t i = 0; i < funcs.size(); ++i) {
				funcs.at(i)(val);
			}
		} else {
			map[param] = std::make_pair<eT, std::vector<std::function<void(eT)>>>(eT(0), {});
		}
		std::get<0>(map.at(param)) = val;
	}

	template<typename eT>
	eT find_in_map(std::string const& param, parameter_list<eT>& map)
	{
		if(map.find(param) != map.end()) {
			return std::get<0>(map.at(param));
		}
		LOG_WARNING("Searched for parameter \"" + param + "\", there is no such parameter.");
		return eT(0);
	}
};

#endif