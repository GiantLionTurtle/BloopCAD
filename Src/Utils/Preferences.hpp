
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef PREFERENCES_HPP_
#define PREFERENCES_HPP_

#include <Utils/Debug_util.hpp>

#include <glm/glm.hpp>

#include <string>
#include <map>
#include <vector>
#include <functional>

// parameter_list is a helper definition to represent a string indexed cache list of parameters, which are a value and a list of callbacks
template<typename eT>
using parameter_list = std::map<std::string, std::pair<eT, std::vector<std::function<void(eT)>>>>;

/*
	@class Preferences is the one go for customization, it is accessible everywhere in the program and
	it currently supports 3 types of Preferences ; long, float and vec3

	@note : This class is a singletong
*/
class Preferences {
private:
	parameter_list<long> mPrefs_long; 		// Long Preferences
	parameter_list<float> mPrefs_float; 	// Float Preferences
	parameter_list<glm::vec3> mPrefs_vec3;	// Vec3 Preferences
public:
	Preferences(Preferences const&) = delete; // As it is a singleton, the copy constructor is deleted

	/*
		@function get_instance gives access to the singleton instance

		@return : The singleton instance
	*/
	static Preferences& get_instance();

	/*
		@function set sets a named preference or creates it if it doesn't exist

		@param pref : 	The name of the preference
		@param val	:	The value to assign to the preference
	*/
	void set(std::string const& pref, long val);
	void set(std::string const& pref, float val);
	void set(std::string const& pref, glm::vec3 val);

	/*
		@function add_callbacks adds a callback which will be invoked each time a particular preference is changed

		@param pref : The preference to which to bind the callback
		@param func	: The callback function
	*/
	void add_callback(std::string const& pref, std::function<void(long)> func);
	void add_callback(std::string const& pref, std::function<void(float)> func);
	void add_callback(std::string const& pref, std::function<void(glm::vec3)> func);

	int n_callback_float(std::string const& pref);
	int n_callback_long(std::string const& pref);
	int n_callback_vec3(std::string const& pref);
	int n_callback(std::string const& pref);

	/*
		@function get_long gives a preference value in the long register

		@param pref : The name of the requested preference

		@return : The value of the preference, or 0 if it doesn't exist 
	*/
	long get_long(std::string const& pref);
	/*
		@function get_float gives a preference value in the float register

		@param pref : The name of the requested preference

		@return : The value of the preference, or 0.0f if it doesn't exist 
	*/
	float get_float(std::string const& pref);
	/*
		@function get_vec3 gives a preference value in the vec3 register

		@param pref : The name of the requested preference

		@return : The value of the preference, or (0.0f, 0.0f, 0.0f) if it doesn't exist 
	*/
	glm::vec3 get_vec3(std::string const& pref);

	/*
		@function load_from_file loads a set of Preferences from an xml file

		@param filePath : The path to the xml file containing Preferences

		@note : The Preferences must be children of a node called "configs", have a type attribute
		and have their value as content. Vec3 are represented as values separeted by semicolons.
		Something like this:

		<configs>
			<namedConfig1 type="long">23</namedConfig1>
			<namedConfig2 type="float">0.5</namedConfig2>
			<namedConfig3 type="vec3">0.6 ; 18 ; 20.2</namedConfig3>
		</configs>
	*/
	void load_from_file(std::string const& filePath);

	/*
		@function clear removes all Preferences and callbacks
	*/
	void clear();
private:
	/*
		@function Preferences creates the Preferences object, it is private so that
		only one object can be created and accessed 
	*/
	Preferences();

	/*
		@function add_callback_to_map adds a callback to a preference in a specified registery, 
		if the preference is not found, it is created and initialized at 0

		@param pref : 	The name of the preference to which to add the callback
		@param map : 	The registery containing the preference
		@param func : 	The callback to be added
	*/
	template<typename eT>
	void add_callback_to_map(std::string const& pref, parameter_list<eT>& map, std::function<void(eT)> func)
	{
		if(map.find(pref) != map.end()) {
			std::get<1>(map.at(pref)).push_back(func); // Push the callback if the preference is found
		} else {
			map[pref] = std::make_pair<eT, std::vector<std::function<void(eT)>>>(eT(0), {func}); // Create the preference
		}
	}

	/*
		@function set_in_map sets a preference's value in a specified registery and calls each of it's callbacks, 
		if the searched preference doesn't exist, it is created

		@param pref : 	The name of the preference to set 
		@param map : 	The registery containing the preference
		@param val : 	The value to be assigned to the preference
	*/
	template<typename eT>
	void set_in_map(std::string const& pref, parameter_list<eT>& map, eT val)
	{
		if(map.find(pref) != map.end()) {
			std::vector<std::function<void(eT)>> funcs = std::get<1>(map.at(pref));
			for(size_t i = 0; i < funcs.size(); ++i) { // Call each callback
				funcs.at(i)(val);
			}
		} else {
			map[pref] = std::make_pair<eT, std::vector<std::function<void(eT)>>>(eT(0), {}); // create the preference
		}
		std::get<0>(map.at(pref)) = val; // Set the value
	}

	/*
		@function find_in_map gives the value of a specified preference in a given registery

		@param pref : 	The name of the preference of interest
		@param map : 	The registery in which to search the preference
		
		@return : The value of the searched preference, or 0
	*/
	template<typename eT>
	eT find_in_map(std::string const& pref, parameter_list<eT>& map)
	{
		if(map.find(pref) != map.end()) {
			return std::get<0>(map.at(pref)); // Get the value part of the preference
		}
		LOG_WARNING("Searched for preference \"" + pref + "\", there is no such preference.");
		return eT(0);
	}
};

#endif