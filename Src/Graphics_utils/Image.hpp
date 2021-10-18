
#include "Texture.hpp"

#include <string>

/*
	@class image describes a texture loaded from an image file
*/
class Image : public Texture {
private:
	std::string mFilePath; // The path to the image file
public:
	/*
		@function image creates an image object

		@param filePath The path to the file that contains the image
	*/
	Image(std::string const& filePath);
	/*
		@function ~image destroys the image texture and cleanly deallocate in openGL
	*/
	~Image();
};