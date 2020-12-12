
#ifndef FILEUTILS_HPP_
#define FILEUTILS_HPP_

#include <string>

/*
    @function readFromFile reads a whole file in a string

    @param filePath : The path to the file to read

    @return : The content of the file
*/
std::string readFromFile(std::string const& filePath);

#endif