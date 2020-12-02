
#include "fileUtils.hpp"
#include <utils/errorLogger.hpp>

#include <fstream>
#include <sstream>

std::string readFromFile(std::string const& filePath)
{
	std::ifstream stream(filePath);
	if(!stream) {
		LOG_ERROR("Could not open file \"" + filePath + "\"");
		return "";
	}

	std::string str(std::istreambuf_iterator<char>(stream), {});
	return str;
}