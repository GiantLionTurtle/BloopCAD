
#include "texture.hpp"

#include <string>

class image : public texture {
private:
	std::string mFilePath;
public:
	image(std::string const& filePath);
	~image();
};