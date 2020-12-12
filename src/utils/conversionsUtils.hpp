
#ifndef CONVERSIONSUTILS_HPP_
#define CONVERSIONSUTILS_HPP_

#include <glm/glm.hpp>

#include <string>

/*
    @function string_to_float attempts to create a float from a string

    @return : The created float or 0.0f
*/
float string_to_float(std::string const& str);
/*
    @function string_to_long attempts to create a long from a string

    @return : The created long or 0
*/
long string_to_long(std::string const& str);
/*
    @function string_to_vec3 attempts to create a vec3 from a string

    @return : The created vec3 or (0.0f, 0.0f, 0.0f)
*/
glm::vec3 string_to_vec3(std::string const& str);

#endif