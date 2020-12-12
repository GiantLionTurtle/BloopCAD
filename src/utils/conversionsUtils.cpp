
#include "conversionsUtils.hpp"
#include <utils/errorLogger.hpp>

#include <exception>
#include <sstream>
#include <algorithm>

float string_to_float(std::string const& str)
{
	// Basically a wrapper for std::atof
	try {
		float value;
		value = std::atof(str.c_str());
		return value;
	} catch (const std::exception& e) {
		(void)e;
		LOG_WARNING(e.what());
		return 0.0f;
	}
}
long string_to_long(std::string const& str)
{
	// Basically a wrapper for std::atoi
	try {
		long value;
		value = std::atoi(str.c_str());
		return value;
	} catch (const std::exception& e) {
		(void)e;
		LOG_WARNING(e.what());
		return 0;
	}
}
glm::vec3 string_to_vec3(std::string const& str)
{
	// Surely the most efficient way to do it but it is clean, and not critical

	glm::vec3 out;
	std::istringstream ss(str);
    int count = 0;
    while (ss  || count > 2) {
		std::string s;
    	if (!getline( ss, s, ';' )) // This breaks the string at the ';' character, not the newline
			break;
		
		// Remove all white characters
		s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
		s.erase(std::remove(s.begin(), s.end(), '\t'), s.end());
		s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

		// Cast segment to float in appropriate pos
		*(&out.x+count) = string_to_float(s);
		count++;
    }
	return out;
}