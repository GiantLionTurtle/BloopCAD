
#ifndef CONVERSIONSUTILS_HPP_
#define CONVERSIONSUTILS_HPP_

#include <glm/glm.hpp>

#include <string>

float string_to_float(std::string const& str);
long string_to_long(std::string const& str);
glm::vec3 string_to_vec3(std::string const& str);

#endif