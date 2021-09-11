
#include "File_util.hpp"
#include <Utils/Debug_util.hpp>

#include <fstream>
#include <sstream>

std::string readFromFile(std::string const& filePath)
{
	std::ifstream stream(filePath);
	if(!stream) {
		LOG_WARNING("Could not open file \"" + filePath + "\"");
		return "";
	}

	std::string str(std::istreambuf_iterator<char>(stream), {}); // Some stl wizardry
	return str;
}